#ifndef MODELS_H
#define MODELS_H
#define TABLE_SIZE_TANAMAN 31
#define TABLE_SIZE_PANEN 53

// ==========================================
// 1. ENUM & UNION (Konsep 6.G: Tagged Union)
// ==========================================
typedef enum {
    BIAYA_BIBIT,
    BIAYA_PUPUK,
    BIAYA_TENAGA_KERJA,
    BIAYA_LAINNYA
} JenisBiaya;

typedef union {
    float biaya_rupiah;
    int jumlah_hari_kerja;
    float jumlah_barang;
} NilaiBiaya;

// Struct pembungkus Biaya Produksi (F06 & F07)
// Menyimpan tag enum agar program tidak salah membaca anggota union aktif
typedef struct {
    int id_biaya;
    int id_tanaman;
    JenisBiaya jenis;      // Penanda (tag) union yang aktif
    NilaiBiaya nilai;      // Nilai union
    float nominal_rupiah;  // Nilai konversi rupiah untuk kalkulasi total biaya & laba (F07 & F14)
    char keterangan[100];
} BiayaProduksi;

// ==========================================
// 2. ENTITY STRUCTS (Konsep 6.F: F01 - F15)
// ==========================================
typedef struct {
    int id;
    char nama[50];
    char kategori[30];
    char satuan[15];
    float harga_jual;
    float stok;
} Tanaman;

typedef struct {
    int id_panen;
    int id_tanaman;
    char tanggal[11];      // Format: DD-MM-YYYY
    float jumlah_panen;
    char kualitas[20];
    char lokasi[50];       // Opsional sesuai F04
} DataPanen;

typedef struct {
    int id_transaksi;
    int id_tanaman;
    char nama_pembeli[50];
    char tanggal[11];      // Format: DD-MM-YYYY
    float jumlah;
    float harga_satuan;
    float total_penjualan;
} TransaksiPenjualan;

typedef struct {
    int id_distribusi;
    int id_transaksi;      // Relasi ke transaksi penjualan terkait (opsional/0 jika kirim langsung)
    int id_tanaman;
    char tujuan[100];
    char penerima[50];
    char tanggal_kirim[11]; // Format: DD-MM-YYYY
    float jumlah;
    float biaya_distribusi;
    char status[20];       // Contoh: "Diproses", "Dalam Perjalanan", "Terkirim"
} Distribusi;

// ==========================================
// 3. DOUBLY LINKED LIST NODES (Konsep 6.H)
// ==========================================
typedef struct NodeTanaman {
    Tanaman data;
    struct NodeTanaman *prev;
    struct NodeTanaman *next;
} NodeTanaman;

typedef struct NodePanen {
    DataPanen data;
    struct NodePanen *prev;
    struct NodePanen *next;
} NodePanen;

typedef struct NodeBiaya {
    BiayaProduksi data;
    struct NodeBiaya *prev;
    struct NodeBiaya *next;
} NodeBiaya;

typedef struct NodePenjualan {
    TransaksiPenjualan data;
    struct NodePenjualan *prev;
    struct NodePenjualan *next;
} NodePenjualan;

typedef struct NodeDistribusi {
    Distribusi data;
    struct NodeDistribusi *prev;
    struct NodeDistribusi *next;
} NodeDistribusi;

// ==========================================
// 4. STACK NODE UNTUK RIWAYAT & UNDO (Konsep 6.I)
// ==========================================
typedef enum {
    AKSI_TAMBAH_TANAMAN,
    AKSI_UBAH_HARGA,
    AKSI_TAMBAH_PANEN,
    AKSI_TAMBAH_PENJUALAN,
    AKSI_TAMBAH_DISTRIBUSI
} TipeAksi;

typedef struct NodeRiwayat {
    char deskripsi[100];
    TipeAksi jenis_aksi;
    int id_target;         // Menyimpan ID data/tanaman yang diubah
    float nilai_lama;      // Snapshot stok atau harga sebelum aksi (memudahkan Undo)
    struct NodeRiwayat *next;
} NodeRiwayat;

// ==========================================
// 5. DEKLARASI GLOBAL HEAD, TAIL & TOP (extern)
// ==========================================
// Inisialisasi (= NULL) wajib diletakkan satu kali di dalam main.c
extern NodeTanaman *hashTableTanaman[TABLE_SIZE_TANAMAN];
extern NodePanen *hashTablePanen[TABLE_SIZE_PANEN];
extern NodeBiaya *headBiaya, *tailBiaya;
extern NodePenjualan *headPenjualan, *tailPenjualan;
extern NodeDistribusi *headDistribusi, *tailDistribusi;
extern NodeRiwayat *topRiwayat;

#endif
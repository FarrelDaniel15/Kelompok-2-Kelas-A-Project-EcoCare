#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "models.h"
#include "tanaman.h"
#include "storage_undo.h"

int id_panen_terakhir = 1;
NodeLogStok *headLogStok = NULL;

// Function hashTanaman untuk melakukan hashing index dengan menggunakan id
unsigned int hashTanaman(int id) {
    if (id < 0) {
        id = -id;
    }
    return (unsigned int)id % TABLE_SIZE_TANAMAN;
}


// Function insertTanamanNode untuk load seluruh node doubly linked lists yang diemplementasikan dengan hash-table
void insertTanamanNode(Tanaman data) {
    unsigned int index = hashTanaman(data.id);
    NodeTanaman *baru = (NodeTanaman*) malloc(sizeof(NodeTanaman));
    if (baru == NULL) {
        printf("\nERROR: Gagal mengalokasikan memori untuk data tanaman!\n");
        return;
    }

    baru->data = data;
    baru->prev = NULL;
    baru->next = hashTableTanaman[index];

    if (hashTableTanaman[index] != NULL) {
        hashTableTanaman[index]->prev = baru;
    }
    hashTableTanaman[index] = baru;
    return;
}


// Function cariTanamanById untuk mencari data tanaman berdasarkan ID
NodeTanaman* cariTanamanById(int id) {
    unsigned int index = hashTanaman(id);
    NodeTanaman* curr = hashTableTanaman[index];

    while (curr != NULL) {
        if (curr->data.id == id) {
            return curr;
        }
        curr = curr->next;
    }
    return NULL;
}


// Function hapusTanamanById untuk hapus tanaman berdasarkan ID
void hapusTanamanById(int id) {
    unsigned int index = hashTanaman(id);
    NodeTanaman* curr = hashTableTanaman[index];

    while (curr != NULL)
    {
        if (curr->data.id == id) {
            if (curr->prev != NULL) {
                curr->prev->next = curr->next;
            }
            else {
                hashTableTanaman[index] = curr->next;
            }

            if (curr->next != NULL) {
                curr->next->prev = curr->prev;
            }

            free(curr);
            return;
        }
        curr = curr->next;
    }
    return;
}


// Function hashPanen untuk melakukan hashing index dengan menggunakan id
unsigned int hashPanen(int id_panen) {
    if (id_panen < 0) {
        id_panen = -id_panen;
    }
    return (unsigned int)id_panen % TABLE_SIZE_PANEN;
}


// Function insertPanenNode untuk load seluruh data dari file.txt menjadi sebuah hash table dengan bucket berupa doubly linked lists
void insertPanenNode(DataPanen data) {
    unsigned int index = hashPanen(data.id_panen);
    NodePanen *baru = (NodePanen*) malloc(sizeof(NodePanen));
    if (baru == NULL) {
        printf("\nERROR: Program gagal mengalokasikan memori untuk data panen!\n");
        return;
    }

    baru->data = data;
    baru->prev = NULL;
    baru->next = hashTablePanen[index];

    if (baru->data.id_panen >= id_panen_terakhir) {
        id_panen_terakhir = baru->data.id_panen + 1;
    }

    if (hashTablePanen[index] != NULL) {
        hashTablePanen[index]->prev = baru;
    }
    hashTablePanen[index] = baru;
    return;
}


// Function cariPanenById untuk mencari data panen berdasarkan ID panen yang diberikan lalu mengembalikan NodePanen yang sesuai
NodePanen *cariPanenById(int id_panen) {
    unsigned int index = hashPanen(id_panen);
    NodePanen *curr = hashTablePanen[index];

    while (curr != NULL) {
        if (curr->data.id_panen == id_panen) {
            return curr;
        }
        curr = curr->next;
    }
    return NULL;
}


// Function hapusPanenById untuk menghapus data panen berdasarkan ID panen yang akan digunakan saat proses (undo)
void hapusPanenById(int id_panen) {
    unsigned int index = hashPanen(id_panen);
    NodePanen *curr = hashTablePanen[index];

    while (curr != NULL) {
        if (curr->data.id_panen == id_panen) {
            if (curr->prev != NULL) {
                curr->prev->next = curr->next;
            } 
            else {
                hashTablePanen[index] = curr->next;
            }

            if (curr->next != NULL) {
                curr->next->prev = curr->prev;
            }
            free(curr);
            return;
        }
        curr = curr->next;
    }
    return;
}


// Function catatPerubahanStok untuk mencatat seluruh kegiatan yang mengubah stok sebuah komoditas
void catatPerubahanStok(int id_tanaman, char nama_tanaman[50], char sumber[30], float stok_lama, float stok_baru) {
    if (stok_lama == stok_baru) {
        return;
    }

    NodeLogStok *baru = (NodeLogStok*) malloc(sizeof(NodeLogStok));
    if (baru == NULL) {
        return;
    }

    baru->id_tanaman = id_tanaman;
    strcpy(baru->nama_tanaman, nama_tanaman);
    strcpy(baru->sumber, sumber);
    baru->stok_lama = stok_lama;
    baru->perubahan = stok_baru - stok_lama;
    baru->stok_baru = stok_baru;
    baru->next = headLogStok;
    headLogStok = baru;

    return;
}


// Function simpanKeFileTanaman untuk menyimpan semua data tanaman ke dalam file
void simpanKeFileTanaman(FILE *fp) {
    for (int i = 0; i < TABLE_SIZE_TANAMAN; i++) {
        NodeTanaman *curr = hashTableTanaman[i];
        while (curr != NULL) {
            fprintf(fp, "%d|%s|%s|%s|%.2f|%.2f\n",
                    curr->data.id,
                    curr->data.nama,
                    curr->data.kategori,
                    curr->data.satuan,
                    curr->data.harga_jual,
                    curr->data.stok);
            curr = curr->next;
        }
    }
}


/* MENU 2 (Kelola Data Tanaman): Awal*/
/* FITUR 1: Awal, Tampilkan daftar tanaman (ID, Nama, Kategori, Satuan, Harga Jual, Stok) */
void tampilkanDaftarTanaman() {
    int adaData = 0;
    printf("\n==================================== Daftar Tanaman =====================================\n");
    printf("%-6s | %-20s | %-15s | %-8s | %-14s | %-10s\n",
           "ID", "Nama Tanaman", "Kategori", "Satuan", "Harga Jual", "Stok");
    printf("-----------------------------------------------------------------------------------------\n");

    for (int i = 0; i < TABLE_SIZE_TANAMAN; i++) {
        NodeTanaman *curr = hashTableTanaman[i];
        while (curr != NULL)
        {
            printf("%-6d | %-20s | %-15s | %-8s | Rp %-11.0f | %-10.2f\n", 
                curr->data.id, curr->data.nama, curr->data.kategori, curr->data.satuan, curr->data.harga_jual, curr->data.stok);
            adaData = 1;
            curr = curr->next;
        }
    }

    if (!adaData)
    {
        printf("Belum ada data tanaman yang tersimpan\n");
    }
    return;
}
/* FITUR 1: Akhir */


/* FITUR 2: Awal, Tambah data tanaman (Meminta user memasukkan ID, Nama, Kategori, Satuan, Harga Jual, Stok) */
void tambahDataTanaman() {
    Tanaman tanamanBaru;

    printf("\n===== Tambah Data Tanaman =====");
    printf("\nMasukkan ID tanaman        : ");
    scanf("%d", &tanamanBaru.id);
    while (getchar() != '\n'); // Mengambil "enter" yang tersisa

    //Panggil function cari data tanaman di sini untuk mengecek bahwa ID belum digunakan
    if (cariTanamanById(tanamanBaru.id) != NULL) {
        printf("\nID tanaman telah digunakan. Gunakan ID lain\n");
        return;
    }

    printf("\nMasukkan nama tanaman      : ");
    fgets(tanamanBaru.nama, sizeof(tanamanBaru.nama), stdin);
    tanamanBaru.nama[strcspn(tanamanBaru.nama, "\n")] = '\0'; // Menghilangkan enter menjadi escape character string

    printf("\nMasukkan kategori tanaman  :");
    fgets(tanamanBaru.kategori, sizeof(tanamanBaru.kategori), stdin);
    tanamanBaru.kategori[strcspn(tanamanBaru.kategori, "\n")] = '\0'; // Menghilangkan enter menjadi escape character string

    printf("\nMasukkan satuan tanaman    :");
    fgets(tanamanBaru.satuan, sizeof(tanamanBaru.satuan), stdin);
    tanamanBaru.satuan[strcspn(tanamanBaru.satuan, "\n")] = '\0'; // Menghilangkan enter menjadi escape character string

    printf("\nMasukkan harga jual tanaman: ");
    scanf("%f", &tanamanBaru.harga_jual);
    while (getchar() != '\n'); // Mengambil "enter" yang tersisa
    if (tanamanBaru.harga_jual < 0) {
        printf("\nHarga jual tanaman tidak boleh negatif\n");
        return;
    }

    printf("\nMasukkan stok tanaman      : ");
    scanf("%f", &tanamanBaru.stok);
    while (getchar() != '\n'); // Mengambil "enter" yang tersisa
    if (tanamanBaru.stok < 0) {
        printf("\nStok tanaman tidak boleh negatif\n");
        return;
    }

    insertTanamanNode(tanamanBaru);
    catatPerubahanStok(tanamanBaru.id, tanamanBaru.nama, "Tambah Tanaman", 0.0f, tanamanBaru.stok);
    // TODO: Tambahkan function untuk push ke riwayat (setelah storage_undo dikerjakan)

    printf("\nPenambahan data tanaman berhasil!");
    return;
}
/* FITUR 2: Akhir */


/* FITUR 3: Awal, Ubah data tanaman (Masukkan ID tanaman, lalu bisa ubah ID, Nama, Kategori, Satuan, Harga Jual, Stok) */
void ubahDataTanaman() {
    int id_tanaman_lama, id_tanaman_baru;

    printf("\n===== Ubah Data Tanaman =====");
    printf("\nMasukkan ID tanaman        : ");
    scanf("%d", &id_tanaman_lama);
    while (getchar() != '\n'); // Mengambil "enter" yang tersisa
    NodeTanaman *nodeTanamanUbah = cariTanamanById(id_tanaman_lama);
    if (nodeTanamanUbah == NULL) {
        printf("\nID tanaman tidak ditemukan\n");
        return;
    }

    Tanaman dataTanamanUbah = nodeTanamanUbah->data;
    printf("\nData tanaman saat ini:\n");
    printf("ID: %d | Nama: %s | Kategori: %s | Satuan: %s | Harga Jual: %.0f | Stok: %.2f\n", dataTanamanUbah.id, dataTanamanUbah.nama, dataTanamanUbah.kategori, dataTanamanUbah.satuan, dataTanamanUbah.harga_jual, dataTanamanUbah.stok);

    printf("\nUbah ID tanaman        : ");
    scanf("%d", &id_tanaman_baru);
    while (getchar() != '\n'); // Mengambil "enter" yang tersisa
    if (id_tanaman_baru != id_tanaman_lama && cariTanamanById(id_tanaman_baru) != NULL) {
        printf("\nID tanaman telah digunakan tanaman lain. Gunakan ID lain\n");
        return;
    }
    dataTanamanUbah.id = id_tanaman_baru;

    printf("\nUbah nama tanaman      : ");
    fgets(dataTanamanUbah.nama, sizeof(dataTanamanUbah.nama), stdin);
    dataTanamanUbah.nama[strcspn(dataTanamanUbah.nama, "\n")] = '\0'; // Menghilangkan enter menjadi escape character string

    printf("\nUbah kategori tanaman  :");
    fgets(dataTanamanUbah.kategori, sizeof(dataTanamanUbah.kategori), stdin);
    dataTanamanUbah.kategori[strcspn(dataTanamanUbah.kategori, "\n")] = '\0'; // Menghilangkan enter menjadi escape character string

    printf("\nUbah satuan tanaman    :");
    fgets(dataTanamanUbah.satuan, sizeof(dataTanamanUbah.satuan), stdin);
    dataTanamanUbah.satuan[strcspn(dataTanamanUbah.satuan, "\n")] = '\0'; // Menghilangkan enter menjadi escape character string

    float hargaJualLama = dataTanamanUbah.harga_jual;
    printf("\nUbah harga jual tanaman: ");
    scanf("%f", &dataTanamanUbah.harga_jual);
    while (getchar() != '\n'); // Mengambil "enter" yang tersisa
    if (dataTanamanUbah.harga_jual < 0) {
        dataTanamanUbah.harga_jual = hargaJualLama;
        printf("\nHarga jual tanaman tidak boleh negatif. Perubahan dibatalkan.\n");
        return;
    }

    float stokTanamanLama = dataTanamanUbah.stok;
    printf("\nUbah stok tanaman      : ");
    scanf("%f", &dataTanamanUbah.stok);
    while (getchar() != '\n'); // Mengambil "enter" yang tersisa
    if (dataTanamanUbah.stok < 0) {
        dataTanamanUbah.stok = stokTanamanLama;
        printf("\nStok tanaman tidak boleh negatif. Perubahan dibatalkan.\n");
        return;
    }

    // Jika ID berubah, hapus node lama lalu alokasikan node baru. Jika tidak berubah, maka hanya update data yang ada di dalam node
    if (id_tanaman_baru != id_tanaman_lama) {
        hapusTanamanById(id_tanaman_lama);
        insertTanamanNode(dataTanamanUbah);
    }
    else {
        nodeTanamanUbah->data = dataTanamanUbah;
    }

    catatPerubahanStok(dataTanamanUbah.id, dataTanamanUbah.nama, "Edit Data (Menu 2)", stokTanamanLama, dataTanamanUbah.stok);
    // TODO: Tambahkan function untuk push ke riwayat (setelah storage_undo dikerjakan)

    printf("\nPerubahan data tanaman berhasil diubah!\n");

    return;
}
/* FITUR 3: Akhir */


/* FITUR 4: Awal, Hapus data tanaman (Meminta user memasukkan ID tanaman yang mau dihapus, lalu hapus node dari doubly linked lists) */
void hapusDataTanaman() {
    int id_tanaman;
    char konfirmasi;

    printf("\n===== Hapus Data Tanaman =====");
    printf("\nMasukkan ID tanaman        : ");
    scanf("%d", &id_tanaman);
    while (getchar() != '\n'); // Mengambil "enter" yang tersisa
    NodeTanaman *nodeTanamanHapus = cariTanamanById(id_tanaman);
    if (nodeTanamanHapus == NULL) {
        printf("\nID tanaman tidak ditemukan. Hapus tanaman dibatalkan.\n");
        return;
    }

    Tanaman dataTanamanHapus = nodeTanamanHapus->data;
    printf("\nData tanaman saat ini:\n");
    printf("ID: %d | Nama: %s | Kategori: %s | Satuan: %s | Harga Jual: %.0f | Stok: %.2f\n", dataTanamanHapus.id, dataTanamanHapus.nama, dataTanamanHapus.kategori, dataTanamanHapus.satuan, dataTanamanHapus.harga_jual, dataTanamanHapus.stok);

    printf("Yakin ingin hapus data tanaman? (y/n): ");
    scanf(" %c", &konfirmasi);
    while (getchar() != '\n'); // Mengambil "enter" yang tersisa
    if (konfirmasi == 'y' || konfirmasi == 'Y') {
        if (dataTanamanHapus.stok > 0) {
            catatPerubahanStok(dataTanamanHapus.id, dataTanamanHapus.nama, "Hapus Tanaman", dataTanamanHapus.stok, 0.0f);
        }
        printf("\nPenghapusan data tanaman berhasil!\n");
        hapusTanamanById(id_tanaman);
    }
    else {
        printf("\nPenghapusan data dibatalkan.\n");
    }

    return;
}
/* FITUR 4: Akhir */


/* FITUR 5: Awal, Cari data tanaman (Meminta user memasukkan ID tanaman yang mau dicari, lalu tampilkan hasilnya) */
void cariDataTanaman() {
    int id_tanaman;

    printf("\n===== Cari Data Tanaman =====");
    printf("\nMasukkan ID tanaman        : ");
    scanf("%d", &id_tanaman);
    while (getchar() != '\n'); // Mengambil "enter" yang tersisa
    NodeTanaman *nodeTanamanCari = cariTanamanById(id_tanaman);
    if (nodeTanamanCari == NULL) {
        printf("\nID tanaman tidak ditemukan.\n");
        return;
    }

    Tanaman dataTanamanCari = nodeTanamanCari->data;
    printf("\nData tanaman:\n");
    printf("ID: %d | Nama: %s | Kategori: %s | Satuan: %s | Harga Jual: %.0f | Stok: %.2f\n", dataTanamanCari.id, dataTanamanCari.nama, dataTanamanCari.kategori, dataTanamanCari.satuan, dataTanamanCari.harga_jual, dataTanamanCari.stok);
    return;
}
/* FITUR 5: Akhir */

/* Tampilan MENU 2 */
void menu2KelolaTanaman() {
    int opsi;
    do {
        printf("\n========== Kelola Data Tanaman ==========\n");
        printf("Ketik angka 0 - 5 untuk memilih submenu:\n");
        printf("0. Kembali ke menu utama\n");
        printf("1. Tampilkan daftar tanaman\n");
        printf("2. Tambahkan data tanaman\n");
        printf("3. Ubah data tanaman\n");
        printf("4. Hapus data tanaman\n");
        printf("5. Cari data tanaman\n");
        printf("Pilihan Anda: ");
        
        if (scanf("%d", &opsi) != 1) {
            while (getchar() != '\n');
            opsi = -1;
        } else {
            while (getchar() != '\n');
        }

        switch (opsi) {
            case 1:
                tampilkanDaftarTanaman();
                break;
            case 2:
                tambahDataTanaman();
                break;
            case 3:
                ubahDataTanaman();
                break;
            case 4:
                hapusDataTanaman();
                break;
            case 5:
                cariDataTanaman();
                break;
            case 0:
                printf("\nKembali ke menu utama...\n");
                break;
            default:
                printf("\nPilihan tidak valid! Silakan masukkan angka 0 - 5.\n");
                break;
        }
    } while (opsi != 0);
    return;
}
/* MENU 2: Akhir*/


/* MENU 3 (Pencatatan Hasil Panen): Awal */
/* FITUR 1: Awal, Menambah hasil panen (meminta ID tanaman, tanggal panen, jumlah hasil panen, kualitas, dan lokasi). Lalu perbarui stok tanaman yang dipanen */
void tambahHasilPanen() {
    int id_tanaman;
    DataPanen dataPanenBaru;
    dataPanenBaru.id_panen = id_panen_terakhir;

    printf("\n======= Tambah Hasil Panen =======");
    printf("\nMasukkan ID tanaman                : ");
    scanf("%d", &id_tanaman);
    while (getchar() != '\n'); // Mengambil "enter" yang tersisa

    NodeTanaman *nodeTanamanCari = cariTanamanById(id_tanaman);
    if (nodeTanamanCari == NULL) {
        printf("\nID tanaman tidak ditemukan.\n");
        return;
    }
    dataPanenBaru.id_tanaman = nodeTanamanCari->data.id;

    printf("\nMasukkan tanggal panen (DD-MM-YYYY): ");
    fgets(dataPanenBaru.tanggal, sizeof(dataPanenBaru.tanggal), stdin);
    if (strchr(dataPanenBaru.tanggal, '\n') == NULL) {
        while (getchar() != '\n'); // Buang sisa karakter jika input >= 10 karakter
    } else {
        dataPanenBaru.tanggal[strcspn(dataPanenBaru.tanggal, "\n")] = '\0';
    }

    printf("\nMasukkan jumlah hasil panen (angka): ");
    scanf("%f", &dataPanenBaru.jumlah_panen);
    while (getchar() != '\n'); // Mengambil "enter" yang tersisa
    if (dataPanenBaru.jumlah_panen <= 0) {
        printf("\nJumlah hasil panen harus lebih besar dari nol.\n");
        return;
    }

    printf("\nMasukkan kualitas panen            : ");
    fgets(dataPanenBaru.kualitas, sizeof(dataPanenBaru.kualitas), stdin);
    dataPanenBaru.kualitas[strcspn(dataPanenBaru.kualitas, "\n")] = '\0'; // Menghilangkan enter menjadi escape character string

    printf("\nMasukkan lokasi panen              : ");
    fgets(dataPanenBaru.lokasi, sizeof(dataPanenBaru.lokasi), stdin);
    dataPanenBaru.lokasi[strcspn(dataPanenBaru.lokasi, "\n")] = '\0'; // Menghilangkan enter menjadi escape character string

    insertPanenNode(dataPanenBaru);
    printf("\nPenambahan data panen berhasil!\n");

    float stokLama = nodeTanamanCari->data.stok;
    nodeTanamanCari->data.stok += dataPanenBaru.jumlah_panen;
    catatPerubahanStok(nodeTanamanCari->data.id, nodeTanamanCari->data.nama, "Input Panen", stokLama, nodeTanamanCari->data.stok);

    // TODO: Tambahkan function untuk push ke riwayat (setelah storage_undo dikerjakan)

    return;
}
/* FITUR 1: Akhir */


/* FITUR 2: Awal, Tampilkan riwayat panen (tampilkan semua data panen dari ID panen, ID tanaman, tanggal panen, jumlah hasil panen, kualitas, dan lokasi) */
void tampilkanRiwayatPanen() {
    int adaData = 0;
    printf("\n======================================== Riwayat Panen =========================================\n");
    printf("%-8s | %-10s | %-13s | %-12s | %-15s | %-23s\n",
           "ID Panen", "ID Tanaman", "Tanggal Panen", "Jumlah Panen", "Kualitas", "Lokasi");
    printf("------------------------------------------------------------------------------------------------\n");

    for (int i = 0; i < TABLE_SIZE_PANEN; i++) {
        NodePanen *curr = hashTablePanen[i];
        while (curr != NULL) {
            printf("%-8d | %-10d | %-13s | %-12.2f | %-15s | %-23s\n",
                   curr->data.id_panen,
                   curr->data.id_tanaman,
                   curr->data.tanggal,
                   curr->data.jumlah_panen,
                   curr->data.kualitas,
                   curr->data.lokasi);
            adaData = 1;
            curr = curr->next;
        }
    }

    if (!adaData) {
        printf("Belum ada data riwayat panen yang tersimpan.\n");
    }
    printf("================================================================================================\n");
    return;
}
/* FITUR 2: Akhir */


/* FITUR 3: Awal, Rekapitulasi panen setiap berdasarkan periode/ komoditas */
void rekapitulasiPanen() {
    int pilihan;
    printf("\n======= Rekapitulasi Hasil Panen =======\n");
    printf("1. Rekapitulasi per Komoditas (Tanaman)\n");
    printf("2. Rekapitulasi per Periode (Bulan-Tahun)\n");
    printf("Pilihan Anda: ");
    if (scanf("%d", &pilihan) != 1) {
        while (getchar() != '\n');
        printf("\nInput tidak valid.\n");
        return;
    }
    while (getchar() != '\n');

    if (pilihan == 1) {
        int adaKomoditas = 0;
        printf("\n======================== Rekapitulasi Panen per Komoditas ========================\n");
        printf("%-8s | %-20s | %-12s | %-15s | %-10s\n",
               "ID", "Nama Komoditas", "Frekuensi", "Total Panen", "Satuan");
        printf("----------------------------------------------------------------------------------\n");

        // Loop luar untuk setiap tanaman di hashTableTanaman
        for (int i = 0; i < TABLE_SIZE_TANAMAN; i++) {
            NodeTanaman *currTanaman = hashTableTanaman[i];
            while (currTanaman != NULL) {
                adaKomoditas = 1;
                int frekuensi = 0;
                float totalPanen = 0.0f;

                // Loop dalam untuk menjumlahkan setiap panen dari tanaman
                for (int j = 0; j < TABLE_SIZE_PANEN; j++) {
                    NodePanen *currPanen = hashTablePanen[j];
                    while (currPanen != NULL) {
                        if (currPanen->data.id_tanaman == currTanaman->data.id) {
                            frekuensi++;
                            totalPanen += currPanen->data.jumlah_panen;
                        }
                        currPanen = currPanen->next;
                    }
                }
                printf("%-8d | %-20s | %-12d | %-15.2f | %-10s\n", currTanaman->data.id, currTanaman->data.nama, frekuensi, totalPanen, currTanaman->data.satuan);
                currTanaman = currTanaman->next;
            }
        }
        if (!adaKomoditas) {
            printf("Belum ada data komoditas tanaman yang tersimpan.\n");
        }
        printf("==================================================================================\n");
    }
    else if (pilihan == 2) {
        char periodeTarget[8]; // format MM-YYYY
        printf("\nMasukkan periode bulan-tahun (MM-YYYY): ");
        fgets(periodeTarget, sizeof(periodeTarget), stdin);
        if (strchr(periodeTarget, '\n') == NULL) {
            while (getchar() != '\n');
        } else {
            periodeTarget[strcspn(periodeTarget, "\n")] = '\0';
        }

        int adaData = 0;
        int totalFrekuensi = 0;
        float totalSemuaPanen = 0.0f;

        printf("\n=================== Rekapitulasi Panen Periode %s ===================\n", periodeTarget);
        printf("%-8s | %-20s | %-12s | %-15s | %-10s\n",
               "ID", "Nama Komoditas", "Frekuensi", "Total Panen", "Satuan");
        printf("----------------------------------------------------------------------------\n");

        for (int i = 0; i < TABLE_SIZE_TANAMAN; i++) {
            NodeTanaman *currTanaman = hashTableTanaman[i];
            while (currTanaman != NULL)
            {
                int frekuensi = 0;
                float totalPanen = 0.0f;

                for (int j = 0; j < TABLE_SIZE_PANEN; j++) {
                    NodePanen *currPanen = hashTablePanen[j];
                    while (currPanen != NULL)
                    {
                        // Cek kecocokan ID dan 7 karakter terakhir dari tanggal
                        if (currPanen->data.id_tanaman == currTanaman->data.id && strlen(currPanen->data.tanggal) >= 10 && strcmp(&currPanen->data.tanggal[3], periodeTarget) == 0) {
                            frekuensi++;
                            totalPanen += currPanen->data.jumlah_panen;
                        }
                        currPanen = currPanen->next;
                    }
                }

                if (frekuensi > 0) {
                    printf("%-8d | %-20s | %-12d | %-15.2f | %-10s\n", currTanaman->data.id, currTanaman->data.nama, frekuensi, totalPanen, currTanaman->data.satuan);
                    adaData = 1;
                    totalFrekuensi += frekuensi;
                    totalSemuaPanen += totalPanen;
                }

                currTanaman = currTanaman->next;
            }
        }

        if (!adaData) {
            printf("Tidak ada catatan panen pada periode %s.\n", periodeTarget);
        }
        else {
            printf("----------------------------------------------------------------------------\n");
            printf("Total Frekuensi Panen: %d kali | Total Volume Panen: %.2f\n", totalFrekuensi, totalSemuaPanen);
        }
        printf("============================================================================\n");
    }
    else {
        printf("\nPilihan tidak valid\n");
    }
    return;
}
/* FITUR 3: Akhir */


/* Tampilan MENU 3 */
void menu3PencatatanHasilPanen() {
    int opsi;
    do {
        printf("\n========== Pencatatan Hasil Panen ==========\n");
        printf("Ketik angka 0 - 3 untuk memilih submenu:\n");
        printf("0. Kembali ke menu utama\n");
        printf("1. Tambahkan hasil panen\n");
        printf("2. Tampilkan riwayat panen\n");
        printf("3. Rekapitulasi hasil panen\n");
        printf("Pilihan Anda: ");
        
        if (scanf("%d", &opsi) != 1) {
            while (getchar() != '\n');
            opsi = -1;
        } else {
            while (getchar() != '\n');
        }

        switch (opsi) {
            case 1:
                tambahHasilPanen();
                break;
            case 2:
                tampilkanRiwayatPanen();
                break;
            case 3:
                rekapitulasiPanen();
                break;
            case 0:
                printf("\nKembali ke menu utama...\n");
                break;
            default:
                printf("\nPilihan tidak valid! Silakan masukkan angka 0 - 3.\n");
                break;
        }
    } while (opsi != 0);
    return;
}
/* MENU 3: Akhir*/


/* MENU 4: (Pengelolaan Persediaan): Awal */
/* FITUR 1: Awal, Tampilkan stok */
void tampilkanStok() {
    int adaData = 0;
    printf("\n==================================== Daftar Stok =====================================\n");
    printf("%-6s | %-20s | %-15s | %-8s | %-14s | %-10s\n",
           "ID", "Nama Tanaman", "Kategori", "Satuan", "Harga Jual", "Stok");
    printf("-----------------------------------------------------------------------------------------\n");

    float totalStok = 0, stokKomoditasTerbanyak = -1.0f;
    char komoditasTerbanyak[50] = "-", satuanKomoditasTerbanyak[15];

    for (int i = 0; i < TABLE_SIZE_TANAMAN; i++) {
        NodeTanaman *curr = hashTableTanaman[i];
        while (curr != NULL) {
            printf("%-6d | %-20s | %-15s | %-8s | Rp %-11.0f | %-10.2f\n", 
                curr->data.id, curr->data.nama, curr->data.kategori, curr->data.satuan, curr->data.harga_jual, curr->data.stok);
            adaData = 1;
            totalStok += curr->data.stok;

            if (stokKomoditasTerbanyak <= curr->data.stok) {
                strcpy(komoditasTerbanyak, curr->data.nama);
                strcpy(satuanKomoditasTerbanyak, curr->data.satuan);
                stokKomoditasTerbanyak = curr->data.stok;
            }
            curr = curr->next;
        }
    }

    if (!adaData) {
            printf("Tidak ada data stok tanaman.\n");
        }
        else {
            printf("-----------------------------------------------------------------------------------------\n");
            printf("Total Seluruh Stok: %.2f | Komoditas terbanyak: %s (%.2f %s)\n", totalStok, komoditasTerbanyak, stokKomoditasTerbanyak, satuanKomoditasTerbanyak);
        }
        printf("=========================================================================================\n");
    return;
}
/* FITUR 1: Akhir */


/* FITUR 2: Awal, Cari stok komoditas */
void cariStok() {
    int id_tanaman;

    printf("\n===== Cari Stok =====");
    printf("\nMasukkan ID tanaman        : ");
    scanf("%d", &id_tanaman);
    while (getchar() != '\n'); // Mengambil "enter" yang tersisa
    NodeTanaman *nodeTanamanCekStok = cariTanamanById(id_tanaman);
    if (nodeTanamanCekStok == NULL) {
        printf("\nID tanaman tidak ditemukan\n");
        return;
    }

    printf("\n=================================== Data Stok Tanaman ====================================\n");
    printf("%-6s | %-20s | %-15s | %-8s | %-14s | %-10s\n",
           "ID", "Nama Tanaman", "Kategori", "Satuan", "Harga Jual", "Stok");
    printf("-----------------------------------------------------------------------------------------\n");
    printf("%-6d | %-20s | %-15s | %-8s | Rp %-11.0f | %-10.2f\n", 
        nodeTanamanCekStok->data.id, nodeTanamanCekStok->data.nama, nodeTanamanCekStok->data.kategori, nodeTanamanCekStok->data.satuan, nodeTanamanCekStok->data.harga_jual, nodeTanamanCekStok->data.stok);
    if (nodeTanamanCekStok->data.stok == 0) {
        printf("CATATAN: Stok HABIS\n");
    }
    else {
        printf("CATATAN: Stok TERSEDIA\n");
    }
    printf("=========================================================================================\n");
    return;
}
/* FITUR 2: Akhir */


/* FITUR 3: Awal, Periksa stok minimum (minta user batas minimum stok, lalu cetak semua data tanaman yang kurang dari batas minimum) */
void periksaStokMinimum() {
    int adaData = 0, totalMenipis = 0, totalHabis = 0;
    float batasMinimum = 0;

    printf("\n===== Periksa Stok Minimum =====");
    printf("\nMasukkan batas stok minimum: ");
    scanf("%f", &batasMinimum);
    while (getchar() != '\n'); // Mengambil "enter" yang tersisa
    if (batasMinimum < 0) {
        printf("\nBatas stok minimum tidak bisa negatif.\n");
        return;
    }

    printf("\n================================= Daftar Stok di Bawah Batas Minimum ==================================\n");
    printf("%-6s | %-20s | %-15s | %-8s | %-14s | %-10s | %-12s\n",
           "ID", "Nama Tanaman", "Kategori", "Satuan", "Harga Jual", "Stok", "Status");
    printf("-------------------------------------------------------------------------------------------------------\n");

    for (int i = 0; i < TABLE_SIZE_TANAMAN; i++)
    {
        NodeTanaman *curr = hashTableTanaman[i];
        while (curr != NULL) {
            if (curr->data.stok < batasMinimum) {
                adaData = 1;
                printf("%-6d | %-20s | %-15s | %-8s | Rp %-11.0f | %-10.2f | %-12s\n", 
                curr->data.id, curr->data.nama, curr->data.kategori, curr->data.satuan, curr->data.harga_jual, curr->data.stok, ((curr->data.stok == 0) ? "HABIS" : "MENIPIS"));
                (curr->data.stok == 0) ? totalHabis++ : totalMenipis++;
            }
            curr = curr->next;
        }
    }
    printf("-------------------------------------------------------------------------------------------------------\n");

    if (adaData == 1) {
        printf("Total komoditas yang habis  : %d\n", totalHabis);
        printf("Total komoditas yang menipis: %d\n", totalMenipis);
    }
    else {
        printf("Semua stok komoditas di atas batas minimal\n");
    }
    printf("=======================================================================================================\n");
    return;
}
/* FITUR 3: Akhir */


/* FITUR 4: Awal, Riwayat perubahan stok */
void riwayatPerubahanStok () {
    printf("\n======================================= Riwayat Perubahan Stok ========================================\n");
    printf("%-6s | %-20s | %-20s | %-12s | %-12s | %-12s\n",
           "ID", "Nama Tanaman", "Sumber Perubahan", "Stok Awal", "Perubahan", "Stok Akhir");
    printf("-------------------------------------------------------------------------------------------------------\n");

    if (headLogStok == NULL) {
        printf("Belum ada riwayat perubahan stok.\n");
        printf("=======================================================================================================\n");
        return;
    }

    NodeLogStok *curr = headLogStok;
    while (curr != NULL) {
        printf("%-6d | %-20s | %-20s | %-12.2f | %+-12.2f | %-12.2f\n",
           curr->id_tanaman, curr->nama_tanaman, curr->sumber, curr->stok_lama, curr->perubahan, curr->stok_baru);
        curr = curr->next;
    }
    printf("=======================================================================================================\n");
    return;
}
/* FITUR 4: Akhir */


/* Tampilan MENU 4 */
void menu4PengelolaanPersediaan() {
    int opsi;
    do {
        printf("\n========= Pengelolaan Persediaan =========\n");
        printf("Ketik angka 0 - 4 untuk memilih submenu:\n");
        printf("0. Kembali ke menu utama\n");
        printf("1. Tampilkan stok\n");
        printf("2. Cari stok komoditas\n");
        printf("3. Periksa stok minimum\n");
        printf("4. Riwayat perubahan stok\n");
        printf("Pilihan Anda: ");
        
        if (scanf("%d", &opsi) != 1) {
            while (getchar() != '\n');
            opsi = -1;
        } else {
            while (getchar() != '\n');
        }

        switch (opsi) {
            case 1:
                tampilkanStok();
                break;
            case 2:
                cariStok();
                break;
            case 3:
                periksaStokMinimum();
                break;
            case 4:
                riwayatPerubahanStok();
                break;
            case 0:
                printf("\nKembali ke menu utama...\n");
                break;
            default:
                printf("\nPilihan tidak valid! Silakan masukkan angka 0 - 4.\n");
                break;
        }
    } while (opsi != 0);
    return;
}
/* MENU 4: Akhir */
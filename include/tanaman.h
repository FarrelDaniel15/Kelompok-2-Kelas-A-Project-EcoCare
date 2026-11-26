#ifndef TANAMAN_H
#define TANAMAN_H

#include <stdio.h>
#include "models.h"

// ==========================================
// 1. HELPER HASH TABLE & OPERASI DASAR TANAMAN
// ==========================================
unsigned int hashTanaman(int id);
void insertTanamanNode(Tanaman data);
NodeTanaman* cariTanamanById(int id);
void hapusTanamanById(int id);
void simpanKeFileTanaman(FILE *fp);

// ==========================================
// 2. HELPER HASH TABLE & OPERASI DASAR PANEN
// ==========================================
unsigned int hashPanen(int id_panen);
void insertPanenNode(DataPanen data);
NodePanen* cariPanenById(int id_panen);
void hapusPanenById(int id_panen);

// ==========================================
// 3. HELPER LOG PERUBAHAN STOK
// ==========================================
void catatPerubahanStok(int id_tanaman, const char *nama_tanaman, const char *sumber, float stok_lama, float stok_baru);

// ==========================================
// 4. MENU 2: KELOLA DATA TANAMAN
// ==========================================
void menu2KelolaTanaman(void);
void tampilkanDaftarTanaman(void);
void tambahDataTanaman(void);
void ubahDataTanaman(void);
void hapusDataTanaman(void);
void cariDataTanaman(void);

// ==========================================
// 5. MENU 3: PENCATATAN HASIL PANEN
// ==========================================
void menu3PencatatanHasilPanen(void);
void tambahHasilPanen(void);
void tampilkanRiwayatPanen(void);
void rekapitulasiPanen(void);

// ==========================================
// 6. MENU 4: PENGELOLAAN PERSEDIAAN
// ==========================================
void menu4PengelolaanPersediaan(void);
void tampilkanStok(void);
void cariStok(void);
void periksaStokMinimum(void);
void riwayatPerubahanStok(void);

#endif
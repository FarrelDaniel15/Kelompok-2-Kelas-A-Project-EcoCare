#ifndef TANAMAN_H
#define TANAMAN_H

#include "models.h"

// ==========================================
// 1. HELPER HASH TABLE & OPERASI DASAR TANAMAN
// ==========================================
unsigned int hashTanaman(int id);
void insertTanamanNode(Tanaman data);
NodeTanaman* cariTanamanById(int id);
void hapusTanamanById(int id);

// ==========================================
// 2. HELPER HASH TABLE & OPERASI DASAR PANEN
// ==========================================
unsigned int hashPanen(int id_panen);
void insertPanenNode(DataPanen data);
NodePanen* cariPanenById(int id_panen);
void hapusPanenById(int id_panen);

// ==========================================
// 3. MENU 2: KELOLA DATA TANAMAN
// ==========================================
void menu2KelolaTanaman(void);
void tampilkanDaftarTanaman(void);
void tambahDataTanaman(void);
void ubahDataTanaman(void);
void hapusDataTanaman(void);
void cariDataTanaman(void);

// ==========================================
// 4. MENU 3: PENCATATAN HASIL PANEN
// ==========================================
void menu3PencatatanHasilPanen(void);
void tambahHasilPanen(void);
void tampilkanRiwayatPanen(void);
void rekapitulasiPanen(void);

// (Nanti saat mulai mengerjakan Menu 4,
// tinggal tambahkan prototipe fungsinya di bawah sini)

#endif
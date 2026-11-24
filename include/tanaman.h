#ifndef TANAMAN_H
#define TANAMAN_H

#include "models.h"

// ==========================================
// 1. HELPER HASH TABLE & OPERASI DASAR
// ==========================================
unsigned int hashTanaman(int id);
void insertTanamanNode(Tanaman data);
NodeTanaman* cariTanamanById(int id);
void hapusTanamanById(int id);

// ==========================================
// 2. MENU 2: KELOLA DATA TANAMAN
// ==========================================
void menu2KelolaTanaman(void);
void tampilkanDaftarTanaman(void);
void tambahDataTanaman(void);
void ubahDataTanaman(void);
void hapusDataTanaman(void);
void cariDataTanaman(void);

// (Nanti saat kamu mulai mengerjakan Menu 3 & Menu 4,
// tinggal tambahkan prototipe fungsinya di bawah sini)

#endif
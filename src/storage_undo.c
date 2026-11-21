#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "models.h"
#include "storage_undo.h"
#include "tanaman.h"


// Function untuk mengkoneksikan data dari tanaman.txt ke file tanaman.c
void muatTanaman(void) {
    FILE *fp = fopen("../test/tanaman.txt", "r");
    if (fp == NULL) {
        printf("[Info] File tanaman.txt belum ditemukan, memulai dengan data kosong.\n");
        return;
    }

    char baris[256];
    Tanaman temp;
    while (fgets(baris, sizeof(baris), fp) != NULL) {
        if (sscanf(baris, "%d|%49[^|]|%29[^|]|%14[^|]|%f|%f",
                   &temp.id, temp.nama, temp.kategori,
                   temp.satuan, &temp.harga_jual, &temp.stok) == 6) {
            insertTanamanNode(temp);
        }
    }

    fclose(fp);
}

// Function untuk mengkoneksikan menyimpan data ke dalam file tanaman.txt
void simpanTanaman(void) {
    FILE *fp = fopen("../test/tanaman.txt", "w");
    if (fp == NULL) {
        printf("[Error] Gagal membuka ../test/tanaman.txt untuk menyimpan.\n");
        return;
    }

    simpanKeFileTanaman(fp);
    fclose(fp);
}
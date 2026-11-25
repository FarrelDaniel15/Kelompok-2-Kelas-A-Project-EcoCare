#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "models.h"
#include "tanaman.h"
#include "storage_undo.h"

int id_panen_terakhir;

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
        printf("ERROR: Gagal mengalokasikan memori untuk data tanaman!\n");
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
        printf("ERROR: Program gagal mengalokasikan memori untuk data panen!\n");
        return;
    }

    baru->data = data;
    baru->prev = NULL;
    baru->next = hashTablePanen[index];

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
        printf("Belum ada data tanaman yang tersimpan");
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
    while (getchar() != '\n'); // mengambil "enter" yang tersisa

    //Panggil function cari data tanaman di sini untuk mengecek bahwa ID belum digunakan
    if (cariTanamanById(tanamanBaru.id) != NULL) {
        printf("ID tanaman telah digunakan. Gunakan ID lain");
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
    while (getchar() != '\n'); // mengambil "enter" yang tersisa
    if (tanamanBaru.harga_jual < 0) {
        printf("Harga jual tanaman tidak boleh negatif");
        return;
    }

    printf("\nMasukkan stok tanaman      : ");
    scanf("%f", &tanamanBaru.stok);
    while (getchar() != '\n'); // mengambil "enter" yang tersisa
    if (tanamanBaru.stok < 0) {
        printf("stok tanaman tidak boleh negatif");
        return;
    }

    insertTanamanNode(tanamanBaru);
    printf("\nPenambahan data tanaman berhasil!");

    // TODO: Tambahkan function untuk push ke riwayat (setelah storage_undo dikerjakan)

    return;
}
/* FITUR 2: Akhir */


/* FITUR 3: Awal, Ubah data tanaman (Masukkan ID tanaman, lalu bisa ubah ID, Nama, Kategori, Satuan, Harga Jual, Stok) */
void ubahDataTanaman() {
    int id_tanaman_lama, id_tanaman_baru;

    printf("\n===== Ubah Data Tanaman =====");
    printf("\nMasukkan ID tanaman        : \n");
    scanf("%d", &id_tanaman_lama);
    while (getchar() != '\n'); // mengambil "enter" yang tersisa
    NodeTanaman *nodeTanamanUbah = cariTanamanById(id_tanaman_lama);
    if (nodeTanamanUbah == NULL) {
        printf("ID tanaman tidak ditemukan");
        return;
    }

    Tanaman dataTanamanUbah = nodeTanamanUbah->data;
    printf("\nData tanaman saat ini:\n");
    printf("ID: %d | Nama: %s | Kategori: %s | Satuan: %s | Harga Jual: %.0f | Stok: %.2f\n", dataTanamanUbah.id, dataTanamanUbah.nama, dataTanamanUbah.kategori, dataTanamanUbah.satuan, dataTanamanUbah.harga_jual, dataTanamanUbah.stok);

    printf("\nUbah ID tanaman        : ");
    scanf("%d", &id_tanaman_baru);
    while (getchar() != '\n'); // mengambil "enter" yang tersisa
    if (id_tanaman_baru != id_tanaman_lama && cariTanamanById(id_tanaman_baru) != NULL) {
        printf("ID tanaman telah digunakan tanaman lain. Gunakan ID lain");
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
    while (getchar() != '\n'); // mengambil "enter" yang tersisa
    if (dataTanamanUbah.harga_jual < 0) {
        dataTanamanUbah.harga_jual = hargaJualLama;
        printf("Harga jual tanaman tidak boleh negatif. Perubahan dibatalkan.");
        return;
    }

    float stokTanamanLama = dataTanamanUbah.stok;
    printf("\nUbah stok tanaman      : ");
    scanf("%f", &dataTanamanUbah.stok);
    while (getchar() != '\n'); // mengambil "enter" yang tersisa
    if (dataTanamanUbah.stok < 0) {
        dataTanamanUbah.stok = stokTanamanLama;
        printf("Stok tanaman tidak boleh negatif. Perubahan dibatalkam");
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

    printf("\nPerubahan data tanaman berhasil diubah!\n");

    // TODO: Tambahkan function untuk push ke riwayat (setelah storage_undo dikerjakan)

    return;
}
/* FITUR 3: Akhir */


/* FITUR 4: Awal, Hapus data tanaman (Meminta user memasukkan ID tanaman yang mau dihapus, lalu hapus node dari doubly linked lists) */
void hapusDataTanaman() {
    int id_tanaman;
    char konfirmasi;

    printf("\n===== Hapus Data Tanaman =====");
    printf("\nMasukkan ID tanaman        : \n");
    scanf("%d", &id_tanaman);
    while (getchar() != '\n'); // mengambil "enter" yang tersisa
    NodeTanaman *nodeTanamanHapus = cariTanamanById(id_tanaman);
    if (nodeTanamanHapus == NULL) {
        printf("ID tanaman tidak ditemukan. Hapus tanaman dibatalkan.");
        return;
    }

    Tanaman dataTanamanHapus = nodeTanamanHapus->data;
    printf("\nData tanaman saat ini:\n");
    printf("ID: %d | Nama: %s | Kategori: %s | Satuan: %s | Harga Jual: %.0f | Stok: %.2f\n", dataTanamanHapus.id, dataTanamanHapus.nama, dataTanamanHapus.kategori, dataTanamanHapus.satuan, dataTanamanHapus.harga_jual, dataTanamanHapus.stok);

    printf("Yakin ingin hapus data tanaman? (y/n): ");
    scanf(" %c", &konfirmasi);
    while (getchar() != '\n'); // mengambil "enter" yang tersisa
    if (konfirmasi == 'y' || konfirmasi == 'Y') {
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
    printf("\nMasukkan ID tanaman        : \n");
    scanf("%d", &id_tanaman);
    while (getchar() != '\n'); // mengambil "enter" yang tersisa
    NodeTanaman *nodeTanamanCari = cariTanamanById(id_tanaman);
    if (nodeTanamanCari == NULL) {
        printf("ID tanaman tidak ditemukan.");
        return;
    }

    Tanaman dataTanamanCari = nodeTanamanCari->data;
    printf("\nData tanaman:\n");
    printf("ID: %d | Nama: %s | Kategori: %s | Satuan: %s | Harga Jual: %.0f | Stok: %.2f\n", dataTanamanCari.id, dataTanamanCari.nama, dataTanamanCari.kategori, dataTanamanCari.satuan, dataTanamanCari.harga_jual, dataTanamanCari.stok);
}
/* FITUR 5: Akhir */

/* Tampilan Menu 2 */
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
                printf("Kembali ke menu utama...\n");
                break;
            default:
                printf("Pilihan tidak valid! Silakan masukkan angka 0 - 5.\n");
                break;
        }
    } while (opsi != 0);
}
/* MENU 2: Akhir*/


/* MENU 3 (Pencatatan Hasil Panen): Awal */
/* FITUR 1: Awal, Menambah hasil panen (meminta ID tanaman, tanggal panen, jumlah hasil panen, kualitas, dan lokasi). Lalu perbarui stok tanaman yang dipanen */
/* FITUR 1: Akhir */


/* FITUR 2: Awal, Tampilkan riwayat panen (tampilkan semua data panen dari ID panen, ID tanaman, tanggal panen, jumlah hasil panen, kualitas, dan lokasi) */
/* FITUR 2: Akhir */


/* FITUR 3: Awal, Rekapitulasi panen setiap berdasarkan komoditas */
/* FITUR 3: Akhir */


/* Tampilan Menu 3 */
/* MENU 3: Akhir*/
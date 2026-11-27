#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "models.h"
#include "storage_undo.h"
#include "tanaman.h"
#include "transaksi.h"

static const char *STATUS_DISTRIBUSI = {"Diproses", "Dalam perjalanan", "Terkirim"};

NodePenjualan *tambahNodePenjualan (TransaksiPenjualan *tanaman){
        NodePenjualan *baru = (NodePenjualan *) malloc(sizeof(NodePenjualan));
        if(baru == NULL){
            puts(" ! Memori tidak cukup. ");
            return NULL;
        }
        baru -> data = *tanaman;
        baru -> next = NULL;
        baru -> prev = tailPenjualan;
        if(tailPenjualan == NULL) headPenjualan = baru;
        else tailPenjualan -> next = baru;
        tailPenjualan = baru;
        return baru;
}

NodePenjualan *cariPenjualan (int id){
    for(NodePenjualan *p = headPenjualan; p != NULL; p = p -> next){
        if(p -> data.id_transaksi == id) return p;
        return NULL;
    }
}

int hapusNodePenjualan(int id){
    NodePenjualan *p = cariPenjualan(id);
    if(p == NULL) return 0;
    if(p -> prev != NULL){
        p -> prev -> next = p -> next;
    } else { 
        headPenjualan = p -> next;
    }
    if(p -> next != NULL){
        p -> next -> prev = p -> prev;
    }else{
        tailPenjualan = p -> prev;
    }
    free(p);
    return 1;
}

void bebaskanPenjualan(void){
    NodePenjualan *p = headPenjualan;
    while (p != NULL){
        NodePenjualan *tmp = p -> next;
        free(p);
        p = tmp;
    }
    headPenjualan = tailPenjualan = NULL;
}


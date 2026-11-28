#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "models.h"
#include "storage_undo.h"
#include "tanaman.h"
#include "transaksi.h"

static const char *STATUS_DISTRIBUSI = {"Diproses", "Dalam perjalanan", "Terkirim"};

NodePenjualan *tambahNodePenjualan (TransaksiPenjualan *tanaman){
        NodePenjualan *penjualanBaru = (NodePenjualan *) malloc(sizeof(NodePenjualan));
        if(penjualanBaru == NULL){
            puts(" ! Memori tidak cukup. ");
            return NULL;
        }
        penjualanBaru -> data = *tanaman;
        penjualanBaru -> next = NULL;
        penjualanBaru -> prev = tailPenjualan;
        if(tailPenjualan == NULL) headPenjualan = penjualanBaru;
        else tailPenjualan -> next = penjualanBaru;
        tailPenjualan = penjualanBaru;
        return penjualanBaru;
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

int idPenjualanBerikutnya(void){
    int maksimum = 0;
    for(NodePenjualan *p = headPenjualan; p; p = p -> next)
        if(p -> data.id_transaksi > maksimum){
            maksimum = p -> data.id_transaksi;
        }
    return maksimum + 1;
}

NodeDistribusi *tambahNodeDistribusi(Distribusi *dist){
    NodeDistribusi *distribusiBaru = (NodeDistribusi *) malloc(sizeof(NodeDistribusi));
    if(distribusiBaru == NULL){
        puts(" ! Memori tidak cukup. ");
        return NULL;
    }
    distribusiBaru -> data = *dist;
    distribusiBaru -> next = NULL;
    distribusiBaru -> prev = tailDistribusi;
    if(tailDistribusi == NULL){
        headDistribusi = distribusiBaru;
    }else{
        tailDistribusi -> next = distribusiBaru;
    }
    tailDistribusi = distribusiBaru;
    return distribusiBaru;
}

NodeDistribusi *cariDistribusi(int id){
    for(NodeDistribusi *p = headDistribusi; p != NULL; p = p ->next)
        if(p -> data.id_distribusi == id){
            return p;
        }
    return NULL;
}
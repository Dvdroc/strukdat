#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct QueueNode {
    char tugas[50];
    char deskripsi[100];
    struct QueueNode *next;
}QueueNode;

typedef struct{
    QueueNode *depan;
    QueueNode *belakang;
}Queue;

typedef struct AnggotaNode{
    char nama[50];
    Queue To_do_list;
    struct AnggotaNode* prev;
    struct AnggotaNode* next;
}AnggotaNode;

typedef struct {
    AnggotaNode* head;
    AnggotaNode* Pnew;
}listpekerja;

void cekQueue(Queue *queue){
    queue->depan = NULL;
    queue->belakang = NULL;
}
void cekperkeja(listpekerja *list){
    list->head = NULL;
    list->Pnew =  NULL;
}

QueueNode *CreatQueueuNode(const char *tugas, const char *deskripsi){
     QueueNode *newNode = (QueueNode*)malloc(sizeof(QueueNode));
    if (!newNode) {
        printf("tidak bisa mengalokasikan memori!\n");
        exit(EXIT_FAILURE);
    }
    strcpy(newNode->tugas, tugas);
    strcpy(newNode->deskripsi, deskripsi);
    newNode->next = NULL;
    return newNode;
}
AnggotaNode *Creatnodepekerja(const char *nama){
    AnggotaNode *newnode = (AnggotaNode*)malloc(sizeof(AnggotaNode));
    if(!newnode){
        printf("tidak bisa mengalokasikan memori!\n");
        exit(EXIT_FAILURE);
    }
    strcpy(newnode->nama,nama);
    cekQueue(&newnode->To_do_list);
    newnode->next = NULL;
    newnode->prev = NULL;
    return newnode;
}
void tambahkanpekerja(listpekerja *list, const char *nama){
    AnggotaNode *nodebaru = Creatnodepekerja(nama);
    if(list->Pnew == NULL){
        list->head = list->Pnew = nodebaru;
    }else {
        list->Pnew->next = nodebaru;
        nodebaru-> prev =list ->Pnew;
        list->Pnew = nodebaru;
    }
    printf("pekerja %s baru saja di tambahkan\n", nama);
}

void tampilkananggota(listpekerja *list){
    if(list->head == NULL){
        printf("tidak ada pekerja");
        return;
    }
    AnggotaNode* data = list->head;
    printf("pekerja: ");
    while(data != NULL){
        printf("%s\t", data->nama);
        data = data->next;
    }
    printf("\n");
}

void enqueue(Queue *queue, const char *jobdeks, const char *deskripsi){
    QueueNode *tugas = CreatQueueuNode(jobdeks, deskripsi);
    if(queue->belakang == NULL){
        queue->depan = queue->belakang = tugas;
    }else{
        queue->belakang->next = tugas;
        queue->belakang = tugas;
    }
    printf("tugas '%s' dengan deskripsi '%s' baru saja ditambahkan.\n", jobdesks, deskripsi);
}

void tambahkan_Jobdeks(listpekerja *list, const char *namapekerja, const char *jobdeks, const char *deskripsi){
    AnggotaNode *data = list->head;
    while(data != NULL){
        if (strcmp(data->nama, namapekerja) == 0){
            enqueue(&data->To_do_list, jobdeks, deskripsi);
            return;
        }
        data = data->next;
    }
    printf("nama %s tidak ditemukan dalam tim\n", namapekerja);
}

void jobdeks_selesai(Queue *queue){
    if(queue->depan == NULL){
        printf("tidak ada pekerjaan");
    }
    QueueNode *data = queue ->depan;
    printf("tugas %s telah selesai", data->tugas);
    queue->depan = queue->depan->next;
    if(queue->depan == NULL){
        queue->belakang = NULL;
    }
    free(data);
}

void tampilkan_jobdeks(Queue *queue){
    if(queue->depan == NULL){
        printf("mengangur");
    }else {
        QueueNode *data = queue ->depan;
        printf("list jobdeks");
        while (data != NULL){
            printf("  %s\n", data);
            data = data ->next;
        }
        
    }
}
void tampilkan(listpekerja *list){
    if(list->head == NULL){
        printf("tidak ada pekerja");
        return;
    }
    AnggotaNode* data = list->head;
    while(data != NULL){
        printf("pekerja:%s\n", data->nama);
        tampilkan_jobdeks(&data->To_do_list);
        data = data->next;
    }
}
int main(){
    listpekerja keperluan;
    cekperkeja(&keperluan);
    AnggotaNode *data;
    int pilih;
    char nama[50];
    char jobdeks[50];
    do{
        printf("menu\n");
        printf("1. tambahkan pekerja\n");
        printf("2. tambahkan jobdeks\n");
        printf("3. pekerjaan yang sudah selesai\n");
        printf("4. tampilkan jobdeks pekerja\n");
        printf("masukan pilihan:");
        scanf("%d", &pilih);
        getchar();
        switch (pilih){
            case 1:
                printf("masukan nama pekerja: ");
                fgets(nama, sizeof(nama), stdin);
                nama[strcspn(nama,"\n")] = 0;
                tambahkanpekerja(&keperluan, nama);
                break;
            case 2:
                tampilkananggota(&keperluan);
                printf("masukan nama pekerja: ");
                fgets(nama, sizeof(nama), stdin);
                nama[strcspn(nama,"\n")] = 0;
                printf("masukan jobdeks: ");
                fgets(jobdeks, sizeof(jobdeks), stdin);
                jobdeks[strcspn(jobdeks,"\n")] = 0;
                tambahkan_Jobdeks(&keperluan, nama, jobdeks);
                break;
            case 3:
                tampilkananggota(&keperluan);
                fgets(nama, sizeof(nama), stdin);
                nama[strcspn(nama, "\n")] = 0;
                data = keperluan.head;
                while (data != NULL) {
                    if (strcmp(data->nama, nama) == 0) {
                        jobdeks_selesai(&data->To_do_list);
                        break;
                    }
                    data = data->next;
                }
                if (data == NULL) {
                    printf("Worker '%s' not found.\n", nama);
                }
                break;
            case 4:
                tampilkan(&keperluan);
            default:
                break;
        }
    }while(pilih != 5);
    return 0;
}

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct TugasNode {
    char id[10];
    char tugas[5000];
    char status[300];
    struct TugasNode *next;
}TugasNode;

typedef struct {
    TugasNode *depan;
    TugasNode *belakang;
}Queue;

typedef struct AnggotaNode {
    char id[10];
    char nama[100];
    TugasAnggota* list;
    struct AnggotaNode* next;
}AnggotaNode;

typedef struct TugasAnggota {
    char tugasId[10];
    char tugas[5000];
    char status[300];
    struct TugasAnggota* next;
}TugasAnggota;

typedef struct ProjekNode {
    char id[10];
    char nama[100];
    AnggotaNode* pekerja;
    Queue list;
    struct ProjekNode* kiri;
    struct ProjekNode* kanan;
}ProjekNode;

ProjekNode* createProjek(ProjekNode* root, char* id, char* nama);
void addAnggota(ProjekNode* projek, char* id, char* nama);  
void addTugasAnggota(ProjekNode* projek, char* anggotaId, char* tugasId, char* tugas, char* status);  
void addTask(Queue* queue, char* id, char* tugas, char* status);
// void dltTask(Queue* queue);
void showProjek(ProjekNode* root);
void showAnggota(AnggotaNode* head);
void showTugasAnggota(TugasAnggota* head);
void showTugas(Queue* queue);
ProjekNode* cariProjek(ProjekNode* root, char* id);
void cekQueue(Queue* queue);

void cekQueue(Queue *queue) {
    queue->depan = queue->belakang = NULL;
}

ProjekNode* cariProjek(ProjekNode* root, char* id) {
    while (root && strcmp(root->id, id) != 0) {
        if (strcmp(id, root->id) < 0)
            root = root->kiri;
        else
            root = root->kanan;
    }
    return root;
}

ProjekNode* createProjek(ProjekNode* root, char* id, char* nama) {
    if (!root) {
        ProjekNode* projekBaru = (ProjekNode*)malloc(sizeof(ProjekNode));
        strcpy(projekBaru->id, id);
        strcpy(projekBaru->nama, nama);
        projekBaru->pekerja = NULL;
        cekQueue(&projekBaru->list);
        projekBaru->kiri = projekBaru->kanan = NULL;
        return projekBaru;
    }
    if (strcmp(id, root->id) < 0)
        root->kiri = createProjek(root->kiri, id, nama);
    else if (strcmp(id, root->id) > 0)
        root->kanan = createProjek(root->kanan, id, nama);
    return root;
}

void addAnggota(ProjekNode* projek, char* id, char* nama) {
    AnggotaNode *newnode = (AnggotaNode*)malloc(sizeof(AnggotaNode));
    if(!newnode){
        printf("tidak bisa mengalokasikan memori!\n");
        exit(EXIT_FAILURE);
    }
    strcpy(newnode->id,id);
    strcpy(newnode->nama,nama);
    cekQueue(&newnode->list);
    newnode->list = NULL;
    newnode->next = projek->pekerja;
    projek->pekerja = newnode;
}

void addTugasAnggota(ProjekNode* projek, char* anggotaId, char* tugasId, char* tugas, char* status) {
    AnggotaNode* anggota = projek->pekerja;
    while (anggota && strcmp(anggota->id, anggotaId) != 0) {
        anggota = anggota->next;
    }
    if (anggota) {
        TugasAnggota* adaTugas = (TugasAnggota*)malloc(sizeof(TugasAnggota));
        strcpy(adaTugas->tugasId, tuggasId);
        strcpy(adaTugas->tugas, tugas);
        strcpy(adaTugas->status, status);
        adaTugas->next = anggota->list;
        anggota->list = adaTugas;
    }
    else {
        printf("Tidak ada pekerja.\n");
    }
}

void addTugas(Queue* queue, char* id, char* tugas, char* status) {
    TugasNode* newNode = (TugasNode*)malloc(sizeof(TugasNode));
    if (!newNode) {
        printf("tidak bisa mengalokasikan memori!\n");
        exit(EXIT_FAILURE);
    }
    strcpy(newNode->id, id);
    strcpy(newNode->tugas, tugas);
    strcpy(newNode->status, status);
    newNode->next = NULL;
    if (!queue->belakang)
        queue->depan = queue->belakang = newNode;
    else {
        queue->belakang->next = newNode;
        queue->belakang = newNode;
    }
}

void dequeue(Queue *queue) {
    if (queue->depan == NULL) {
        printf("Tidak ada tugas untuk dihapus, antrian kosong!\n");
        return;
    }
    TugasNode *temp = queue->depan;
    printf("Tugas dengan ID '%s' telah selesai dan dihapus dari antrian.\n", temp->id);
    queue->depan = queue->depan->next;
    if (queue->depan == NULL) {
        queue->belakang = NULL;
    }
    free(temp);
}

void showAnggota(AnggotaNode* head){
    if(!head) {
        printf("Tidak ada pekerja.\n\n");
        return;
    }
    else {
        AnggotaNode* tmp = head;
        while (tmp) {
            printf("  ID: %s, Nama: %s\n", tmp->id, tmp->nama);
            printf("    Tugas:\n");
            showTugasAnggota(tmp->list);
            tmp = tmp->next;
        }
    }
}

void showTugasAnggota(TugasAnggota* head) {
    TugasAnggota* tmp = head;
    while (tmp) {
        printf("      ID: %s, Tugas: %s, Status: %s\n", tmp->id, tmp->tugas, tmp->status);
        tmp = tmp->next;
    }
}

void showTugas(Queue *queue) {
    if(queue->depan == NULL) {
        printf("Tidak ada tugas, pekerja ngantin aja.\n");
    }
    else {
        TugasNode* data = queue ->depan;
        while (data != NULL) {
            printf("  ID: %s, Tugas: %s\n Status: %s\n", data->id, data->tugas, data->status);
            data = data->next;
        }
    }
}

/*
void dltTugas(Queue *queue){
    if(queue->depan == NULL){
        printf("tidak ada pekerjaan\n\n");
        return;
    }
    TugasNode *data = queue ->depan;
    printf("tugas %s telah selesai\n", data->tugas);
    queue->depan = queue->depan->next;
    if(queue->depan == NULL){
        queue->belakang = NULL;
    }
    free(data);
}
*/

void showProjek(ProjekNode* root) {
    if (root) {
        showProjek(root->kiri);
        printf("\nID Proyek: %s, Nama Proyek: %s\n", root->id, root->nama);
        printf("Pekerja:\n");
        showAnggota(root->pekerja);
        printf("Tugas:\n");
        showTugas(root->list);
        showProjek(root->kanan);
    }
}

int main(){
    ProjekNode* root = NULL;
    int pilih;
    char projekId[10], anggotaId[10], tugasId[10];
    char projekNama[100], anggotaNama[100], descTugas[5000], statTugas[300];
    ProjekNode* projekBtr = NULL;
    
    do {
        printf("menu\n");
        printf("1. tambahkan proyek\n");
        printf("2. tambahkan pekerja\n");
        printf("3. tambahkan jobdesk\n");
        printf("4. pekerjaan yang sudah selesai\n");
        printf("5. tampilkan secara lengkap\n");
        printf("masukan pilihan:");
        scanf("%d", &pilih);
        getchar();
        switch (pilih) {
            case 1:
                printf("Masukkan ID Proyek: ");
                fgets(projekId, sizeof(projekId), stdin);
                projekId[strcspn(projekId, "\n")] = 0;
                printf("Masukan Nama Proyek: ");
                fgets(projekNama, sizeof(projekNama), stdin);
                projekNama[strcspn(projekNama,"\n")] = 0;
                root = createProjek(root, projekId, projekNama);
                printf("Proyek '%s' berhasil dibuat!\n", projekNama);
                break;
            case 2:
                printf("Masukkan ID Proyek: ");
                fgets(projekId, sizeof(projekId), stdin);
                projekId[strcspn(projekId, "\n")] = 0;
                projekBtr = cariProjek(root, projekId);
                if (projekBtr) {
                    printf("Masukan ID Pekerja: ");
                    fgets(anggotaId, sizeof(anggotaId), stdin);
                    anggotaId[strcspn(anggotaId,"\n")] = 0;
                    printf("Masukan Nama Pekerja: ");
                    fgets(anggotaNama, sizeof(anggotaNama), stdin);
                    anggotaNama[strcspn(anggotaNama,"\n")] = 0;
                    addAnggota(projekBtr, anggotaId, anggotaNama);
                    printf("Pekerja '%s' berhasil ditambahkan ke proyek!\n");
                    }
                else {
                    printf("Proyek tidak ada!\n");
                }
                break;
            case 3:
                printf("Masukkan ID Proyek: ");
                fgets(projekId, sizeof(projekId), stdin);
                projekId[strcspn(projekId, "\n")] = 0;
                projekBtr = cariProjek(root, projekId);
                if (projekBtr) {
                    printf("Masukan ID Pekerja: ");
                    fgets(anggotaId, sizeof(anggotaId), stdin);
                    anggotaId[strcspn(anggotaId,"\n")] = 0;
                    printf("Masukan ID Jobdesk: ");
                    fgets(tugasId, sizeof(tugasId), stdin);
                    tugasId[strcspn(tugasId,"\n")] = 0;
                    printf("Masukan Jobdesk Pekerja: ");
                    fgets(descTugas, sizeof(descTugas), stdin);
                    descTugas[strcspn(descTugas,"\n")] = 0;
                    printf("Status Jobdesk (Belum/Proses): ");
                    fgets(statTugas, sizeof(statTugas), stdin);
                    statTugas[strcspn(statTugas,"\n")] = 0;
                    addTugasAnggota(projekBtr, anggotaId, tugasId, descTugas, statTugas);
                    printf("Jobdesk '%s' berhasil ditambahkan!\n", descTugas);
                }
                else {
                    printf("Proyek tidak ada!"\n);
                }
                break;

            case 4:
                printf("Masukkan ID proyek: ");
                scanf("%s", projekId);
                projekBtr = cariProjek(root, projekId);
                if (!projekBtr) {
                    printf("Proyek tidak ditemukan!\n");
                    break;
                }
                dequeue(&projekBtr->list);
                break;
            
            case 5:
                showProjek(root);
                break;
            default:
                printf("pilihan anda tidak valid");
                break;
        }
    }while(pilih != 5);
    return 0;
}

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

typedef struct TugasAnggota {
    char tugasId[10];
    char tugas[5000];
    char status[300];
    struct TugasAnggota* next;
}TugasAnggota;

typedef struct AnggotaNode {
    char id[10];
    char nama[100];
    TugasAnggota* list;
    struct AnggotaNode* next;
}AnggotaNode;

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
    //validasi id apakah ada yang sama
    if (cariProjek(root, id)) {
        printf("Proyek dengan ID '%s' sudah ada.\n", id);
        return root; 
    }
    if (!root) {
        ProjekNode* projekBaru = (ProjekNode*)malloc(sizeof(ProjekNode));
        strcpy(projekBaru->id, id);
        strcpy(projekBaru->nama, nama);
        projekBaru->pekerja = NULL;
        cekQueue(&projekBaru->list);
        projekBaru->kiri = projekBaru->kanan = NULL;
        return projekBaru;
    }
    if (strcmp(id, root->id) < 0){
        root->kiri = createProjek(root->kiri, id, nama);
    } else if (strcmp(id, root->id) > 0){
        root->kanan = createProjek(root->kanan, id, nama);
    }
    return root;
}

void addAnggota(ProjekNode* projek, char* id, char* nama) {
    //validasi untuk mengecek apa ada duplikasi ID
    AnggotaNode* current = projek->pekerja;
    while (current) {
        if (strcmp(current->id, id) == 0) {
            printf("Anggota dengan ID '%s' sudah ada di proyek ini.\n", id);
            return;
        }
        current = current->next;
    }
    AnggotaNode *newnode = (AnggotaNode*)malloc(sizeof(AnggotaNode));
    if(!newnode){
        printf("tidak bisa mengalokasikan memori!\n\n");
        exit(EXIT_FAILURE);
    }
    strcpy(newnode->id,id);
    strcpy(newnode->nama,nama);
    newnode->list = NULL;
    newnode->next = projek->pekerja;
    projek->pekerja = newnode;
    printf("Pekerja '%s' berhasil ditambahkan ke proyek.\n", nama);
}

void addTugasAnggota(ProjekNode* projek, char* anggotaId, char* tugasId, char* tugas, char* status) {
       // Mencari pekerja dengan ID yang sesuai
    AnggotaNode* anggota = projek->pekerja;
    while (anggota && strcmp(anggota->id, anggotaId) != 0) {
        anggota = anggota->next;
    }
    
    if (!anggota) {
        // Jika pekerja tidak ditemukan
        printf("Tidak ada pekerja dengan ID '%s' di proyek ini.\n", anggotaId);
        return; // Kembali tanpa menambahkan tugas
    }
    // Memeriksa apakah tugas sudah ada pada anggota ini
    TugasAnggota* currentTugas = anggota->list;
    while (currentTugas) {
        if (strcmp(currentTugas->tugasId, tugasId) == 0) {
            printf("Tugas dengan ID '%s' sudah ada untuk pekerja '%s'.\n", tugasId, anggota->nama);
            return; // Kembalikan tanpa menambahkan tugas yang sama
        }
        currentTugas = currentTugas->next;
    }
    // Jika tidak ditemukan tugas dengan ID yang sama, tambahkan tugas baru
    TugasAnggota* tugasBaru = (TugasAnggota*)malloc(sizeof(TugasAnggota));
    if (!tugasBaru) {
        printf("Gagal mengalokasikan memori untuk tugas baru.\n");
        return;
    }
    strcpy(tugasBaru->tugasId, tugasId);
    strcpy(tugasBaru->tugas, tugas);
    strcpy(tugasBaru->status, status);
    
    // Menambahkan tugas ke dalam daftar tugas pekerja
    tugasBaru->next = anggota->list;
    anggota->list = tugasBaru;

    printf("Jobdesk '%s' berhasil ditambahkan untuk pekerja '%s'.\n", tugas, anggota->nama);
}

void addTugas(Queue* queue, char* id, char* tugas, char* status) {
    TugasNode* newNode = (TugasNode*)malloc(sizeof(TugasNode));
    if (!newNode) {
        printf("tidak bisa mengalokasikan memori!\n\n");
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
        printf("      ID: %s, Tugas: %s, Status: %s\n", tmp->tugasId, tmp->tugas, tmp->status);
        tmp = tmp->next;
    }
}

void showTugas(Queue* queue) {
    if(queue->depan == NULL) {
        printf("tidak ada tugas.\n");
        return;
    }
    else {
        TugasNode* data = queue ->depan;
        while (data != NULL) {
            printf("  ID: %s, Tugas: %s\n Status: %s\n", data->id, data->tugas, data->status);
            data = data->next;
        }
    }
}

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

void showProjek(ProjekNode* root) {
    if (root) {
        showProjek(root->kiri);
        printf("\nID Proyek: %s, Nama Proyek: %s\n", root->id, root->nama);
        printf("Pekerja:\n");
        showAnggota(root->pekerja);
        printf("Tugas:\n");
        showTugas(&root->list);
        showProjek(root->kanan);
    }else 
        printf("proyek belum ditambahkan.\n");
        return;
}

int main(){
    ProjekNode* root = NULL;
    int pilih;
    char projekId[10], anggotaId[10], tugasId[10];
    char projekNama[100], anggotaNama[100], descTugas[5000], statTugas[300];
    ProjekNode* projekBtr = NULL;
    
    do {
        printf("\nmenu\n");
        printf("1. tambahkan proyek\n");
        printf("2. tambahkan pekerja\n");
        printf("3. tambahkan jobdesk\n");
        printf("4. pekerjaan yang sudah selesai\n");
        printf("5. tampilkan secara lengkap\n");
        printf("6. keluar program\n");
        printf("masukan pilihan:");
        scanf("%d", &pilih);
        getchar();
        switch (pilih) {
            case 1:
                printf("Masukkan ID Proyek: ");
                fgets(projekId, sizeof(projekId), stdin);
                projekId[strcspn(projekId, "\n")] = 0;
                if (strlen(projekId) == 0) {
                    printf("ID Proyek tidak boleh kosong.\n");
                    break;
                    }
                printf("Masukan Nama Proyek: ");
                fgets(projekNama, sizeof(projekNama), stdin);
                projekNama[strcspn(projekNama,"\n")] = 0;
                if (strlen(projekNama) == 0) {
                    printf("nama projek tidak boleh kosong.\n");
                    break;
                    }
                ProjekNode* existingProjek = cariProjek(root, projekId);
                if (!existingProjek) {
                    root = createProjek(root, projekId, projekNama);
                    printf("Proyek '%s' berhasil dibuat!\n", projekNama);
                    }
                else {
                    printf("Proyek dengan ID '%s' sudah ada.\n", projekId);
                    }
                break;
            case 2:
                printf("Masukkan ID Proyek: ");
                fgets(projekId, sizeof(projekId), stdin);
                projekId[strcspn(projekId, "\n")] = 0;
                if (strlen(projekId) == 0) {
                    printf("ID Proyek tidak boleh kosong.\n");
                    break;
                    }
                projekBtr = cariProjek(root, projekId);
                if (projekBtr) {
                    printf("Masukan ID Pekerja: ");
                    fgets(anggotaId, sizeof(anggotaId), stdin);
                    anggotaId[strcspn(anggotaId,"\n")] = 0;
                    if (strlen(anggotaId) == 0) {
                        printf("ID Pekerja tidak boleh kosong.\n");
                        break;
                        }
                    printf("Masukan Nama Pekerja: ");
                    fgets(anggotaNama, sizeof(anggotaNama), stdin);
                    anggotaNama[strcspn(anggotaNama,"\n")] = 0;
                    if (strlen(anggotaNama) == 0) {
                        printf("Nama Pekerja tidak boleh kosong.\n");
                        break;
                        }
                    addAnggota(projekBtr, anggotaId, anggotaNama);
                    }
                else {
                    printf("Proyek tidak ada!\n");
                }
                break;
            case 3:
                printf("Masukkan ID Proyek: ");
                fgets(projekId, sizeof(projekId), stdin);
                projekId[strcspn(projekId, "\n")] = 0;
                if (strlen(projekId) == 0) {
                    printf("ID Proyek tidak boleh kosong.\n");
                    break;
                    }
                projekBtr = cariProjek(root, projekId);
                if (projekBtr) {
                    printf("Masukan ID Pekerja: ");
                    fgets(anggotaId, sizeof(anggotaId), stdin);
                    anggotaId[strcspn(anggotaId,"\n")] = 0;
                    if (strlen(anggotaId) == 0) {
                        printf("ID Pekerja tidak boleh kosong.\n");
                        break;
                        }
                    printf("Masukan ID Jobdesk: ");
                    fgets(tugasId, sizeof(tugasId), stdin);
                    tugasId[strcspn(tugasId,"\n")] = 0;
                    if (strlen(tugasId) == 0) {
                        printf("ID Jobdesk tidak boleh kosong.\n");
                        break;
                        }
                    printf("Masukan Jobdesk Pekerja: ");
                    fgets(descTugas, sizeof(descTugas), stdin);
                    descTugas[strcspn(descTugas,"\n")] = 0;
                    if (strlen(descTugas) == 0) {
                        printf("Deskripsi Tugas tidak boleh kosong.\n");
                        break;
                        }
                    printf("Status Jobdesk (Belum/Proses): ");
                    fgets(statTugas, sizeof(statTugas), stdin);
                    statTugas[strcspn(statTugas,"\n")] = 0;
                    if (strlen(statTugas) == 0) {
                        printf("Status Tugas tidak boleh kosong.\n");
                        break;
                        }
                    addTugasAnggota(projekBtr, anggotaId, tugasId, descTugas, statTugas);
                }
                else {
                    printf("Proyek tidak ada!\n");
                }
                break;
            case 4:
                printf("Masukkan ID Proyek: ");
                fgets(projekId, sizeof(projekId), stdin);
                projekId[strcspn(projekId, "\n")] = 0;
                if (strlen(projekId) == 0) {
                    printf("ID Proyek tidak boleh kosong.\n");
                    break;
                    }
                projekBtr = cariProjek(root, projekId);

                if (projekBtr) {
                    printf("Masukkan ID Pekerja: ");
                    fgets(anggotaId, sizeof(anggotaId), stdin);
                    anggotaId[strcspn(anggotaId, "\n")] = 0;
                    if (strlen(anggotaId) == 0) {
                        printf("ID Pekerja tidak boleh kosong.\n");
                        break;
                        }
                    AnggotaNode* pekerja = projekBtr->pekerja;
                    while (pekerja && strcmp(pekerja->id, anggotaId) != 0) {
                        pekerja = pekerja->next;
                    }

                    if (pekerja) {
                        TugasAnggota* tugas = pekerja->list;
                        if (!tugas) {
                            printf("Pekerja ini tidak memiliki tugas.\n");
                        } else {
                            printf("Menyelesaikan semua tugas pekerja '%s':\n", pekerja->nama);
                            while (tugas) {
                                printf("  Menyelesaikan tugas '%s' (ID: %s)...\n", tugas->tugas, tugas->tugasId);
                                TugasAnggota* selesai = tugas;
                                tugas = tugas->next;
                                free(selesai);
                            }
                            pekerja->list = NULL;
                            printf("Semua tugas pekerja '%s' telah selesai.\n", pekerja->nama);
                        }
                    } else {
                        printf("Pekerja dengan ID '%s' tidak ditemukan di proyek ini.\n", anggotaId);
                    }
                } else {
                    printf("Proyek dengan ID '%s' tidak ditemukan.\n", projekId);
                }
                break;

            case 5:
                showProjek(root);
                break;
            case 6:
                printf("keluar dari perogram");
                break;
            default:
                printf("pilihan anda tidak valid");
                break;
        }
    }while(pilih != 6);
    return 0;
}

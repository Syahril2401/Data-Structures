#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h> 
#include <time.h>  // Untuk pembuatan ID unik


#define ALPHABET_SIZE 26
#define MAX_NAME_LENGTH 100
#define MAX_ACCOUNT_LENGTH 25
#define MAX_ID_LENGTH 20
#define MIN_SALDO 10000 // Minimal saldo 10.000

// Struktur untuk data nasabah
typedef struct dataNasabah {
    char id[MAX_ID_LENGTH];     // ID unik nasabah
    char nama[MAX_NAME_LENGTH];
    char norek[MAX_ACCOUNT_LENGTH];
    double saldo;
} dataNasabah;

// Node trie
typedef struct trieNode {
    struct trieNode* children[ALPHABET_SIZE];
    bool isEndOfWord;
    dataNasabah* data;
} trieNode;

// Fungsi untuk membuat node baru
trieNode* createNode() {
    trieNode* node = (trieNode*)malloc(sizeof(trieNode));
    if (node) {
        node->isEndOfWord = false;
        node->data = NULL;
        for (int i = 0; i < ALPHABET_SIZE; i++) {
            node->children[i] = NULL;
        }
    }
    return node;
}

// Untuk mengubah karakter ke index
int charToIndex(char c) {
    return tolower(c) - 'a'; 
}

// Fungsi untuk memeriksa apakah node memiliki child
bool anyChild(trieNode* node) {
    for (int i = 0; i < ALPHABET_SIZE; i++)
        if (node->children[i]) return true;
    return false;
}

// Fungsi untuk menghasilkan ID unik
void generateID(char* id) {
    static int counter = 1;
    time_t rawtime;
    struct tm *timeinfo;
    char buffer[15];
    
    time(&rawtime);
    timeinfo = localtime(&rawtime);
    
    // Format: NSB-YYYYMMDD-XXX (XXX adalah nomor urut)
    strftime(buffer, 15, "%Y%m%d", timeinfo);
    sprintf(id, "NSB-%s-%03d", buffer, counter++);
}

// Fungsi validasi nama (hanya huruf dan spasi)
bool isValidName(const char* name) {
    if (strlen(name) < 3 || strlen(name) >= MAX_NAME_LENGTH) {
        printf("Error: Panjang nama harus antara 3 dan %d karakter.\n", MAX_NAME_LENGTH - 1);
        return false;
    }
    
    for (int i = 0; name[i]; i++) {
        if (!isalpha(name[i]) && name[i] != ' ') {
            printf("Error: Nama hanya boleh berisi huruf dan spasi.\n");
            return false;
        }
    }
    return true;
}

// Fungsi validasi nomor rekening (hanya angka dan panjang 10-16 digit)
bool isValidAccountNumber(const char* accNum) {
    if (strlen(accNum) < 10 || strlen(accNum) > 16) {
        printf("Error: Nomor rekening harus antara 10 dan 16 digit.\n");
        return false;
    }
    
    for (int i = 0; accNum[i]; i++) {
        if (!isdigit(accNum[i])) {
            printf("Error: Nomor rekening hanya boleh berisi angka.\n");
            return false;
        }
    }
    return true;
}

// Fungsi validasi saldo (harus lebih dari nilai minimum)
bool isValidBalance(double saldo) {
    if (saldo < MIN_SALDO) {
        printf("Error: Saldo awal minimal Rp %.2f\n", (float)MIN_SALDO);
        return false;
    }
    return true;
}

// Fungsi untuk memeriksa apakah nomor rekening sudah ada
bool isAccountNumberExists(trieNode* root, const char* norek) {
    bool found = false;
    
    // Fungsi helper untuk traversal
    void checkNorek(trieNode* node) {
        if (node == NULL) return;

        if (node->isEndOfWord && node->data != NULL) {
            if (strcmp(node->data->norek, norek) == 0) {
                found = true;
                return;
            }
        }

        for (int i = 0; i < ALPHABET_SIZE && !found; i++) {
            if (node->children[i] != NULL) {
                checkNorek(node->children[i]);
            }
        }
    }

    checkNorek(root);
    return found;
}

// Fungsi untuk mencari nasabah berdasarkan ID
dataNasabah* findNasabahByID(trieNode* root, const char* id) {
    dataNasabah* result = NULL;
    
    // Fungsi helper untuk traversal
    void traverseForID(trieNode* node) {
        if (node == NULL || result != NULL) return;

        if (node->isEndOfWord && node->data != NULL) {
            if (strcmp(node->data->id, id) == 0) {
                result = node->data;
                return;
            }
        }

        for (int i = 0; i < ALPHABET_SIZE && result == NULL; i++) {
            if (node->children[i] != NULL) {
                traverseForID(node->children[i]);
            }
        }
    }

    traverseForID(root);
    return result;
}

// Tambah data nasabah
void insert(trieNode* root, char* key, dataNasabah* data) {
    trieNode* curr = root;
    for (int i = 0; i < strlen(key); i++) {
        int index = charToIndex(key[i]);
        if (index < 0 || index >= ALPHABET_SIZE) {
            continue; // Lewati karakter non-alfabet
        }
        if (curr->children[index] == NULL)
            curr->children[index] = createNode();
        curr = curr->children[index];
    }
    curr->isEndOfWord = true;
    curr->data = data;
}

// Cari node berdasarkan nama
trieNode* search(trieNode* root, char* key) {
    trieNode* curr = root;
    for (int i = 0; i < strlen(key); i++) {
        int index = charToIndex(key[i]);
        if (index < 0 || index >= ALPHABET_SIZE) {
            continue; // Lewati karakter non-alfabet
        }
        if (curr->children[index] == NULL)
            return NULL;
        curr = curr->children[index];
    }
    if (curr != NULL && curr->isEndOfWord)
        return curr;
    return NULL;
}

// Hapus data - fungsi helper
bool deleteHelper(trieNode* curr, char* key, int depth, int len) {
    if (!curr) return false;

    if (depth == len) {
        if (curr->isEndOfWord) {
            curr->isEndOfWord = false;
            free(curr->data);
            curr->data = NULL;

            for (int i = 0; i < ALPHABET_SIZE; i++)
                if (curr->children[i]) return false;

            return true;
        }
    } else {
        int index = charToIndex(key[depth]);
        if (index < 0 || index >= ALPHABET_SIZE) {
            return false;
        }
        if (deleteHelper(curr->children[index], key, depth + 1, len)) {
            free(curr->children[index]);
            curr->children[index] = NULL;

            return !curr->isEndOfWord && 
                   !curr->data &&
                   !anyChild(curr);
        }
    }
    return false;
}

// Hapus data - fungsi utama berdasarkan nama
void deleteData(trieNode* root, char* key) {
    trieNode* node = search(root, key);
    if (node) {
        printf("Data ditemukan:\n");
        printf("ID: %s\n", node->data->id);
        printf("Nama: %s\n", node->data->nama);
        printf("No Rekening: %s\n", node->data->norek);
        printf("Saldo: %.2lf\n", node->data->saldo);
        
        char konfirmasi;
        printf("Apakah Anda yakin ingin menghapus data ini? (y/n): ");
        scanf(" %c", &konfirmasi);
        getchar(); // Membersihkan buffer
        
        if (tolower(konfirmasi) == 'y') {
            deleteHelper(root, key, 0, strlen(key));
            printf("Data berhasil dihapus.\n");
        } else {
            printf("Penghapusan dibatalkan.\n");
        }
    } else {
        printf("Data tidak ditemukan.\n");
    }
}

// Hapus data berdasarkan ID
void deleteDataByID(trieNode* root, const char* id) {
    bool found = false;
    char nama[MAX_NAME_LENGTH];
    
    // Fungsi untuk menemukan nama dari ID
    void findNameByID(trieNode* node) {
        if (node == NULL || found) return;

        if (node->isEndOfWord && node->data != NULL) {
            if (strcmp(node->data->id, id) == 0) {
                strcpy(nama, node->data->nama);
                found = true;
                return;
            }
        }

        for (int i = 0; i < ALPHABET_SIZE && !found; i++) {
            if (node->children[i] != NULL) {
                findNameByID(node->children[i]);
            }
        }
    }

    findNameByID(root);
    
    if (found) {
        deleteData(root, nama);
    } else {
        printf("ID nasabah tidak ditemukan.\n");
    }
}

// Update data berdasarkan nama
void updateData(trieNode* root, char* nama) {
    trieNode* node = search(root, nama);
    if (node) {
        printf("Data ditemukan.\n");
        printf("ID: %s\n", node->data->id);
        printf("Nama: %s\n", node->data->nama);
        printf("No Rekening: %s\n", node->data->norek);
        printf("Saldo saat ini: %.2lf\n", node->data->saldo);
    
        double newSaldo;
        printf("Input saldo baru: ");
        
        // Validasi input saldo
        while (1) {
            if (scanf("%lf", &newSaldo) != 1) {
                printf("Input tidak valid. Masukkan angka: ");
                while (getchar() != '\n'); 
                continue;
            }
            
            if (newSaldo < 0) {
                printf("Saldo tidak boleh negatif. Masukkan saldo yang valid: ");
                continue;
            }
            
            break;
        }
        
        getchar(); // Membersihkan buffer
        node->data->saldo = newSaldo;
        printf("Data berhasil diperbarui.\n");
    } else {
        printf("Data tidak ditemukan.\n");
    }
}

// Update data berdasarkan ID
void updateDataByID(trieNode* root, const char* id) {
    dataNasabah* nasabah = findNasabahByID(root, id);
    
    if (nasabah) {
        printf("Data ditemukan.\n");
        printf("ID: %s\n", nasabah->id);
        printf("Nama: %s\n", nasabah->nama);
        printf("No Rekening: %s\n", nasabah->norek);
        printf("Saldo saat ini: %.2lf\n", nasabah->saldo);
    
        double newSaldo;
        printf("Input saldo baru: ");
        
        // Validasi input saldo
        while (1) {
            if (scanf("%lf", &newSaldo) != 1) {
                printf("Input tidak valid. Masukkan angka: ");
                while (getchar() != '\n'); // Membersihkan buffer
                continue;
            }
            
            if (newSaldo < 0) {
                printf("Saldo tidak boleh negatif. Masukkan saldo yang valid: ");
                continue;
            }
            
            break;
        }
        
        getchar(); // Membersihkan buffer
        nasabah->saldo = newSaldo;
        printf("Data berhasil diperbarui.\n");
    } else {
        printf("ID nasabah tidak ditemukan.\n");
    }
}

// Tampilkan data berdasarkan nama
void tampilkanData(trieNode* root, char* nama) {
    trieNode* node = search(root, nama);
    if (node) {
        printf("=== Data Nasabah ===\n");
        printf("ID: %s\n", node->data->id);
        printf("Nama: %s\n", node->data->nama);
        printf("No Rekening: %s\n", node->data->norek);
        printf("Saldo: %.2f\n", node->data->saldo);
    } else {
        printf("Data tidak ditemukan\n");
    }
}

// Tampilkan data berdasarkan ID
void tampilkanDataByID(trieNode* root, const char* id) {
    dataNasabah* nasabah = findNasabahByID(root, id);
    
    if (nasabah) {
        printf("=== Data Nasabah ===\n");
        printf("ID: %s\n", nasabah->id);
        printf("Nama: %s\n", nasabah->nama);
        printf("No Rekening: %s\n", nasabah->norek);
        printf("Saldo: %.2f\n", nasabah->saldo);
    } else {
        printf("ID nasabah tidak ditemukan\n");
    }
}

// Struktur untuk menyimpan data nasabah sementara untuk tabel
typedef struct {
    int nomor;
    char id[MAX_ID_LENGTH];
    char nama[MAX_NAME_LENGTH];
    char norek[MAX_ACCOUNT_LENGTH];
    double saldo;
} TableRow;

// Fungsi untuk menampilkan garis tabel
void printTableLine(int idWidth, int nameWidth, int accountWidth) {
    printf("+----+");
    for (int i = 0; i < idWidth + 2; i++) printf("-");
    printf("+");
    for (int i = 0; i < nameWidth + 2; i++) printf("-");
    printf("+");
    for (int i = 0; i < accountWidth + 2; i++) printf("-");
    printf("+----------------+\n");
}

// Fungsi untuk mengumpulkan data untuk tabel
void collectTableData(trieNode* node, TableRow** rows, int* count, int* capacity) {
    if (node == NULL) return;
    
    // Jika node ini adalah akhir kata (menyimpan data nasabah)
    if (node->isEndOfWord && node->data != NULL) {
        // Realokasi jika perlu
        if (*count >= *capacity) {
            *capacity *= 2;
            *rows = (TableRow*)realloc(*rows, (*capacity) * sizeof(TableRow));
            if (!(*rows)) {
                printf("Error realokasi memori untuk tabel\n");
                return;
            }
        }
        
        // Tambahkan data ke array
        (*rows)[*count].nomor = *count + 1;
        strncpy((*rows)[*count].id, node->data->id, MAX_ID_LENGTH - 1);
        (*rows)[*count].id[MAX_ID_LENGTH - 1] = '\0';
        strncpy((*rows)[*count].nama, node->data->nama, MAX_NAME_LENGTH - 1);
        (*rows)[*count].nama[MAX_NAME_LENGTH - 1] = '\0';
        strncpy((*rows)[*count].norek, node->data->norek, MAX_ACCOUNT_LENGTH - 1);
        (*rows)[*count].norek[MAX_ACCOUNT_LENGTH - 1] = '\0';
        (*rows)[*count].saldo = node->data->saldo;
        
        (*count)++;
    }
    
    // Traversal ke semua children
    for (int i = 0; i < ALPHABET_SIZE; i++) {
        if (node->children[i] != NULL) {
            collectTableData(node->children[i], rows, count, capacity);
        }
    }
}

// Fungsi untuk menampilkan semua data nasabah dalam bentuk tabel
void displayAllData(trieNode* root) {
    int count = 0;
    int capacity = 10; // Kapasitas awal
    TableRow* rows = (TableRow*)malloc(capacity * sizeof(TableRow));
    
    if (!rows) {
        printf("Error alokasi memori untuk tabel\n");
        return;
    }
    
    // Kumpulkan data nasabah
    collectTableData(root, &rows, &count, &capacity);
    
    if (count == 0) {
        printf("\n=== Daftar Semua Nasabah ===\n");
        printf("Belum ada data nasabah.\n");
        free(rows);
        return;
    }
    
    // Tentukan lebar kolom yang sesuai berdasarkan data
    int idWidth = 14; // Default width for ID (NSB-YYYYMMDD-XXX)
    int nameWidth = 20; // Default width
    int accountWidth = 16; // Default width
    
    for (int i = 0; i < count; i++) {
        int idLen = strlen(rows[i].id);
        int nameLen = strlen(rows[i].nama);
        int accLen = strlen(rows[i].norek);
        if (idLen > idWidth) idWidth = idLen;
        if (nameLen > nameWidth) nameWidth = nameLen;
        if (accLen > accountWidth) accountWidth = accLen;
    }
    
    // Header tabel
    printf("\n=== Daftar Semua Nasabah ===\n");
    printTableLine(idWidth, nameWidth, accountWidth);
    printf("| No |");
    printf(" %-*s |", idWidth, "ID");
    printf(" %-*s |", nameWidth, "Nama");
    printf(" %-*s |", accountWidth, "No Rekening");
    printf(" %-14s |\n", "Saldo");
    printTableLine(idWidth, nameWidth, accountWidth);
    
    // Isi tabel
    for (int i = 0; i < count; i++) {
        printf("| %-2d |", rows[i].nomor);
        printf(" %-*s |", idWidth, rows[i].id);
        printf(" %-*s |", nameWidth, rows[i].nama);
        printf(" %-*s |", accountWidth, rows[i].norek);
        printf(" Rp.%-11.2f |\n", rows[i].saldo);
    }
    
    // Footer tabel
    printTableLine(idWidth, nameWidth, accountWidth);
    printf("Total nasabah: %d\n", count);
    
    // Bersihkan memori
    free(rows);
}

// Fungsi untuk membersihkan seluruh trie
void freeTrieNode(trieNode* node) {
    if (node == NULL) return;
    
    for (int i = 0; i < ALPHABET_SIZE; i++) {
        if (node->children[i] != NULL) {
            freeTrieNode(node->children[i]);
        }
    }
    
    if (node->data != NULL) {
        free(node->data);
    }
    free(node);
}

// Fungsi untuk membaca input string dengan aman
void safeInputString(char *buffer, int maxLength, const char *prompt) {
    printf("%s", prompt);
    if (fgets(buffer, maxLength, stdin) != NULL) {
        // Menghapus karakter newline jika ada
        buffer[strcspn(buffer, "\n")] = 0;
    }
}

// Fungsi untuk membaca input angka dengan aman
double safeInputDouble(const char *prompt) {
    char buffer[64];
    double value;
    
    while (1) {
        printf("%s", prompt);
        if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
            printf("Error membaca input. Coba lagi.\n");
            continue;
        }
        
        // Menghapus karakter newline
        buffer[strcspn(buffer, "\n")] = 0;
        
        // Mencoba konversi ke double
        char *endptr;
        value = strtod(buffer, &endptr);
        
        // Mengecek apakah seluruh string berhasil dikonversi
        if (*endptr == '\0' && value >= 0) {
            return value;
        }
        
        printf("Input tidak valid. Masukkan angka positif.\n");
    }
}

int main() {
    trieNode* root = createNode();
    int choice;
    char nama[MAX_NAME_LENGTH];
    char id[MAX_ID_LENGTH];
    char input[10]; // Buffer untuk input pilihan menu

    // Print banner saat awal program
    printf("\n");
    printf("==================================================\n");
    printf("||                                              ||\n");
    printf("||          SISTEM MANAJEMEN NASABAH BANK       ||\n");
    printf("||                                              ||\n");
    printf("==================================================\n");

    while (1) {
        printf("\n===== Menu CRUD Nasabah Bank =====\n");
        printf("1. Tambah Data Nasabah\n");
        printf("2. Cari Data Nasabah\n");
        printf("    a. Berdasarkan Nama\n");
        printf("    b. Berdasarkan ID\n");
        printf("3. Update Data Nasabah\n");
        printf("    a. Berdasarkan Nama\n");
        printf("    b. Berdasarkan ID\n");
        printf("4. Hapus Data Nasabah\n");
        printf("    a. Berdasarkan Nama\n");
        printf("    b. Berdasarkan ID\n");
        printf("5. Lihat Semua Data Nasabah\n");
        printf("6. Keluar\n");
        printf("Pilih: ");
        
        // Validasi input menu
        if (fgets(input, sizeof(input), stdin) == NULL) {
            printf("Error membaca input.\n");
            continue;
        }
        
        // Bersihkan input dari newline
        input[strcspn(input, "\n")] = 0;
        
        // Penanganan submenu
        if (strlen(input) == 2 && (input[0] >= '2' && input[0] <= '4') && 
            (input[1] == 'a' || input[1] == 'b')) {
            choice = (input[0] - '0') * 10 + (input[1] == 'a' ? 1 : 2);
        } else {
            // Konversi input ke integer
            if (sscanf(input, "%d", &choice) != 1) {
                printf("Input tidak valid. Masukkan angka 1-6 atau opsi submenu (contoh: 2a).\n");
                continue;
            }
        }

        switch (choice) {
            case 1: {
                dataNasabah* nasabah = (dataNasabah*)malloc(sizeof(dataNasabah));
                if (!nasabah) {
                    printf("Error alokasi memori\n");
                    break;
                }
                
                // Generate ID otomatis
                generateID(nasabah->id);
                printf("ID Nasabah dibuat: %s\n", nasabah->id);
                
                // Input dan validasi nama
                while (1) {
                    safeInputString(nasabah->nama, MAX_NAME_LENGTH, "Masukkan Nama: ");
                    if (isValidName(nasabah->nama)) break;
                }
                
                // Input dan validasi nomor rekening
                while (1) {
                    safeInputString(nasabah->norek, MAX_ACCOUNT_LENGTH, "Masukkan No Rekening: ");
                    if (!isValidAccountNumber(nasabah->norek)) {
                        continue;
                    }
                    
                    if (isAccountNumberExists(root, nasabah->norek)) {
                        printf("Error: Nomor rekening sudah terdaftar. Gunakan nomor lain.\n");
                        continue;
                    }
                    
                    break;
                }
                
                // Input dan validasi saldo
                while (1) {
                    nasabah->saldo = safeInputDouble("Masukkan Saldo: ");
                    if (isValidBalance(nasabah->saldo)) break;
                }

                insert(root, nasabah->nama, nasabah);
                printf("Data berhasil ditambahkan.\n");
                printf("ID Nasabah: %s\n", nasabah->id);
                break;
            }
            case 21: // Cari berdasarkan nama
                safeInputString(nama, MAX_NAME_LENGTH, "Masukkan nama untuk dicari: ");
                tampilkanData(root, nama);
                break;
            case 22: // Cari berdasarkan ID
                safeInputString(id, MAX_ID_LENGTH, "Masukkan ID untuk dicari: ");
                tampilkanDataByID(root, id);
                break;
            case 31: // Update berdasarkan nama
                safeInputString(nama, MAX_NAME_LENGTH, "Masukkan nama untuk diperbarui: ");
                updateData(root, nama);
                break;
            case 32: // Update berdasarkan ID
                safeInputString(id, MAX_ID_LENGTH, "Masukkan ID untuk diperbarui: ");
                updateDataByID(root, id);
                break; 
            case 41: // Hapus berdasarkan nama
                safeInputString(nama, MAX_NAME_LENGTH, "Masukkan nama untuk dihapus: ");
                deleteData(root, nama);
                break;
            case 42: // Hapus berdasarkan ID
                safeInputString(id, MAX_ID_LENGTH, "Masukkan ID untuk dihapus: ");
                deleteDataByID(root, id);
                break;
            case 5:
                displayAllData(root);
                break;
            case 6:
                // Membersihkan memori sebelum keluar
                freeTrieNode(root);
                printf("Terima kasih telah menggunakan sistem kami.\n");
                exit(0);
            default:
                printf("Pilihan tidak sesuai. Masukkan angka 1-6 atau opsi submenu (contoh: 2a).\n");
        }
    }

    return 0;
}
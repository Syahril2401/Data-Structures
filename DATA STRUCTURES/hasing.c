#include <stdio.h>
#define TABLE_SIZE 10

int hash_table[TABLE_SIZE];

void init_table() {
    for(int i= 0; i < TABLE_SIZE; i++) {
        hash_table[i] = -1;
    }
}

int hash_fuction(int key) {
    return key % TABLE_SIZE;
}

void insert(int key) {
    int index =  hash_fuction(key);
    int start_index = index;

    while (hash_table[index] != -1) {
        if (index == start_index) { // Jika sudah penuh
            printf("Hash table penuh! Tidak dapat menyisipkan %d\n", key);
            return;
        }
    }
    hash_table[index] = key;
    printf("%d dimasukkan ke indeks %d\n", key, index);
    }
}

int search(int key) {
    int index = hash_fuction(key);
    int start_index = index;

    while (hash_table[index] != -1) {
        if (hash_table[index] == key) {
            return index;
        }
        index = (index + 1) % TABLE_SIZE;
        if (index == start_index) { // Jika kembali ke titik awal
            break;
        }
    }
    return -1;
}

void pop(int key) {
    int index = search(key);
    if (index ! = -1){
        hash_table[index] = -1;
        printf("Elemen %d dihapus dari indeks %d\n", key, index);
    } else {
        printf("Elemen %d tidak ditemukan!\n", key);
    }
}

int pick(int key) {

}

void display() {

}

int main() {

}
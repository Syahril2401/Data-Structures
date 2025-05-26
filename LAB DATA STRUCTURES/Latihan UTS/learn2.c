// FIFO

#include <stdio.h>
#include <stdlib.h>

// Struktur node untuk DLL
struct Node {
    int data;
    struct Node* next;
    struct Node* prev;
};

// Head dan tail dari queue
struct Node* head = NULL;
struct Node* tail = NULL;

// Fungsi untuk enqueue (tambahkan ke akhir)
void enqueue(int data) {
    struct Node* newNode = (struct Node*)malloc(sizeof(struct Node));
    if (!newNode) {
        printf("Heap overflow\n");
        return;
    }
    newNode->data = data;
    newNode->next = NULL;
    
    if (head == NULL) {  // Jika queue kosong
        newNode->prev = NULL;
        head = tail = newNode;
    } else {
        newNode->prev = tail;
        tail->next = newNode;
        tail = newNode;
    }
    printf("Inserted %d into queue\n", data);
}

// Fungsi untuk dequeue (hapus dari depan)
void dequeue() {
    if (head == NULL) {
        printf("Queue is empty, cannot delete node\n");
        return;
    }
    struct Node* temp = head;
    printf("Deleted %d from queue\n", temp->data);
    head = head->next;

    if (head) {
        head->prev = NULL;
    } else {
        tail = NULL;  // Jika queue kosong
    }
    free(temp);
}

// Fungsi untuk menampilkan queue
void display() {
    struct Node* current = head;
    printf("Queue: ");
    while (current) {
        printf("%d <-> ", current->data);
        current = current->next;
    }
    printf("NULL\n");
}

// Main program untuk simulasi sesuai soal
int main() {
    enqueue(40);
    enqueue(50);
    enqueue(60);
    display();
    dequeue();
    dequeue();
    display();
    return 0;
}

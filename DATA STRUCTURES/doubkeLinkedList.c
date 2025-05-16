#include <stdio.h>
#include <stdlib.h>

#define MAX_NODES 10
//Muhammad Syahril Alauddin -2802404551

// struktur node Circular Doubly Linked List
typedef struct Node {
    int data;
    struct Node* next;
    struct Node* prev;
} Node;

// struktur Circular Doubly Linked List
typedef struct {
    Node* head;
    Node* current;
    int count;
} CircularDoublyLinkedList;

// buat linked list barus kosong
void createList(CircularDoublyLinkedList* list) {
    list->head = list->current = NULL;
    list->count = 0;
}

// cek linked list kosong
int isEmpty(CircularDoublyLinkedList* list) {
    return list->count == 0;
}

// cek linked list penuh
int isFull(CircularDoublyLinkedList* list) {
    return list->count == MAX_NODES;
}

// tambah ndoe
void insert(CircularDoublyLinkedList* list, int value) {
    if (isFull(list)) return;
    Node* newNode = (Node*)malloc(sizeof(Node));
    newNode->data = value;
    if (isEmpty(list)) {
        newNode->next = newNode->prev = newNode;
        list->head = list->current = newNode;
    } else {
        newNode->next = list->current->next;
        newNode->prev = list->current;
        list->current->next->prev = newNode;
        list->current->next = newNode;
    }
    list->current = newNode;
    list->count++;
}

// hapus node
void deleteNode(CircularDoublyLinkedList* list) {
    if (isEmpty(list)) return;
    Node* temp = list->current;
    if (list->count == 1) {
        list->head = list->current = NULL;
    } else {
        temp->prev->next = temp->next;
        temp->next->prev = temp->prev;
        if (list->current == list->head)
            list->head = temp->next;
        list->current = temp->next;
    }
    free(temp);
    list->count--;
}

// atur current ke head
void findFirst(CircularDoublyLinkedList* list) {
    if (!isEmpty(list)) list->current = list->head;
}

// atur current ke tail
void findLast(CircularDoublyLinkedList* list) {
    if (!isEmpty(list)) list->current = list->head->prev;
}

// menampilkan semua elemen dalam linked list
void display(CircularDoublyLinkedList* list) {
    if (isEmpty(list)) { printf("List kosong!\n"); return; }
    Node* temp = list->head;
    do {
        printf("%d ", temp->data);
        temp = temp->next;
    } while (temp != list->head);
    printf("\n");
}

int main() {
    CircularDoublyLinkedList list;
    createList(&list);
    insert(&list, 10);
    insert(&list, 20);
    insert(&list, 30);
    display(&list);
    findFirst(&list);
    deleteNode(&list);
    display(&list);
    return 0;
}


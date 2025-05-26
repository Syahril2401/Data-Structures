// LIFO

#include <stdio.h>
#include <stdlib.h>

// Struktur node
struct Node {
    int data;
    struct Node* next;
};

// Stack (LIFO)
struct Node* top = NULL;

// Fungsi untuk push data ke stack
void push(int data) {
    struct Node* newNode = (struct Node*)malloc(sizeof(struct Node));
    if (!newNode) {
        printf("Heap overflow\n");
        return;
    }
    newNode->data = data;
    newNode->next = top;
    top = newNode;
    printf("Inserted %d into stack\n", data);
}

// Fungsi untuk pop data dari stack
void pop() {
    if (top == NULL) {
        printf("Stack is empty, cannot delete node\n");
        return;
    }
    struct Node* temp = top;
    printf("Deleted %d from stack\n", temp->data);
    top = top->next;
    free(temp);
}

// Fungsi untuk menampilkan stack
void display() {
    struct Node* current = top;
    printf("Stack: ");
    while (current) {
        printf("%d -> ", current->data);
        current = current->next;
    }
    printf("NULL\n");
}

// Main program untuk simulasi sesuai soal
int main() {
    push(40);
    push(50);
    push(60);
    display();
    pop();
    pop();
    display();
    return 0;
}

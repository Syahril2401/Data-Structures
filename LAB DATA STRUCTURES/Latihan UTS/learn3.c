#include <stdio.h>
#include <stdlib.h>

struct Node {
    int data;
    struct Node* next;
};

struct Node* head = NULL;

void insert(int data) {
    struct Node* newNode = (struct Node*)malloc(sizeof(struct Node));
    newNode->data = data;
    if (head == NULL) {
        head = newNode;
        head->next = head;
    } else {
        struct Node* temp = head;
        while (temp->next != head) temp = temp->next;
        temp->next = newNode;
        newNode->next = head;
    }
}

void deleteValue(int val) {
    if (head == NULL) return;
    struct Node *curr = head, *prev = NULL;

    do {
        if (curr->data == val) {
            if (curr == head) {
                struct Node* temp = head;
                while (temp->next != head) temp = temp->next;
                head = head->next;
                temp->next = head;
                free(curr);
                return;
            } else {
                prev->next = curr->next;
                free(curr);
                return;
            }
        }
        prev = curr;
        curr = curr->next;
    } while (curr != head);
}

void printTwice() {
    if (head == NULL) return;
    struct Node* temp = head;
    int count = 0;
    do {
        printf("%d ", temp->data);
        temp = temp->next;
        if (temp == head) count++;
    } while (count < 2);
    printf("\n");
}

int main() {
    insert(10);
    insert(20);
    insert(30);
    deleteValue(20);
    printTwice();
    return 0;
}



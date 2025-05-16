#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_NAME 100

// Node structure
typedef struct Node {
    char name[MAX_NAME];
    char membership[MAX_NAME];
    int priority;
    struct Node* next;
} Node;

Node* front = NULL;

int getPriority(char* membership) {
    if (strcmp(membership, "VVIP") == 0) return 1;
    if (strcmp(membership, "VIP") == 0) return 2;
    if (strcmp(membership, "Member") == 0) return 3;
    return 4; // Guest
}

void displayQueue() {
    printf("Waiting Line:\n");
    if (front == NULL) {
        printf("Queue is empty\n");
        return;
    }
    Node* temp = front;
    int idx = 1;
    while (temp != NULL) {
        printf("%d. %s\n", idx++, temp->name);
        temp = temp->next;
    }
    printf("\n");
}

void addCustomer(char* input) {
    char membership[MAX_NAME], name[MAX_NAME];
    sscanf(input, "%s %[^\n]", membership, name); 

    Node* newNode = (Node*)malloc(sizeof(Node));
    strcpy(newNode->name, name);
    strcpy(newNode->membership, membership);
    newNode->priority = getPriority(membership);
    newNode->next = NULL;

    if (!front || newNode->priority < front->priority) {
        newNode->next = front;
        front = newNode;
    } else {
        Node* temp = front;
        while (temp->next && temp->next->priority <= newNode->priority) {
            if (temp->next->priority == newNode->priority) break;
            temp = temp->next;
        }
        while (temp->next && temp->next->priority == newNode->priority)
            temp = temp->next;

        newNode->next = temp->next;
        temp->next = newNode;
    }
}

void serveOne() {
    if (!front) {
        printf("Queue is empty\n");
        return;
    }
    if (strcmp(front->membership, "VVIP") == 0)
        printf("Attention! %s is being served at VVIP table\n", front->name);
    else if (strcmp(front->membership, "VIP") == 0)
        printf("Attention! %s is being served at the VIP table\n", front->name);
    else if (strcmp(front->membership, "Member") == 0)
        printf("Attention! Member %s is being served at regular table\n", front->name);
    else
        printf("Attention! Guest %s is being served at a regular table\n", front->name);

    Node* temp = front;
    front = front->next;
    free(temp);
    printf("\n");
}

void serveAll() {
    while (front) {
        serveOne();
    }
}

void dismissQueue() {
    while (front) {
        Node* temp = front;
        front = front->next;
        free(temp);
    }
    printf("End of the day.\n");
}

int main() {
    int menu;
    char input[MAX_NAME];

    do {
        printf("==============================\n");
        printf("SUNIB Restaurant Reservation\n");
        printf("==============================\n");
        displayQueue();
        printf("1. Add Customer to Queue\n");
        printf("2. Serve One\n");
        printf("3. Serve All\n");
        printf("4. Dismiss Queue\n");
        printf("0. Exit\n");
        printf("Input Menu Number: ");
        scanf("%d", &menu);
        getchar(); // consume newline

        switch (menu) {
            case 1:
                printf("Input: ");
                fgets(input, sizeof(input), stdin);
                input[strcspn(input, "\n")] = 0; // Remove trailing newline
                addCustomer(input);
                break;
            case 2:
                serveOne();
                break;
            case 3:
                serveAll();
                break;
            case 4:
                dismissQueue();
                break;
            case 0:
            printf("Thanks dawg!!!\n");
                break;
            default:
                printf("Invalid menu number.\n");
        }
    } while (menu != 0);

    return 0;
}

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Patient {
    char name[26], description[101], code[7];
    int age, priority;
    struct Patient *next;
} Patient;

Patient *head = NULL;

int getPriority(char *code) {
    return (!strcmp(code, "Red")) ? 3 : (!strcmp(code, "Yellow")) ? 2 : (!strcmp(code, "Green")) ? 1 : 0;
}

void insertPatient(char *name, int age, char *description, char *code) {
    Patient *newPatient = (Patient*)malloc(sizeof(Patient)), **curr = &head;
    strcpy(newPatient->name, name);
    strcpy(newPatient->description, description);
    strcpy(newPatient->code, code);
    newPatient->age = age;
    newPatient->priority = getPriority(code);
    newPatient->next = NULL;

    while (*curr && (*curr)->priority >= newPatient->priority) 
        curr = &(*curr)->next;

    newPatient->next = *curr;
    *curr = newPatient;

    printf("Pasien telah ditambahkan!\n");
}

void viewQueue() {
    if (!head) {
        printf("Masih egk ada antrean!\n");
        return;
    }

    printf("Daftar Pasien:\n");
    for (Patient *temp = head; temp; temp = temp->next)
        printf("Nama: %s, Umur: %d, Deskripsi: %s, Kode: %s\n", temp->name, temp->age, temp->description, temp->code);
}

void nextQueue() {
    if (!head) {
        printf("Masih egk ada antrean!\n");
        return;
    }
    
    Patient *temp = head;
    head = head->next;
    
    printf("Melayani pasien: %s\n", temp->name);
    free(temp);
}

int main() {
    int choice;

    do {
        printf("Bluejack Hospital\n=================\n");
        printf("1. Insert\n2. View\n3. Next Queue\n4. Exit\n>> ");
        
        scanf("%d", &choice);
        getchar();  // Mencegah bug dari scanf

        if (choice == 1) {
            char name[26], description[101], code[7]; 
            int age;

            do { 
                printf("Enter Name (4-25 chars): "); 
                fgets(name, 26, stdin); 
                name[strcspn(name, "\n")] = 0; 
            } while (strlen(name) < 4 || strlen(name) > 25);

            do { 
                printf("Enter Age (>=0): "); 
                scanf("%d", &age); 
                getchar(); 
            } while (age < 0);

            do { 
                printf("Enter Description (>=6 chars): "); 
                fgets(description, 101, stdin); 
                description[strcspn(description, "\n")] = 0; 
            } while (strlen(description) < 6);

            do { 
                printf("Enter Code (Red/Yellow/Green): "); 
                fgets(code, 7, stdin); 
                code[strcspn(code, "\n")] = 0; 
            } while (strcmp(code, "Red") && strcmp(code, "Yellow") && strcmp(code, "Green"));

            insertPatient(name, age, description, code);
        } 
        else if (choice == 2) viewQueue();
        else if (choice == 3) nextQueue();

    } while (choice != 4);

    printf("Exiting program...\n");

    return 0;
}

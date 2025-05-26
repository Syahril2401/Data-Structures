#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_NAME_LENGTH 25

//utk linked list
typedef struct Patient {
    char id[10];
    char name[MAX_NAME_LENGTH];
    char priority[10];
    char doctor[MAX_NAME_LENGTH];
    struct Patient* next;
} Patient;

//utk antrean
typedef struct {
    Patient* head;
} PriorityQueue;

void initQueue(PriorityQueue* queue) {
    queue->head = NULL;
}

//node pasien baru
Patient * createPatien(char id[], char name[], char priority[], char doctor[]) {
    Patient* newPatient = (Patient*)malloc(sizeof(Patient));
    strcpy(newPatient->id, id);
    strcpy(newPatient->name, name);
    strcpy(newPatient->priority, priority);
    strcpy(newPatient->doctor, doctor);
    newPatient->next = NULL;
    return newPatient;
}

//add patient ke antrean(prioritas)
void addPatient(PriorityQueue* queue, char id[], char name[], char priority[], char doctor[]) {
    Patient*newPatient = createPatien(id, name, priority, doctor);
    if (queue->head == NULL || strcmp(newPatient->priority, queue->head->priority) < 0) {
        newPatient->next = queue->head;
        queue->head = newPatient;
    } else {
        Patient* temp = queue->head;
        while (temp->next != NULL && strcmp(newPatient->priority, temp->next->priority) >= 0) {
            temp = temp->next;
        }
        newPatient->next = temp->next;
        temp->next = newPatient;
    }
    printf("Patient %s (%s) added successfully.\n", name, priority);
}

//melayani pasien dg prioritas tinggi
void serveOnePatient(PriorityQueue* queue) {
    if (queue->head == NULL) {
        printf("No patients in queue.\n");
        return;
    }
    Patient* temp = queue->head;
    printf("ATTENTION! Patien %s (%s) is now being treated by Dr. %s\n", temp->name, temp->priority, temp->doctor);
    queue->head = queue->head->next;
    free(temp);
}

//melayani semua patients(prioritas)
void serveAllPatients(PriorityQueue* queue) {
    while (queue->head != NULL) {
        serveOnePatient(queue);
    }
}

//menampilkan data patient(prioritas)
void displayPatients(PriorityQueue* queue) {
    if (queue->head == NULL) {
        printf("No patients in queue.\n");
        return;
    }

    printf("=== Patients Grouped by Priority ===\n");

    char *priorities[] = {"Critical", "Emergency", "Serious", "Mild"};
    int found;

    for (int i = 0; i < 4; i++) {
        printf("%s:\n", priorities[i]);
        found = 0;
        Patient* temp = queue->head;
        while (temp != NULL) {
            if (strcmp(temp->priority, priorities[i]) == 0) {
                printf(" - %s (ID: %s)\n", temp->name, temp->id);
                found = 1;
            }
            temp = temp->next;
        }
        if (!found) {
            printf(" (No patients)\n");
        }
    }
}

//cari riwayat patient(ID)
void searchPatient(PriorityQueue* queue, char id[]) {
    Patient* temp = queue->head;
    while (temp != NULL) {
        if (strcmp(temp->id, id) == 0) {
            printf("Patient found: %s (ID: %s)\nPriority Level: %s\nAssigned Doctor: %s\n", temp->name, temp->id, temp->priority, temp->doctor);
            return;
        }
        printf("No record found for Patient ID: %s\n");
    }
}

//hapus semua data patient
void deleteAllPatients(PriorityQueue* queue) {
    Patient* temp;
    while (queue->head != NULL) {
        temp = queue->head;
        queue->head = queue->head->next;
        free(temp);
    }
    printf("All patient records have been deleted. System reset.\n");
}



int main(){
    PriorityQueue queue;
    initQueue(&queue);
    int choice;
    char id[10], name[MAX_NAME_LENGTH], priority[10], doctor[MAX_NAME_LENGTH];

    do{
        printf("\n1. Add Patient to Queue\n");
        printf("2. Serve One Patient\n");
        printf("3. Serve All Patients\n");
        printf("4. Search Patient History\n");
        printf("5. Display Patients by Priority\n");
        printf("6. Delete All Data\n");
        printf("7. Exit\n");
        printf("Choose an option: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                printf("Enter Patient ID: ");
                scanf("%s", id);
                printf("Enter Patient Name: ");
                scanf("%s", name);
                printf("Enter Priority (Critical/Emergency/Serious/Mild): ");
                scanf("%s", priority);
                printf("Enter Doctor Name: ");
                scanf("%s", doctor);
                addPatient(&queue, id, name, priority, doctor);
                break;
            case 2:
                serveOnePatient(&queue);
                break;
            case 3:
                serveAllPatients(&queue);
                break;
            case 4:
                printf("Enter Patient ID to search: ");
                scanf("%s", id);
                searchPatient(&queue, id);
                break;
            case 5:
                displayPatients(&queue);
                break;
            case 6:
                deleteAllPatients(&queue);
                break;
            case 7:
                printf("Exiting program...\n");
                break;
            default:
                printf("Invalid option, try again.\n");
        }
    } while (choice != 7);
    return 0;
}
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TABLE_SIZE 1007

typedef struct Book {
    char bookId[30], title[51], author[30], isbn[14];
    int pages;
    struct Book *next;
} Book;

Book *hashTable[TABLE_SIZE];
int bookCount = 0;

int hashFunction(char *bookId) {
    int sum = 0;
    for (int i = 0; bookId[i] != '\0'; i++) sum += bookId[i];
    return sum % TABLE_SIZE;
}

void insertBook() {
    char title[51], author[30], isbn[14];
    int pages;
    printf("Enter book title (5-50 chars)(unique): ");
    scanf(" %[^\n]", title);
    if (strlen(title) < 5 || strlen(title) > 50) return;

    printf("Enter author (Mr./Mrs. & 3-25 chars): ");
    scanf(" %[^\n]", author);
    if ((strncmp(author, "Mr. ", 4) != 0 && strncmp(author, "Mrs. ", 5) != 0) || (strlen(author) < 7 || strlen(author) > 25)) return;

    printf("Enter ISBN (10-13 digits)(numeric): ");
    scanf("%s", isbn);
    if (strlen(isbn) < 10 || strlen(isbn) > 13) return;

    printf("Enter page number (>= 16): ");
    scanf(" %d", &pages);
    if (pages < 16) return;

    bookCount++;
    Book *newBook = (Book *)malloc(sizeof(Book));
    sprintf(newBook->bookId, "B%05d-%s-%c%c", bookCount, isbn, author[0], title[0]);
    strcpy(newBook->title, title);
    strcpy(newBook->author, author);
    strcpy(newBook->isbn, isbn);
    newBook->pages = pages;
    newBook->next = NULL;

    int index = hashFunction(newBook->bookId);
    Book **ptr = &hashTable[index];
    while (*ptr) ptr = &((*ptr)->next);
    *ptr = newBook;
    printf("Book inserted successfully with ID: %s\n", newBook->bookId);
}

void viewBook() {
    printf("-------------------------------------------------------------------------------------------------------------\n");
    printf("| %-20s | %-30s | %-20s | %-13s | %-10s |\n", "Book ID", "Book Title", "Book Author", "ISBN", "Page Number");
    printf("-------------------------------------------------------------------------------------------------------------\n");

    int empty = 1;
    for (int i = 0; i < TABLE_SIZE; i++) {
        for (Book *temp = hashTable[i]; temp; temp = temp->next){
            printf("| %-20s | %-30s | %-20s | %-13s | %-10d |\n", 
                   temp->bookId, temp->title, temp->author, temp->isbn, temp->pages);
            empty = 0;
        }
    }

    if (empty) {
        printf("| %-91s |\n", "There are no books in the library.");
    }

    printf("-------------------------------------------------------------------------------------------------------------\n");
}


void removeBook() {
    char bookId[30];
    printf("Enter Book Id to remove: ");
    scanf(" %s", bookId);
    int index = hashFunction(bookId);
    Book **ptr = &hashTable[index];
    while (*ptr) {
        if (strcmp((*ptr)->bookId, bookId) == 0) {
            printf("Book found: %s | %s | %s | %s | %d pages\n", (*ptr)->bookId, (*ptr)->title, (*ptr)->author, (*ptr)->isbn, (*ptr)->pages);
            printf("Are you sure to delete this book? (y/n): ");
            char confirm;
            scanf(" %c", &confirm);
            if (confirm == 'y') {
                Book *temp = *ptr;
                *ptr = (*ptr)->next;
                free(temp);
                printf("Book deleted successfully!\n");
            }
            return;
        }
        ptr = &((*ptr)->next);
    }
    printf("Book not found!\n");
}

int main() {
    int choice;
    while (1) {
        printf("Bluejack Library\n");
        printf("=================\n");
        printf("1. View Book\n2. Insert Book\n3. Remove Book\n4. Exit\n");
        printf(">>");
        scanf(" %d", &choice);
        switch (choice){
        case 1: viewBook(); break;
        case 2: insertBook(); break;
        case 3: removeBook(); break;
        case 4: exit(0);
        default: printf("Invalid choice!\n");
        }
    }
}

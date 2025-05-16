#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LENGTH 500

typedef struct Node {
    char *word;
    struct Node *next;
} Node;

void push(Node **top, char *word) {
    Node *newNode = (Node *)malloc(sizeof(Node));
    newNode->word = strdup(word);
    newNode->next = *top;
    *top = newNode;
}

void reverseString(char *str) {
    int len = strlen(str);
    for (int i = 0; i < len / 2; i++) {
        char temp = str[i];
        str[i] = str[len - 1 - i];
        str[len - 1 - i] = temp;
    }
}

void popAll(Node *top) {
    while (top) {
        reverseString(top->word); 
        printf("%s ", top->word);
        Node *temp = top;
        top = top->next;
        free(temp->word);
        free(temp);
    }
    printf("\n");
}

void reverseSentence(char *sentence) {
    Node *stack = NULL;
    char *token = strtok(sentence, " ");
    int count = 0;

    while (token) {
        push(&stack, token);
        count++;
        token = strtok(NULL, " ");
    }

    if (count < 5) {
        printf("Kalimat harus terdiri dari minimal 5 kata!\n");
        return;
    }

    printf("Reversed Sentence: ");
    popAll(stack);
}

int main() {
    char sentence[MAX_LENGTH], choice[10];
    printf("Reverse Sentence\n");
    printf("================\n");
    printf("\n");

    do {
        printf("Masukkan Kalimat(minimal 5 kata): ");
        fgets(sentence, MAX_LENGTH, stdin);
        sentence[strcspn(sentence, "\n")] = 0;

        reverseSentence(sentence);

        printf("Mau lanjutkan? (yes/no): ");
        fgets(choice, 10, stdin);
        choice[strcspn(choice, "\n")] = 0;
    } while (strcasecmp(choice, "yes") == 0);

    printf("Exiting program.....\n");
    return 0;
}


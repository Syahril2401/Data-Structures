#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

#define ALPHABET_SIZE 26
#define MAX_STRING_LENGTH 100

// struktur node
typedef struct TrieNode
{
    struct TrieNode *children[ALPHABET_SIZE];
    int isEndOfWord;
    char author[MAX_STRING_LENGTH];
    char description[MAX_STRING_LENGTH];
} TrieNode;

// untuk buat node baru
TrieNode *createNode()
{
    TrieNode *newNode = (TrieNode *)malloc(sizeof(TrieNode));
    newNode->isEndOfWord = 0;
    strcpy(newNode->author, "");
    strcpy(newNode->description, "");

    for (int i = 0; i < ALPHABET_SIZE; i++)
    {
        newNode->children[i] = NULL;
    }
    return newNode;
}

// fungsi utk konversi karakter ke index
int charToIndex(char c)
{
    return tolower(c) - 'a';
}

// fungsi utk insert/update buku ke Trie
void insertBook(TrieNode *root, char *title, char *author, char *description)
{
    TrieNode *current = root;
    for (int i = 0; i < strlen(title); i++)
    {
        if (!isalpha(title[i]))
            continue;

        int index = charToIndex(title[i]);

        if (current->children[index] == NULL)
        {
            current->children[index] = createNode();
        }
        current = current->children[index];
    }
    current->isEndOfWord = 1;
    strcpy(current->author, author);
    strcpy(current->description, description);
}

// fungsi utk mencari buku Trie
TrieNode *searchBook(TrieNode *root, char *title)
{
    TrieNode *current = root;

    for (int i = 0; i < strlen(title); i++)
    {
        if (!isalpha(title[i]))
            continue;

        int index = charToIndex(title[i]);

        if (current->children[index] == NULL)
        {
            return NULL;
        }
        current = current->children[index];
    }
    return (current != NULL && current->isEndOfWord) ? current : NULL;
}

// fungsi utk mencari prefix Trie
TrieNode *searchPrefix(TrieNode *root, char *prefix)
{
    TrieNode *current = root;

    for (int i = 0; i < strlen(prefix); i++)
    {
        if (!isalpha(prefix[i]))
            continue;

        int index = charToIndex(prefix[i]);

        if (current->children[index] == NULL)
        {
            return NULL;
        }
        current = current->children[index];
    }
    return current;
}

// fungsi utk mengumpulkan semua kata dengan prefix tertentu
void collectWordsWithPrefix(TrieNode *node, char *prefix, char *currentWord, int depth)
{
    if (node->isEndOfWord)
    {
        currentWord[depth] = '\0';
        printf(" -%s%s (Author: %s)\n", prefix, currentWord, node->author);
    }

    for (int i = 0; i < ALPHABET_SIZE; i++)
    {
        if (node->children[i] != NULL)
        {
            currentWord[depth] = 'a' + i;
            collectWordsWithPrefix(node->children[i], prefix, currentWord, depth + 1);
        }
    }
}

// fungsi utk menampilkan semua kata Trie
void displayAllBooks(TrieNode *node, char *currentWord, int depth, int *count)
{
    if (node->isEndOfWord)
    {
        currentWord[depth] = '\0';
        (*count)++;
        printf("%d. %s\n", *count, currentWord);
        printf(" Author: %s\n", node->author);
        printf(" Description: %s\n\n", node->description);
    }

    for (int i = 0; i < ALPHABET_SIZE; i++)
    {
        if (node->children[i] != NULL)
        {
            currentWord[depth] = 'a' + i;
            displayAllBooks(node->children[i], currentWord, depth + 1, count);
        }
    }
}

// fungsi utk menghitung jumlah buku
int countBooks(TrieNode *node)
{
    if (node == NULL)
        return 0;

    int count = 0;
    if (node->isEndOfWord)
        count = 1;

    for (int i = 0; i < ALPHABET_SIZE; i++)
    {
        if (node->children[i] != NULL)
        {
            count += countBooks(node->children[i]);
        }
    }
    return count;
}

// fungsi utk input judul
int isValidTitle(char *title)
{
    if (strlen(title) <= 1)
        return 0;

    // check if contains space
    for (int i = 0; i < strlen(title); i++)
    {
        if (title[i] == ' ')
            return 0;
    }
    return 1;
}

// fungsi validasi input author/description(minimal 2 kata)
int isValidDescription(char *text)
{
    int words = 0;
    int inWord = 0;

    for (int i = 0; i < strlen(text); i++)
    {
        if (text[i] != ' ' && text[i] != '\t' && text[i] != '\n')
        {
            if (!inWord)
            {
                words++;
                inWord = 1;
            }
        }
        else
        {
            inWord = 0;
        }
    }
    return words >= 2;
}

// fungsi utk membersihkan input buffer
void clearInputBuffer()
{
    int c;
    while ((c = getchar()) != '\n' && c != EOF)
        ;
}

void getStringInput(char *buffer, int maxLenght)
{
    if (fgets(buffer, maxLenght, stdin) != NULL)
    {
        buffer[strcspn(buffer, "\n")] = '\0';
    }
}

// menu 1: tambah buku baru
void addNewBook(TrieNode *root)
{
    char title[MAX_STRING_LENGTH];
    char author[MAX_STRING_LENGTH];
    char description[MAX_STRING_LENGTH * 2];

    printf("\n ADD NEW BOOK \n");

    do
    {
        printf("Input book title [Must be more than 1 character and contains no space]: ");
        getStringInput(title, MAX_STRING_LENGTH);

        if (!isValidTitle(title))
        {
            printf("Invalid title! Title must be more than 1 character and contains no space.\n");
        }
    } while (!isValidTitle(title));

    // input dan validasi author
    do
    {
        printf("Input book author [Must be more than 1 word]: ");
        getStringInput(author, MAX_STRING_LENGTH);

        if (!isValidDescription(author))
        {
            printf("Invalid author! Author must be more than 1 word.\n");
        }
    } while (!isValidDescription(author));

    // input dan validasi description
    do
    {
        printf("Input book description [Must be more than 1 word]: ");
        getStringInput(description, MAX_STRING_LENGTH * 2);

        if (!isValidDescription(description))
        {
            printf("Invalid description! Description must be more than 1 word.\n");
        }
    } while (!isValidDescription(description));

    // Check if book already exists
    TrieNode *existing = searchBook(root, title);

    insertBook(root, title, author, description);

    if (existing != NULL)
    {
        printf("\nSuccessfully updated book information.\n");
    }
    else
    {
        printf("\nSuccessfully added new book to library.\n");
    }

    printf("\nPress enter to continue...");
    getchar();
}

// menu 2: cari buku
void searchBookMenu(TrieNode *root)
{
    char title[MAX_STRING_LENGTH];

    printf("\n SEARCH BOOK\n");

    do
    {
        printf("Input book title to search [Must be more than 1 character and contains no space]: ");
        getStringInput(title, MAX_STRING_LENGTH);

        if (!isValidTitle(title))
        {
            printf("Invalid title! Title must be more than 1 character and contains no space.\n");
        }
    } while (!isValidTitle(title));

    TrieNode *result = searchBook(root, title);

    if (result == NULL)
    {
        printf("\nThere is no book \"%s\" in the library.\n", title);
    }
    else
    {
        printf("\nBook found:\n");
        printf("Title: %s\n", title);
        printf("Author: %s\n", result->author);
        printf("Description: %s\n", result->description);
    }

    printf("\nPress enter to continue...");
    getchar();
}

// menu 3: lihat buku berdasarkan prefix
void viewBookByPrefix(TrieNode *root)
{
    char prefix[MAX_STRING_LENGTH];
    char currentWord[MAX_STRING_LENGTH];

    printf("\n VIEW BOOK BY PREFIX\n");
    printf("Input prefix to search: ");
    getStringInput(prefix, MAX_STRING_LENGTH);

    TrieNode *prefixNode = searchPrefix(root, prefix);

    if (prefixNode == NULL)
    {
        printf("\n There is no prefix \"%s\" in the library.\n", prefix);
    }
    else
    {
        printf("\nBooks starting with \"%s\":\n", prefix);
        collectWordsWithPrefix(prefixNode, prefix, currentWord, 0);
    }

    printf("\nPress enter to continue...");
    getchar();
}

// menu 4: lihat semua buku
void viewAllBooks(TrieNode *root)
{
    char currentWord[MAX_STRING_LENGTH];
    int count = 0;

    printf("\n ALL BOOKS IN LIBRARY\n");

    int totalBooks = countBooks(root);

    if (totalBooks == 0)
    {
        printf("There are no books yet in the library.\n");
    }
    else
    {
        printf("List of all books in the library:\n\n");
        displayAllBooks(root, currentWord, 0, &count);
        printf("Total books: %d\n", totalBooks);
    }
    printf("\nPress enter to continue...");
    getchar();
}

// funsi untuk menampilkan menu utama
void displayMenu()
{
    printf("\n================================\n");
    printf("DIGITAL LIBRARY SYSTEM\n");
    printf("================================\n");
    printf("1. Add new book\n");
    printf("2. Search book\n");
    printf("3. View books by prefix\n");
    printf("4. View all books\n");
    printf("5. Exit\n");
    printf("================================\n");
    printf("Choose menu: ");
}

// fungsi utk inisialisasi data sample
void initializeSampleData(TrieNode *root)
{
    // Sample data untuk testing
    insertBook(root, "algorithms", "Thomas Cormen", "Introduction to algorithms and data structures");
    insertBook(root, "programming", "Brian Kernighan", "The art of computer programming");
    insertBook(root, "datastructures", "Robert Sedgewick", "Algorithms and data structures in C");
    insertBook(root, "computerscience", "Andrew Tanenbaum", "Computer science fundamentals");
    insertBook(root, "softwareengineering", "Ian Sommerville", "Software engineering principles and practices");
    insertBook(root, "databases", "Raghu Ramakrishnan", "Database management systems");
    insertBook(root, "networks", "James Kurose", "Computer networking top down approach");
    insertBook(root, "operatingsystems", "Abraham Silberschatz", "Operating system concepts");
    insertBook(root, "artificialintelligence", "Stuart Russell", "Artificial intelligence modern approach");
    insertBook(root, "machinelearning", "Tom Mitchell", "Machine learning algorithms and applications");
    insertBook(root, "cybersecurity", "William Stallings", "Principles and practices of cybersecurity");
    insertBook(root, "cloudcomputing", "Rajkumar Buyya", "Essentials of cloud computing and distributed systems");
    insertBook(root, "mobiledevelopment", "Bill Phillips", "Android app development using Kotlin");
    insertBook(root, "frontenddevelopment", "Eric Freeman", "HTML, CSS, and JavaScript for modern UI design");
    insertBook(root, "backenddevelopment", "Jon Duckett", "Server-side programming and database integration");
}

int main()
{
    TrieNode *root = createNode();
    int choice;

    initializeSampleData(root);

    printf("Welcome to Digital Library System!\n");
    printf("System initialized with 10 sample books.\n");

    do
    {
        displayMenu();
        scanf("%d", &choice);
        clearInputBuffer();

        switch (choice)
        {
        case 1:
            addNewBook(root);
            break;
        case 2:
            searchBookMenu(root);
            break;
        case 3:
            viewBookByPrefix(root);
            break;
        case 4:
            viewAllBooks(root);
            break;
        case 5:
            printf("\nThank you for using Digital Library System!\n");
            printf("Have a great day :)\n");
            break;
        default:
            printf("\nInvalid choice! Please choose 1-5.\n");
            printf("Press enter to continue...");
            getchar();
            break;
        }
    } while (choice != 5);

    return 0;
}
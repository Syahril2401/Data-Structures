#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

typedef struct Customer {
    char name[30];
    char email[30];
    char phone[15];
    int points;
    struct Customer *left, *right;
} Customer;

// Membuat node baru 
Customer* createNode(char name[], char email[], char phone[]) {
    Customer* newNode = (Customer*)malloc(sizeof(Customer));
    strcpy(newNode->name, name);
    strcpy(newNode->email, email);
    strcpy(newNode->phone, phone);
    newNode->points = 10; // Default 10 poin untuk pelanggan baru
    newNode->left = newNode->right = NULL;
    return newNode;
}

// Menambahkan pelanggan
Customer* insert(Customer* root, char name[], char email[], char phone[]) {
    if (root == NULL) return createNode(name, email, phone);
    if (strcmp(phone, root->phone) < 0)
        root->left = insert(root->left, name, email, phone);
    else if (strcmp(phone, root->phone) > 0)
        root->right = insert(root->right, name, email, phone);
    return root;
}

// Menampilkan semua pelanggan 
void inOrder(Customer* root) {
    if (root != NULL) {
        inOrder(root->left);
        printf("| %-10s | %-13s | %-20s | %-6d |\n", root->name, root->phone, root->email, root->points);
        printf("--------------------------------------------------------------\n");
        inOrder(root->right);
    }
}

// Mencari pelanggan 
Customer* search(Customer* root, char phone[]) {
    if (root == NULL || strcmp(root->phone, phone) == 0)
        return root;
    if (strcmp(phone, root->phone) < 0)
        return search(root->left, phone);
    return search(root->right, phone);
}

// Menghapus pelanggan 
Customer* deleteNode(Customer* root, char phone[]) {
    if (root == NULL) return root;

    if (strcmp(phone, root->phone) < 0) {
        root->left = deleteNode(root->left, phone);
    } else if (strcmp(phone, root->phone) > 0) {
        root->right = deleteNode(root->right, phone);
    } else {
        
        if (root->left == NULL) {
            Customer* temp = root->right;
            free(root);
            return temp;
        } else if (root->right == NULL) {
            Customer* temp = root->left;
            free(root);
            return temp;
        }

        Customer* temp = root->right;
        while (temp && temp->left != NULL) {
            temp = temp->left;
        }
        strcpy(root->name, temp->name);
        strcpy(root->email, temp->email);
        strcpy(root->phone, temp->phone);
        root->points = temp->points;
        root->right = deleteNode(root->right, temp->phone);
    }
    return root;
}

// Validasi nomor tlp
int isValidPhone(char phone[]) {
    int len = strlen(phone);
    if (len < 10 || len > 13) return 0;
    for (int i = 0; i < len; i++) {
        if (!isdigit(phone[i])) return 0;
    }
    return 1;
}

// Validasi nama 
int isValidName(char name[]) {
    int len = strlen(name);

    name[strcspn(name, "\n")] = 0;

    if (len < 6 || len > 25) return 0; 

    if ((strncmp(name, "Mr. ", 4) == 0 || strncmp(name, "Mrs. ", 5) == 0) && isalpha(name[strlen(name) - 1])) {
        return 1;
    }
    return 0;
}


// Validasi email 
int isValidEmail(char email[]) {
    int len = strlen(email);
    if (len < 10 || len > 20) return 0;
    if (!strchr(email, '@')) return 0;
    if (!(strstr(email, ".com") || strstr(email, ".co.id"))) return 0;
    return 1;
}

// Memproses pesanan pelanggan
void processOrder(Customer** root) {
    char phone[15], name[30], email[30];
    printf("Enter Phone Number: ");
    fgets(phone, sizeof(phone), stdin);
    phone[strcspn(phone, "\n")] = 0; 

    if (!isValidPhone(phone)) {
        printf("Invalid Phone Number!\n");
        return;
    }

    Customer* customer = search(*root, phone);
    if (!customer) {
        printf("Enter name: ");
        fgets(name, sizeof(name), stdin);
        name[strcspn(name, "\n")] = 0; 
        if (!isValidName(name)) {
            printf("Invalid Name!\n");
            return;
        }

        printf("Enter email: ");
        fgets(email, sizeof(email), stdin);
        email[strcspn(email, "\n")] = 0; 
        if (!isValidEmail(email)) {
            printf("Invalid Email!\n");
            return;
        }

        *root = insert(*root, name, email, phone);
        printf("Customer added successfully!\n");
        customer = search(*root, phone);
    }

    int orderTotal = 0, quantity;
    char drink[20], choice;
    do {
        printf("Enter Drink Name (Cafe Latte / Caramel Macchiato / Cappuccino / Cafe Mocha): ");
        fgets(drink, sizeof(drink), stdin);
        drink[strcspn(drink, "\n")] = 0; 
        if (strcmp(drink, "Cafe Latte") != 0 && strcmp(drink, "Caramel Macchiato") != 0 &&
            strcmp(drink, "Cappuccino") != 0 && strcmp(drink, "Cafe Mocha") != 0) {
            printf("Invalid Drink!\n");
            return;
        }

        printf("Enter Quantity: ");
        scanf("%d", &quantity);
        getchar(); 
        if (quantity < 1) {
            printf("Invalid Quantity!\n");
            return;
        }

        orderTotal += quantity * 30000;
        printf("Do you want to order more? (y/n): ");
        scanf("%c", &choice);
        getchar(); 
    } while (choice == 'y');

    printf("Total Order: %d\n", orderTotal);
    customer->points += (orderTotal / 50000) * 3;
}

// Menampilkan menu utama
void menu(Customer** root) {
    int choice;
    char phone[15]; 
    do {
        printf("\nBlueBucks\n");
        printf("==================\n");
        printf("1. Process Order\n");
        printf("2. View All Customers\n");
        printf("3. Remove Customer\n");
        printf("4. Exit\n");
        printf(">> ");
        scanf("%d", &choice);
        getchar(); 

        switch (choice) {
            case 1:
                processOrder(root);
                break;
            case 2:
                if (*root == NULL)
                    printf("There is no data!\n");
                else {
                    printf("\n--------------------------------------------------------------\n");
                    printf("| Name       | Phone Number  | Email                | Points |\n");
                    printf("--------------------------------------------------------------\n");
                    inOrder(*root);
                }
                break;
            case 3:
                printf("Enter phone number to remove: ");
                fgets(phone, sizeof(phone), stdin);
                phone[strcspn(phone, "\n")] = 0; 
                if (search(*root, phone)) {
                    *root = deleteNode(*root, phone);
                    printf("Customer removed successfully!\n");
                } else {
                    printf("Data not found!\n");
                }
                break;
            case 4:
                printf("Exiting...\n");
                break;
            default:
                printf("Invalid choice!\n");
        }
    } while (choice != 4);
}

int main() {
    Customer* root = NULL;
    menu(&root);
    return 0;
}
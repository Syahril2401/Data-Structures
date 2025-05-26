#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//#define FILE_NAME "cakes.txt"
#define MAX_CAKES 100

typedef struct {
	char code[6];
	char name[30];
	int available;
	int price;
} Cake;

Cake cakes[MAX_CAKES];
int cakeCount = 0;

void loadCakes() {
	FILE *file = fopen("cakes.txt", "r");
	if (!file) {
		printf("error file opening!\n");
		return;
	}
	cakeCount = 0;
	while (fscanf(file, "%s %[^0-9] %d %d", cakes[cakeCount].code, cakes[cakeCount].name, &cakes[cakeCount].available, &cakes[cakeCount].price) == 4){
		cakeCount++;	
	}
	fclose(file);
}

void saveCakes() {
	FILE *file = fopen("cakes.txt", "w");
	if(!file) {
		printf("error file opening!\n");
		return;
	}
	for (int i = 0; i < cakeCount; i++) {
		fprintf(file, "%s %s %d %d\n", cakes[i].code, cakes[i].name, cakes[i].available, cakes[i].price);
	}
	fclose(file);
}

void displayCakes() {
	printf("\nBLUE CAKE SHOP CASHIER\n====================\n");
	printf("\n");
	printf("No. | Cake Code | Cake Name               | Available | Price\n");
	printf("------------------------------------------------------------------\n");
	
	for (int i = 0; i < cakeCount; i++){
		printf("%02d. | %-9s | %-23s | %9d | Rp. %d,-\n", i + 1,cakes[i].code, cakes[i].name, cakes[i].available, cakes[i].price);
	}
	printf("------------------------------------------------------------------\n");
}

int findCake(char *code) {
	for (int i = 0; i < cakeCount; i++){
		if (strcmp(cakes[i].code, code) == 0)
		return i;
	}
	return -1;
}

void sellCake() {
	char code[6];
	int quantity;
	printf("Enter Cake Code: ");
	scanf("%s", &code);
	int index = findCake(code);
	if (index == -1) {
		printf("--- The Cake Code doesnt exist ---\n");
		return;
	}
	printf("Input Quantity: ");
	scanf("%d", &quantity);
	if (quantity <= 0 || quantity > cakes[index].available){
		printf("...The quantity of cake is not enough...\n");
		return;
	}
	int totalPrice = quantity * cakes[index].price;
	printf("Total Price is : Rp %d, - x %d = Rp%d, -\n", cakes[index].price, quantity, totalPrice);
	cakes[index].available -= quantity;
	saveCakes();
	
	printf("--- Thank You ---\n");
}

void addStock() {
	char code[6];
	int quantity;
	printf("Input Cake Code : ");
	scanf("%s", code);
	int index = findCake(code);
	if(index == -1) {
		printf("--- The Cake Code doesnt exist ---\n");
		return;
	}
	printf("Enter Quantity (1..10):");
	scanf("%d", &quantity);
	if(quantity < 1 || quantity > 10) {
		printf("Invalid quantity!\n");
		return;
	}
	cakes[index].available += quantity;
	printf("--- Adding Stock Success ---\n");
	saveCakes();
}

int main() {
	loadCakes();
	int choice;
	do {
		displayCakes();
		printf("1. Sell\n2. Add Stock\n3. Exit\nInput choice: ");
		scanf("%d", &choice);
		switch (choice) {
			case 1:
				sellCake();
				break;
			case 2:
				addStock();
				break;	
			case 3:
				printf("Exiting program...\n");
				break;
			default:
				printf("Invalid choice!\n");
		}
	} while (choice != 3);
	return 0;
}

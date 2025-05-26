#include <stdio.h>
#include <stdlib.h>

#define MAX_SIZE 100

int heap[MAX_SIZE];
int heapSize = 0;

void swap(int *a, int *b)
{
    int temp = *a;
    *a = *b;
    *b = temp;
}


void heapifyUp(int index) {
    while (index > 0) {
        int parent = (index - 1) / 2;
        if (heap[index] > heap[parent]) {
            swap(&heap[index], &heap[parent]);
            index = parent;
        } else {
            break;
        }
    }
}


void insert(int value) {
    if (heapSize >= MAX_SIZE) {
        printf("return is full\n");
        return;
    }
    heap[heapSize] = value;
    heapifyUp(heapSize);
    heapSize++;
    printf("insert Success\n");
}


void heapifyDown(int index) {
    int largest = index;
    int left = 2 * index + 1;
    int right = 2 * index + 2;

    if (left < heapSize && heap[left] > heap[largest]) largest = left;
    if (right < heapSize && heap[right] >heap[largest]) largest = right;    
    if (largest != index) {
        swap(&heap[index], &heap[largest]);
        heapifyDown(largest);
    }
}


void pop() {
    if (heapSize == 0) {
        printf("There is no data\n");
        return;
    }
    heap[0] = heap[heapSize - 1];
    heapSize--;
    heapifyDown;
}


void view() {
    if (heapSize == 0) {
        printf("Heap is empty\n");
        return;
    }
    for (int i = 0; i < heapSize; i++) {
        printf("%d", heap[i]);
    }
    printf("\n");
}


int main() {
    int choice, value;
    while (1) {
        printf("MAX HEAP TREE\n");
        printf("==============\n");
        printf("1. Insert\n");
        printf("2. View\n");
        printf("3. Pop\n");
        printf("4. Exit\n");
        printf("Choose: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
            printf("Input number (1-100): ");
            scanf("%d", &value);
            if (value >= 1 && value <= 100) {
                insert(value);
            } else {
                printf("Invalid input\n");
            }
            break;
        case 2:
            view();
            break;
        case 3:
            pop();
            break;
        case 4:
            printf("Exiting program...\n");
            heapSize = 0;
            exit(0);
        default:
                printf("Invalid choice\n");
        }
    }
    return 0;
}
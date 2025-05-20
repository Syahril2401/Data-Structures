#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

struct Edge {
    char src, dest;
};

struct Graph {
    int V, E;
    struct Edge* edges;
};

struct Subset {
    char parent;
    int rank;
};

struct Graph* createGraph(int V, int E) {
    struct Graph* graph = (struct Graph*)malloc(sizeof(struct Graph));
    graph->V = V;
    graph->E = E;
    graph->edges = (struct Edge*)malloc(E * sizeof(struct Edge));
    return graph;
}

char find(struct Subset subsets[], char i) {
    if (subsets[i - 'a'].parent != i)
        subsets[i - 'a'].parent = find(subsets, subsets[i - 'a'].parent);
    
    return subsets[i - 'a'].parent;
}

void Union(struct Subset subsets[], char x, char y) {
    char xroot = find(subsets, x);
    char yroot = find(subsets, y);

    if (subsets[xroot - 'a'].rank < subsets[yroot - 'a'].rank)
        subsets[xroot - 'a'].parent = yroot;
    else if (subsets[xroot - 'a'].rank > subsets[yroot - 'a'].rank)
        subsets[yroot - 'a'].parent = xroot;
    else {
        subsets[yroot - 'a'].parent = xroot;
        subsets[xroot - 'a'].rank++;
    }
}

bool isCyclic(struct Graph* graph) {
    struct Subset* subsets = (struct Subset*)malloc(graph->V * sizeof(struct Subset));
    
    for (int v = 0; v < graph->V; v++) {
        char vertexChar = 'a' + v;
        subsets[v].parent = vertexChar;
        subsets[v].rank = 0;
        printf("Make Set(%c)\n", vertexChar);
    }
    
    for (int e = 0; e < graph->E; e++) {
        char x = find(subsets, graph->edges[e].src);
        char y = find(subsets, graph->edges[e].dest);
        
        printf("\nLoop %d:\n", e + 1);
        printf("Edge %d (%c, %c)\n", e + 1, graph->edges[e].src, graph->edges[e].dest);
        printf("Find(%c) = %c\n", graph->edges[e].src, x);
        printf("Find(%c) = %c\n", graph->edges[e].dest, y);
        
        if (x == y) {
            printf("Vertex %c dan %c berada dalam subset yang sama.\n", graph->edges[e].src, graph->edges[e].dest);
            printf("Graf tersebut SIKLIK!\n");
            return true;
        }
        
        printf("Vertex %c dan %c berada dalam subset berbeda, melakukan Union(%c, %c)\n", 
               graph->edges[e].src, graph->edges[e].dest, graph->edges[e].src, graph->edges[e].dest);
        Union(subsets, graph->edges[e].src, graph->edges[e].dest);
        
        printf("Status subset setelah union:\n");
        for (int v = 0; v < graph->V; v++) {
            printf("Vertex %c -> parent: %c, rank: %d\n", 'a' + v, subsets[v].parent, subsets[v].rank);
        }
    }
    
    printf("\nGraf tersebut TIDAK SIKLIK!\n");
    return false;
}

void printGraph(struct Graph* graph) {
    printf("Graf memiliki %d vertex dan %d edge\n", graph->V, graph->E);
    printf("Edge-edge dalam graf:\n");
    for (int i = 0; i < graph->E; i++) {
        printf("%c -- %c\n", graph->edges[i].src, graph->edges[i].dest);
    }
}

int main() {
    int V = 4;
    int E = 4;

    struct Graph* graph = createGraph(V, E);

    graph->edges[0].src = 'a';
    graph->edges[0].dest = 'b';
    
    graph->edges[1].src = 'a';
    graph->edges[1].dest = 'c';

    graph->edges[2].src = 'a';
    graph->edges[2].dest = 'd';

    graph->edges[3].src = 'c';
    graph->edges[3].dest = 'd';

    printf("Simulasi Deteksi Siklus menggunakan Disjoint Sets:\n\n");
    printGraph(graph);
    printf("\n");
    
    if (isCyclic(graph))
        printf("\nHasil: Graf memiliki siklus!\n");
    else
        printf("\nHasil: Graf tidak memiliki siklus.\n");
    
    return 0;
}
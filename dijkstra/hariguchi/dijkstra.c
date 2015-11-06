#include <stdio.h>

#define NNODE 6
#define INF 100

int Empty(int* Neighbor);
int Full(int* set);
void insert(int* set, int index);
int SetOrNotSet(int* set, int index);
void Dijkstra(int root, int (*cost)[NNODE]);
int Pull(int* neighbor);
void Update(int* neighbor, int* set, int currentvertex,
                                        int (*cost)[NNODE]);
int Minmum(int* set, int* Dcost, int (*cost)[NNODE]);
void AddNeighbor(int* neighbor, int index);
char GetChar(int val);

int
main()
{
    int cost[NNODE][NNODE] = {
        { 0, 2, 5, 1, INF, INF }, { 2, 0, 3, 2, INF, INF },
        { 5, 3, 0, 3, 1, 5 },     { 1, 2, 3, 0, 1, INF },
        { INF, INF, 1, 1, 0, 2 }, { INF, INF, 5, INF, 2, 0 }
    };
    int root;
    for (root = 0; root < NNODE; root++)
        Dijkstra(root, cost);
}

void
Dijkstra(int root, int (*cost)[NNODE])
{
    int D[NNODE];
    int p[NNODE];
    int set[NNODE];
    int neighbor[NNODE];
    int i;
    for (i = 0; i < NNODE; i++)
        set[i] = -1;
    set[0] = root;
    for (i = 0; i < NNODE; i++)
        neighbor[i] = -1;
        for (i = 0; i < NNODE; i++)
        {
            D[i] = cost[root][i];
            if (cost[root][i] == INF)
                p[i] = -1;
            else {
                p[i] = root;
                if (root != i) {
                    AddNeighbor(neighbor, i);
                }
            }
        }
    while (!Full(set)) {
        int w = Minmum(set, D, cost);
        insert(set, w);
        Update(neighbor, set, w, cost);
        while (Empty(neighbor)) {
            int v = Pull(neighbor);
            if (D[v] > D[w] + cost[w][v]) {
                D[v] = D[w] + cost[w][v];
                p[v] = w;
            }
        }
    }

    printf("Root Node is: %c\n\t", GetChar(root));
    for (i = 0; i < NNODE; i++) {
        printf("[%c, %c, %d] ", GetChar(i), GetChar(p[i]), D[i]);
    }
    printf("\n");
}

int
Empty(int* neighbor)
{
    if (neighbor[0] == -1) {
        return 0;
    }
    return 1;
}

int
Full(int* set)
{
    int i;
    for (i = 0; i < NNODE; i++) {
        if (set[i] == -1) {
            return 0;
        }
    }
    return 1;
}

int
SetOrNotSet(int* set, int index)
{
    int i;
    for (i = 0; i < NNODE; i++) {
        if (set[i] == index) {
            return 1;
        }
    }
    return 0;
}

int
Pull(int* neighbor)
{
    int vertex = 0;
    int i = 0;
    while (!vertex) {
        if (neighbor[i] == -1) {
            vertex = i;
        }
        i++;
    }
    int ret = neighbor[vertex - 1];
    neighbor[vertex - 1] = -1;
    return ret;
}

void
insert(int* set, int index)
{
    int i;
    for (i = 0; i < NNODE; i++) {
        if (set[i] == -1) {
            set[i] = index;
            break;
        }
    }
}

int
Minmum(int* set, int* Dcost, int (*cost)[NNODE])
{
    int min = INF;
    int vertex = INF;
    int i;
    for (i = 0; i < NNODE; i++) {
        if (min >= Dcost[i] && !SetOrNotSet(set, i) && Dcost[i] > 0) {
            vertex = i;
            min = Dcost[i];
        }
    }
    return vertex;
}

void
AddNeighbor(int* neighbor, int index)
{
    int i;
    for (i = 0; i < NNODE; i++) {
        if (neighbor[i] == -1) {
            neighbor[i] = index;
            break;
        }
    }
}

void
Update(int* neighbor, int* set, int currentvertex, int (*cost)[NNODE])
{
    int i;
    for (i = 0; i < NNODE; i++) {
        neighbor[i] = -1;
    }
    for (i = 0; i < NNODE; i++) {
        if (cost[currentvertex][i] != INF && cost[currentvertex][i] != 0 &&
                !SetOrNotSet(set, i)) {
            AddNeighbor(neighbor, i);
        }
    }
}

char
GetChar(int node)
{
    if(node == 0) { return 'A'; }
    if(node == 1) { return 'B'; }
    if(node == 2) { return 'C'; }
    if(node == 3) { return 'D'; }
    if(node == 4) { return 'E'; }
    if(node == 5) { return 'F'; }
    return 'X';
}

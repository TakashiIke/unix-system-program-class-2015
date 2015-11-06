#include <stdio.h>

#define NNODE 6
#define INF 100

void Initialization(int *set, int (*cost)[NNODE], int *neighbor, int *p, int *Dcost, int root);
int IsNeighbor(int *Neighbor);
int IsFull(int *set);
void insertSet(int *set, int index);
int isSet(int *set, int index);
void DijkstraShortPath(int root, int (*cost)[NNODE]);
int TakeNeighbor(int *neighbor);
void UpdateNeighbor(int *neighbor, int *set, int currentvertex, int (*cost)[NNODE]);
int MinCostVertex(int *set, int *Dcost, int (*cost)[NNODE]);
void InsertNeighbor(int *neighbor, int index);
void InsertSet(int *set, int index);
void PrintAnswer(int *Dcost, int *p, int root);
char Conversion(int val);

int main(){
  int cost[NNODE][NNODE] = {
    {0, 2, 5, 1, INF, INF},
    {2, 0, 3, 2, INF, INF},
    {5, 3, 0, 3, 1, 5},
    {1, 2, 3, 0, 1, INF},
    {INF, INF, 1, 1, 0, 2},
    {INF, INF, 5, INF, 2, 0}
  };

  for(int i = 0; i < NNODE; i++){
    DijkstraShortPath(i, cost);
  }
  return 0;
}

void DijkstraShortPath(int root, int (*cost)[NNODE]){
  //declaration
  int Dcost[NNODE];
  int p[NNODE];
  int set[NNODE];
  int neighbor[NNODE];
  // declaration end
  Initialization(set, cost, neighbor, p, Dcost, root);
  // Algorithm start
  while(!IsFull(set)){
    int w = MinCostVertex(set, Dcost, cost);
    InsertSet(set, w);
    UpdateNeighbor(neighbor, set, w, cost);
    while(IsNeighbor(neighbor)){
      int v = TakeNeighbor(neighbor);
      if(Dcost[v] > Dcost[w] + cost[w][v]){
  Dcost[v] = Dcost[w] + cost[w][v];
  p[v] = w;
      }
    }
  }
  PrintAnswer(Dcost, p, root);
}

// initialization for set, neighbor, p, and Dcost.
// Place -1 for all elemnts in set and neighbor at first.
// place root node for set array and the neighbor nodes that are adjacent to the root node
// calculate the initial cost for all possible path
void Initialization(int *set, int (*cost)[NNODE], int *neighbor, int *p, int *Dcost, int root){
  for(int i = 0; i < NNODE; i++){
    set[i] = -1;
  }
  set[0] = root;
  for(int i = 0; i < NNODE; i++){
    neighbor[i] = -1;
  }
  for(int i = 0; i < NNODE; i++){
    Dcost[i] = cost[root][i];
    if(cost[root][i] == INF){
      p[i] = -1;
    }
    else{
      p[i] = root;
      if(root != i){
  InsertNeighbor(neighbor, i);
      }
    }
  }
}

// check if array has non negative elements or not
// if there existe, return 1 and 0 otherwise
int IsNeighbor(int *neighbor){
  if(neighbor[0] == -1){
    return 0;
  }
  return 1;
}
// return 1 if all the vertecies in question is in the set
// return 0 otherwise
int IsFull(int *set){
  for(int i = 0; i < NNODE; i++){
    if(set[i] == -1){
      return 0;
    }
  }
  return 1;
}
// return 1 if index is in set return 0 otherwise
int isSet(int *set, int index){
  for(int i = 0; i < NNODE; i++){
    if(set[i] == index){
      return 1;
    }
  }
  return 0;
}

// assume that there exist some neighbor vertex in the set.
// also assume that neighbor indecies are put from the right consecutevely
// like 0, 1, 2 or something like that
// start from array index 0, if we hit the array index that
// contains -1, we know that that element is the first appearence of -1
// this hold true by the first and second assumption
// take the neighbor[first appearence of -1 index - 1] so that we don't want to
// contain -1 randomly in the array we set -1 after taking out the value
int TakeNeighbor(int *neighbor){
  int vertex = 0;
  int i = 0;
  while(!vertex){
    if(neighbor[i] == -1){
      vertex = i;
    }
    i++;
  }
  int ret = neighbor[vertex - 1];
  neighbor[vertex - 1] = -1;
  return ret;
}

// put the index in the set such that set array contains -1
// we assume that non negative value in the set is put for early indecies in the set
// and -1 after that
void InsertSet(int *set, int index){
  for(int i = 0; i < NNODE; i++){
    if(set[i] == -1){
      set[i] = index;
      break;
    }
  }
}

// take the vertex that contains mininum total cost with the vertex not in the set.
// printing statement is just for debuggin as this function never returns INF
int MinCostVertex(int *set, int *Dcost, int (*cost)[NNODE]){
  int min = INF;
  int vertex = INF;
  for(int i = 0; i < NNODE; i++){
    if(min >= Dcost[i] && !isSet(set, i) && Dcost[i] > 0){
      vertex = i;
      min = Dcost[i];
    }
  }
  if(vertex == INF){
    printf("Oops something bad happened in MINCOSTVERTEX\n");
  }
  return vertex;
}

// the same as InsertSet funciton just chill
void InsertNeighbor(int *neighbor, int index){
  for(int i = 0; i < NNODE; i++){
    if(neighbor[i] == -1){
      neighbor[i] = index;
      break;
    }
  }
}

// after getting the vertex from MinCostVertex, we need to update the neighbor set
// in order to pursue the algorithm. Just that's it !!!!!!!!!!!!!!!
void UpdateNeighbor(int *neighbor, int *set, int currentvertex, int (*cost)[NNODE]){
  for(int i = 0; i < NNODE; i++){
    neighbor[i] = -1;
  }
  for(int i = 0; i < NNODE; i++){
    if(cost[currentvertex][i] != INF && cost[currentvertex][i] != 0 && !isSet(set, i)){
      InsertNeighbor(neighbor, i);
    }
  }
}

// printing asnwers
void PrintAnswer(int *Dcost, int *p, int root){
  printf("Root Node : %c\n\t", Conversion(root));
  for(int i = 0; i < NNODE; i++){
    printf("[%c, %c, %d] ", Conversion(i), Conversion(p[i]), Dcost[i]);
  }
  printf("\n");
}

// conversiont between node to string
// 0 => A
// 1 => B
// 2 => C
// 3 => D
// 4 => E
// 5 => F
char Conversion(int node){
  switch(node){
  case 0:
    return 'A';
  case 1:
    return 'B';
  case 2:
    return 'C';
  case 3:
    return 'D';
  case 4:
    return 'E';
  case 5:
    return 'F';
  }
}

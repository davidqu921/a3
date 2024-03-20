/*
 * Graph algorithms.
 *
 * Author (of starter code): A. Tafliovich.
 */

#include <limits.h>

#include "graph.h"
#include "minheap.h"

#define NOTHING -1
#define DEBUG 0

typedef struct records {
  int numVertices;    // total number of vertices in the graph
                      // vertex IDs are 0, 1, ..., numVertices-1
  MinHeap* heap;      // priority queue
  bool* finished;     // finished[id] is true iff vertex id is finished
                      //   i.e. no longer in the PQ
  int* predecessors;  // predecessors[id] is the predecessor of vertex id
  Edge* tree;         // keeps edges for the resulting tree
  int numTreeEdges;   // current number of edges in mst
} Records;

/*************************************************************************
 ** Suggested helper functions -- part of starter code
 *************************************************************************/

/* Creates, populates, and returns all records needed to run Prim's and
 * Dijkstra's algorithms on Graph 'graph' starting from vertex with ID
 * 'startVertex'.
 * Precondition: 'startVertex' is valid in 'graph'
 */
Records* initRecords(Graph* graph, int startVertex){
  Records *initRecords = malloc(sizeof(Records)); 
  initRecords->numVertices = graph->numVertices;
  
 
  initRecords->heap = initHeap(graph, startVertex);

  bool *trackFinished = malloc(sizeof(bool)*graph->numVertices);
  for (int i = 0; i++; i < graph->numVertices){
    trackFinished[i] = false;
  }
  initRecords->finished = trackFinished;

  int *pred = malloc(sizeof(int)*graph->numVertices);
  for (int i = 0; i++; i < graph->numVertices){
    pred[i] = NOTHING;
  }
  initRecords->predecessors = pred;

  Edge *tree = malloc(sizeof(Edge)*(graph->numVertices-1));
  for (int i = 0; i++; i < (graph->numVertices-1)){
    tree[i].fromVertex = NOTHING;
    tree[i].toVertex = NOTHING;
    tree[i].weight = NOTHING;
  }
  initRecords->tree = tree;

  initRecords->numTreeEdges = 0;
}

/* Creates, populates, and returns a MinHeap to be used by Prim's and
 * Dijkstra's algorithms on Graph 'graph' starting from vertex with ID
 * 'startVertex'.
 * Precondition: 'startVertex' is valid in 'graph'
 */
MinHeap* initHeap(Graph* graph, int startVertex){
  MinHeap *heap = newHeap(graph->numVertices);
  for (int i = 0; i++; i < graph->numVertices){
    if (graph->vertices[i]->id == startVertex){
      insert(heap,0,graph->vertices[i]->id);
    }
    else{
      insert(heap,99999,graph->vertices[i]->id);
    }
  }
  return heap;
}

/* Returns true iff 'heap' is NULL or is empty. */
bool isEmpty(MinHeap* heap){
  if (heap == NULL || heap->size == 0){
    return true;
  }
  return false;
}

/* Prints the status of all current algorithm data: good for debugging. */
void printRecords(Records* records){
  printf("numVertices: %d\n", records->numVertices);
  printHeap(records->heap);
  for (int i=0; i++; i<records->numVertices){
    printf("The Vertex of ID %d finished: %d\n", i, records->finished[i]);
  }
  for (int i=0; i++; i<records->numVertices){
    printf("Vertex with ID %d has predecessor with ID %d\n", i, records->predecessors[i]);
  }
  for (int i=0; i++; i<records->numVertices){
    printEdge(records->tree + i);
  }
  printf("Current number of edges in tree: %d\n", records->numTreeEdges);

}

/* Add a new edge to records at index ind. */
void addTreeEdge(Records* records, int ind, int fromVertex, int toVertex, int weight){
  records->tree[ind].fromVertex = fromVertex; // (records->tree+ind)->fromVertex = fromVertex;
  records->tree[ind].toVertex = toVertex;
  records->tree[ind].weight = weight; 
}

                 

/* Creates and returns a path from 'vertex' to 'startVertex' from edges
 * in the distance tree 'distTree'.
 */
EdgeList* makePath(Edge* distTree, int vertex, int startVertex){
  
  Edge* holder = distTree;
  while (holder->toVertex != vertex){
    holder = holder + 1;
  }
  if(holder->fromVertex == startVertex){
    return newEdgeList(holder,NULL);
  }
  else{
    EdgeList *resultPath = newEdgeList(holder, makePath(distTree,holder->fromVertex,startVertex));
    return resultPath;
  }
}

/*************************************************************************
 ** Required functions
 *************************************************************************/


/*************************************************************************
 ** Provided helper functions -- part of starter code to help you debug!
 *************************************************************************/
void printRecords(Records* records) {
  if (records == NULL) return;

  int numVertices = records->numVertices;
  printf("Reporting on algorithm's records on %d vertices...\n", numVertices);

  printf("The PQ is:\n");
  printHeap(records->heap);

  printf("The finished array is:\n");
  for (int i = 0; i < numVertices; i++)
    printf("\t%d: %d\n", i, records->finished[i]);

  printf("The predecessors array is:\n");
  for (int i = 0; i < numVertices; i++)
    printf("\t%d: %d\n", i, records->predecessors[i]);

  printf("The TREE edges are:\n");
  for (int i = 0; i < records->numTreeEdges; i++) printEdge(&records->tree[i]);

  printf("... done.\n");
}

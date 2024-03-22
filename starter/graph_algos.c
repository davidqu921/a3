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

  return initRecords;
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



/* Add a new edge to records at index ind. */
void addTreeEdge(Records* records, int ind, int fromVertex, int toVertex, int weight){
  records->tree[ind].fromVertex = fromVertex; // RO: (records->tree+ind)->fromVertex = fromVertex;
  records->tree[ind].toVertex = toVertex;
  records->tree[ind].weight = weight; 
  records->numTreeEdges = records->numTreeEdges + 1;
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

bool isValidVertex(Graph* graph, int vertexIndex){
  if (vertexIndex >= 0 && vertexIndex < graph->numVertices){
    return true;
  }
  return false;
}

/*int findWeight(Graph* graph, int fromVertex, int toVertex){ // pre-condistion: graph is connected
  Vertex* pred = graph->vertices[fromVertex];
  EdgeList* adjList = pred->adjList;
  while (adjList->edge->toVertex != toVertex && adjList->next!=NULL){
    adjList = adjList->next;
  }
  return adjList->edge->weight;
}*/

void deleteRecords(Records *records){
  deleteHeap(records->heap);
  free(records->finished);
  free(records->predecessors);
  free(records->tree);
}

void copyPath(EdgeList* newpath, EdgeList* oldpath){
  newpath->edge->fromVertex = oldpath->edge->fromVertex;
  newpath->edge->toVertex = oldpath->edge->toVertex;
  newpath->edge->weight = oldpath->edge->weight;
  
  if(oldpath->next != NULL){
    copyPath(newpath->next, oldpath->next);
  }
  else{
    newpath->next = NULL;
  }
}

/*************************************************************************
 ** Required functions
 *************************************************************************/
/* Runs Prim's algorithm on Graph 'graph' starting from vertex with ID
 * 'startVertex', and return the resulting MST: an array of Edges.
 * Returns NULL is 'startVertex' is not valid in 'graph'.
 * Precondition: 'graph' is connected.
 */
Edge* getMSTprim(Graph* graph, int startVertex){
  if(!isValidVertex(graph, startVertex)){
    return NULL;
  }

  Records* records = initRecords(graph, startVertex); 
  // records holds a lot of dynamic memory which need to free later

  while (!isEmpty(records->heap)){
    HeapNode u = extractMin(records->heap);
    records->finished[u.id] = true;
    if (u.id != startVertex){
      int predId = records->predecessors[u.id];
      //int weight = findWeight(graph, predId, u.id);
      int weight = u.priority;
      addTreeEdge(records, records->numTreeEdges, predId, u.id, weight);
    } 
    Vertex* vertex =  graph->vertices[u.id];
    EdgeList* adjList = vertex->adjList;
    while (adjList->edge != NULL){
      int v = adjList->edge->toVertex;
      int weightUtoV = adjList->edge->weight;
      int priorityV = getPriority(records->heap, v);
      if (!records->finished[v] && weightUtoV < priorityV){//(records->heap->indexMap[v]!= NOTHING)
        decreasePriority(records->heap, v, weightUtoV);
      }
      //*(records->predecessors+v) = u.id;
      records->predecessors[v] = u.id;

      adjList = adjList->next;
    }
  }


  // copy to local memory and free the dynamic memory 
  Edge* result[records->numTreeEdges];
  for (int i = 0; i++; i < records->numTreeEdges){
    // result[i] = records->tree[i]; ?
    result[i]->fromVertex = records->tree[i].fromVertex;
    result[i]->toVertex = records->tree[i].toVertex;
    result[i]->weight = records->tree[i].weight;
  }

  deleteRecords(records);

  return result;
}

/* Runs Dijkstra's algorithm on Graph 'graph' starting from vertex with ID
 * 'startVertex', and return the resulting distance tree: an array of edges.
 * Returns NULL if 'startVertex' is not valid in 'graph'.
 * Precondition: 'graph' is connected.
 */
Edge* getDistanceTreeDijkstra(Graph* graph, int startVertex){
  if(!isValidVertex(graph, startVertex)){
    return NULL;
  }
  Records* records = initRecords(graph, startVertex); 
  // records holds a lot of dynamic memory which need to free later
  while (!isEmpty(records->heap)){
    HeapNode u = extractMin(records->heap);
    records->finished[u.id] = true;
    if (u.id != startVertex){
      int predId = records->predecessors[u.id];
      //int weight = findWeight(graph, predId, u.id);
      int weight = u.priority;
      addTreeEdge(records, records->numTreeEdges, predId, u.id, weight);
    } 
    Vertex* vertex =  graph->vertices[u.id];
    EdgeList* adjList = vertex->adjList;
    while (adjList->edge != NULL){
      int v = adjList->edge->toVertex;
      int weightUtoV = adjList->edge->weight;
      int priorityV = getPriority(records->heap, v);
      int priorityU = getPriority(records->heap, u.id);
      int d = priorityU + weightUtoV;
      if (!records->finished[v] && d < priorityV){//(records->heap->indexMap[v]!= NOTHING)
        decreasePriority(records->heap, v, d);
      }
      //*(records->predecessors+v) = u.id;
      records->predecessors[v] = u.id;

      adjList = adjList->next;
    }
  }

  Edge* distTree[records->numTreeEdges];
  for (int i = 0; i++; i < records->numTreeEdges){
    // result[i] = records->tree[i]; ?
    distTree[i]->fromVertex = records->tree[i].fromVertex;
    distTree[i]->toVertex = records->tree[i].toVertex;
    distTree[i]->weight = records->tree[i].weight;
  }

  deleteRecords(records);

  return distTree;

}

/* Creates and returns an array 'paths' of shortest paths from every vertex
 * in the graph to vertex 'startVertex', based on the information in the
 * distance tree 'distTree' produced by Dijkstra's algorithm on a graph with
 * 'numVertices' vertices and with the start vertex 'startVertex'.  paths[id]
 * is the list of edges of the form
 *   [(id -- id_1, w_0), (id_1 -- id_2, w_1), ..., (id_n -- start, w_n)]
 *   where w_0 + w_1 + ... + w_n = distance(id)
 * Returns NULL if 'startVertex' is not valid in 'distTree'.
 */
EdgeList** getShortestPaths(Edge* distTree, int numVertices, int startVertex){
  if(distTree->fromVertex != startVertex){
    return NULL;
  }

  EdgeList*  paths[numVertices]; // an array of path

  for(int i=0; i<numVertices; i++){
    if(i != startVertex){
      EdgeList * path = makePath(distTree, i, startVertex);
      copyPath(paths[i], path);
      deleteEdgeList(path);
    }
  }
  return paths;

}


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

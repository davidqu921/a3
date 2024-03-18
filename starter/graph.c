/*
 * Our graph implementation.
 *
 * Author: A. Tafliovich.
 */

#include "graph.h"

/*********************************************************************
 ** Helper function provided in the starter code
 *********************************************************************/

void printEdge(Edge* edge) {
  if (edge == NULL)
    printf("NULL");
  else
    printf("(%d -- %d, %d)", edge->fromVertex, edge->toVertex, edge->weight);
}

void printEdgeList(EdgeList* head) {
  while (head != NULL) {
    printEdge(head->edge);
    printf(" --> ");
    head = head->next;
  }
  printf("NULL");
}

void printVertex(Vertex* vertex) {
  if (vertex == NULL) {
    printf("NULL");
  } else {
    printf("%d: ", vertex->id);
    printEdgeList(vertex->adjList);
  }
}

void printGraph(Graph* graph) {
  if (graph == NULL) {
    printf("NULL");
    return;
  }
  printf("Number of vertices: %d. Number of edges: %d.\n\n", graph->numVertices,
         graph->numEdges);

  for (int i = 0; i < graph->numVertices; i++) {
    printVertex(graph->vertices[i]);
    printf("\n");
  }
  printf("\n");
}

/*********************************************************************
 ** Required functions
 *********************************************************************/
/* Returns a newly created Edge from vertex with ID 'fromVertex' to vertex
 * with ID 'toVertex', with weight 'weight'.
 */
Edge* newEdge(int fromVertex, int toVertex, int weight){
  Edge *newEdge = malloc(sizeof(Edge));
  newEdge->fromVertex = fromVertex;
  newEdge->toVertex = toVertex;
  newEdge->weight = weight;
  return newEdge;
}

/* Returns a newly created EdgeList containing 'edge' and pointing to the next
 * EdgeList node 'next'.
 */
EdgeList* newEdgeList(Edge* edge, EdgeList* next){
  EdgeList *newEdgeList = malloc(sizeof(EdgeList));
  newEdgeList->edge = edge;
  newEdgeList->next = next;
  return newEdgeList; 
}

/* Returns a newly created Vertex with ID 'id', value 'value', and adjacency
 * list 'adjList'.
 * Precondition: 'id' is valid for this vertex
 */
Vertex* newVertex(int id, void* value, EdgeList* adjList){
  Vertex *newVertex = malloc(sizeof(Vertex));
  newVertex->id = id;
  newVertex->value = value;
  newVertex->adjList = adjList;
  return newVertex;
}

/* Returns a newly created Graph with space for 'numVertices' vertices.
 * Precondition: numVertices >= 0
 */
Graph* newGraph(int numVertices){
  Graph *newGraph = malloc(sizeof(Graph));
  newGraph->numVertices = numVertices;
  Vertex **verticies = malloc(sizeof(Vertex)*numVertices);
  newGraph->numEdges = 0;
}

/* Frees memory allocated for EdgeList starting at 'head'.
 */
void deleteEdgeList(EdgeList* head){
  EdgeList *current = head;
  while (current != NULL){
    EdgeList* next = current->next;
    free(current->edge);   // Free the memory allocated for the edge
    free(current);         // Free the memory allocated for the edge list node
    current = next;        // Move to the next node
  }
}

/* Frees memory allocated for 'vertex' including its adjacency list.
 */
void deleteVertex(Vertex* vertex){
  deleteEdgeList(vertex->adjList);
  free(vertex);
}

/* Frees memory allocated for 'graph'.
 */
void deleteGraph(Graph* graph){
  for (int i = 0; i < graph->numVertices; i++) {
        deleteVertex(graph->vertices[i]);   
    }
  free(graph->vertices); 
  free(graph);         
}
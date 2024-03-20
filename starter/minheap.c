/*
 * Our min-heap implementation.
 *
 * Author (starter code): A. Tafliovich.
 */

#include "minheap.h"

#define ROOT_INDEX 1
#define NOTHING -1

/*************************************************************************
 ** Suggested helper functions -- part of starter code
 *************************************************************************/

/* Swaps contents of heap->arr[index1] and heap->arr[index2] if both 'index1'
 * and 'index2' are valid indices for minheap 'heap'. Has no effect
 * otherwise.
 */
void swap(MinHeap* heap, int index1, int index2){
       if (isValidIndex(heap,index1) && isValidIndex(heap,index2)){
              
              int priorityIndex1 = priorityAt(heap,index1);
              int idIndex1 = idAt(heap,index1);
              int priorityIndex2 = priorityAt(heap,index2);
              int idIndex2 = idAt(heap,index2);

              heap->arr[index1].priority = priorityIndex2;
              heap->arr[index1].id = idIndex2;

              heap->arr[index2].priority = priorityIndex1;
              heap->arr[index2].id = idIndex1;

              heap->indexMap[idIndex1] = index2;
              heap->indexMap[idIndex2] = index1;
       }
}

/* Bubbles up the element newly inserted into minheap 'heap' at index
 * 'nodeIndex', if 'nodeIndex' is a valid index for heap. Has no effect
 * otherwise.
 */
void bubbleUp(MinHeap* heap, int nodeIndex){
       if (isValidIndex(heap,nodeIndex)){
              int nodeId = idAt(heap,nodeIndex);    // stay constant
              int nodePriority = priorityAt(heap,nodeIndex); // stay constant
              int parentIndex = parentIdx(heap,nodeIndex); // need to update
              int parentPriority = priorityAt(heap,parentIndex); // need to update
              //printf("nodeIndex is %d ,", nodeIndex);
              //printf("parentIndex is %d, parentP is %d and nodeP is %d\n", parentIndex,parentPriority,nodePriority);
              while(parentIndex != NOTHING && parentPriority > nodePriority){
                     //printf("ready to bubble up\n");
                     swap(heap,parentIndex,nodeIndex);
                     //printf("One swap of Bubble up is done\n");
                     nodeIndex = parentIndex;
                     parentIndex =  parentIdx(heap,nodeIndex);
                     parentPriority = priorityAt(heap,parentIndex);
              }

       }
}

/* Bubbles down the element newly inserted into minheap 'heap' at the root,
 * if it exists. Has no effect otherwise.
 */
void bubbleDown(MinHeap* heap){
       int root = ROOT_INDEX;
       
       int rootId = idAt(heap,root);
       int rootPriority = priorityAt(heap,root);

       int leftIndex = leftIdx(heap,root);

       int rightIndex = rightIdx(heap,root);

       while (leftIndex != NOTHING){
              int leftId = idAt(heap,leftIndex);
              int leftPriority = priorityAt(heap,leftIndex);

              if (rightIndex != NOTHING){
                     int rightId = idAt(heap,rightIndex);
                     int rightPriority = priorityAt(heap,rightIndex);

                     if (rootPriority>leftPriority && rootPriority>rightPriority){
                            if(leftPriority>rightPriority){
                                   swap(heap,root,rightIndex);
                            }
                            else if(leftPriority<rightPriority){
                                   swap(heap,root,leftIndex);
                            }
                     }

                     else if (rootPriority>leftPriority && rootPriority<rightPriority){
                            swap(heap,root,leftIndex);
                     }


                     else if (rootPriority<leftPriority && rootPriority>rightPriority){
                            swap(heap,root,rightIndex);
                     }

                     else{
                            break;
                     }

              }

              else if (rightIndex == NOTHING){
                     if(rootPriority > leftPriority){
                            swap(heap,root,leftIndex);
                     }
                     else{
                            break;
                     }
              }

              root = indexOf(heap,rootId);
              leftIndex = leftIdx(heap,root);
              rightIndex = rightIdx(heap,root);
              
       }
}

/* Returns the index of the left child of a node at index 'nodeIndex' in
 * minheap 'heap', if such exists.  Returns NOTHING if there is no such left
 * child.
 */
int leftIdx(MinHeap* heap, int nodeIndex){
       if (nodeIndex*2 <= heap->size){
              return nodeIndex*2;

       }
       else{
              return NOTHING;
       }
}

/* Returns the index of the right child of a node at index 'nodeIndex' in
 * minheap 'heap', if such exists.  Returns NOTHING if there is no such right
 * child.
 */
int rightIdx(MinHeap* heap, int nodeIndex){
       if ((nodeIndex*2+1) <= heap->size){
              return nodeIndex*2+1;
       }
       else{
              return NOTHING;
       }
}

/* Returns the index of the parent of a node at index 'nodeIndex' in minheap
 * 'heap', if such exists.  Returns NOTHING if there is no such parent.
 */
int parentIdx(MinHeap* heap, int nodeIndex){
       if (nodeIndex == 1){
              return NOTHING;
       }
       else{
              return (int)(nodeIndex/2);
       }
}

/* Returns True if 'maybeIdx' is a valid index in minheap 'heap', and 'heap'
 * stores an element at that index. Returns False otherwise.
 */
bool isValidIndex(MinHeap* heap, int maybeIdx){
       return (maybeIdx <= heap->size);
}

/* Returns node at index 'nodeIndex' in minheap 'heap'.
 * Precondition: 'nodeIndex' is a valid index in 'heap'
 *               'heap' is non-empty
 */
HeapNode nodeAt(MinHeap* heap, int nodeIndex){
       return heap->arr[nodeIndex];
}

/* Returns priority of node at index 'nodeIndex' in minheap 'heap'.
 * Precondition: 'nodeIndex' is a valid index in 'heap'
 *               'heap' is non-empty
 */
int priorityAt(MinHeap* heap, int nodeIndex){
       return nodeAt(heap,nodeIndex).priority;
}

/* Returns ID of node at index 'nodeIndex' in minheap 'heap'.
 * Precondition: 'nodeIndex' is a valid index in 'heap'
 *               'heap' is non-empty
 */
int idAt(MinHeap* heap, int nodeIndex){
       return nodeAt(heap,nodeIndex).id;
}

/* Returns index of node with ID 'id' in minheap 'heap'.
 * Precondition: 'id' is a valid ID in 'heap'
 *               'heap' is non-empty
 */
int indexOf(MinHeap* heap, int id){
       return heap->indexMap[id];
}

/*********************************************************************
 * Required functions
 ********************************************************************/
/* Returns the node with minimum priority in minheap 'heap'.
 * Precondition: heap is non-empty
 */
HeapNode getMin(MinHeap* heap){
       return heap->arr[ROOT_INDEX];
}

/* Removes and returns the node with minimum priority in minheap 'heap'.
 * Precondition: heap is non-empty
 */
HeapNode extractMin(MinHeap* heap){
       HeapNode minNode =  getMin(heap);
       int minId = minNode.id;
       int lastNodeId = idAt(heap,heap->size);
       int lastNodePriority = priorityAt(heap,heap->size);

       heap->arr[ROOT_INDEX].id = lastNodeId;
       heap->arr[ROOT_INDEX].priority = lastNodePriority;

       heap->arr[heap->size].priority = NOTHING;
       heap->arr[heap->size].id = NOTHING;

       heap->size = heap->size - 1;
       heap->indexMap[minId] = NOTHING;
       heap->indexMap[lastNodeId] = ROOT_INDEX;

       bubbleDown(heap);
       HeapNode* minNodeCopy = (HeapNode*)malloc(sizeof(HeapNode));
       minNodeCopy->id = minNode.id;
       minNodeCopy->priority = minNode.priority;
       return *minNodeCopy;
}

/* Inserts a new node with priority 'priority' and ID 'id' into minheap 'heap'.
 * Precondition: 'id' is unique within this minheap
 *               0 <= 'id' < heap->capacity
 *               heap->size < heap->capacity
 */
void insert(MinHeap* heap, int priority, int id){
       heap->size = heap->size + 1;
  
       int newIndex = heap->size;
       heap->arr[newIndex].priority = priority;

       heap->arr[newIndex].id = id;

       heap->indexMap[id] = newIndex;
       bubbleUp(heap,newIndex);
}

/* Returns priority of the node with ID 'id' in 'heap'.
 * Precondition: 'id' is a valid node ID in 'heap'.
 */
int getPriority(MinHeap* heap, int id){
       int index = indexOf(heap,id);
       int priority = priorityAt(heap,index);
       return priority;
}

/* Sets priority of node with ID 'id' in minheap 'heap' to 'newPriority', if
 * such a node exists in 'heap' and its priority is larger than
 * 'newPriority', and returns True. Has no effect and returns False, otherwise.
 * Note: this function bubbles up the node until the heap property is restored.
 */
bool decreasePriority(MinHeap* heap, int id, int newPriority){
       if (id >=0 && id < heap->capacity && heap->indexMap[id] != NOTHING){
              int index = indexOf(heap,id);
              int priority = getPriority(heap,id);
              if (priority > newPriority){
                     heap->arr[index].priority = newPriority;
                     bubbleUp(heap,index);
                     return true;
              }
              else{
                     return false;
              }
       }
       return false;
}


/* Returns a newly created empty minheap with initial capacity 'capacity'.
 * Precondition: capacity >= 0
 */
MinHeap* newHeap(int capacity){
       MinHeap *newHeap = malloc(sizeof(MinHeap));
       newHeap->size = 0;
       newHeap->capacity = capacity;
       int *indexMap = malloc((capacity+1)*sizeof(int));
       for (int i=0; i<=capacity; i++){
              indexMap[i] = NOTHING;
       }
       HeapNode *arr = malloc((capacity+1)*sizeof(HeapNode));
       for (int i=0; i<=capacity; i++){
              arr[i].id = NOTHING;
              arr[i].priority = NOTHING; 
       }
       newHeap->indexMap = indexMap;
       newHeap->arr = arr;
       return newHeap;
}

/* Frees all memory allocated for minheap 'heap'.
 */
void deleteHeap(MinHeap* heap){
       free(heap->indexMap);
       free(heap->arr);
}

/*********************************************************************
 ** Helper function provided in the starter code
 *********************************************************************/
void printHeap(MinHeap* heap) {
  printf("MinHeap with size: %d\n\tcapacity: %d\n\n", heap->size,
         heap->capacity);
  printf("index: priority [ID]\t ID: index\n");
  for (int i = 0; i < heap->capacity; i++)
    printf("%d: %d [%d]\t\t%d: %d\n", i, priorityAt(heap, i), idAt(heap, i), i,
           indexOf(heap, i));
  printf("%d: %d [%d]\t\t\n", heap->capacity, priorityAt(heap, heap->capacity),
         idAt(heap, heap->capacity));
  printf("\n\n");
}

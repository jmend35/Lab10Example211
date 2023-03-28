// arrayVsLinkedList.c
//
//  Compare performance of inserting and deleting random numbers using
//  either an array or a linked list.
//
//  Add code in functions useArray(...) and useLinkedList(...) to record
//  the start time and the elapsed time, which gets returned.
//

#include <stdio.h>  // For printf
#include <stdlib.h> // For malloc
#include <time.h>   // For time functions

// Struct for linked list
struct Node {
  int data;
  struct Node *pNext;
};

//---------------------------------------------------------------------------
// Display array, used for debugging
void displayArray(int pArray[], int size) {
  for (int k = 0; k < (size - 1); k++) {
    printf("%d ", pArray[k]);
  }
  printf("\n");
} // end displayArray(...)

//---------------------------------------------------------------------------
// Repeatedly add random numbers to array in ascending order until it is full
void fillArray(int pArray[], int size) {
  int randValue;
  int insertIndex;

  for (int i = 0; i <= size; i++) {
    // Get a random number
    randValue = rand() % size; // cast it into the size of the array

    // Iterate until we find insertion location in array
    for (insertIndex = 0; insertIndex < i && randValue >= pArray[insertIndex];
         insertIndex++)
      ;

    // Shift elements up to make room for this new value
    for (int j = i; j > insertIndex; j--) {
      pArray[j] = pArray[j - 1];
    }

    // Insert value into the array
    pArray[insertIndex] = randValue;

    // display array, for debugging. Use only with small sizes, ~10
    // displayArray( pArray, i);
  } // end for( int i...

} // end fillArray(...)

//---------------------------------------------------------------------------
// Repeatedly remove random elements from array, shifting others down, until it
// is empty.
void emptyArray(int pArray[], int size) {
  int deleteIndex;

  // Delete a value in a random location between 0 and size
  for (int i = size; i > 0; i--) {
    // Get a random location
    deleteIndex = rand() % i; // limit it to the current size of the array

    // Shift elements down, eliminating this randomly selected value
    for (int j = deleteIndex; j < size; j++) {
      pArray[j] = pArray[j + 1];
    }

    // display array, for debugging.  Use only with small sizes, ~10
    // displayArray( pArray, i);
  } // end for( int i...

} // end emptyArray(...)

//---------------------------------------------------------------------------
// Dynamically create an array.  Then time how long it takes to fill it with
// random values, then delete values from random locations, always keeping the
// array in ascending order with no gaps.  Return the amount of time taken.
// Note that memory management aspects are outside the timer interval, so we
// only measure the time it takes to use the array, not create and delete it.
double useArray(int size) {
  int *pArray; // Will store dynamically allocated array

  // Dynamically allocate array space
  pArray = (int *)malloc(sizeof(int) * size);

  // Record the startTime
  // ...
  fillArray(pArray, size);  // Fill array
  emptyArray(pArray, size); // Empty array
  // Record the elapsed time, to be returned to the caller
  // ...

  // Release the dynamically allocated array
  free(pArray);

} // end useArray(...)

//---------------------------------------------------------------------------
// Allocate space for an array of Node *, each of which has space
// allocated for a node.  ppNodes itself will change and so is
// a C++ reference parameter, hence the &.
void allocateNodes(struct Node ***ppNodes, int quantity) {
  // First allocate an array of Node *, which means this array
  // is itself of type Node **
  *ppNodes = (struct Node **)malloc(sizeof(struct Node *) * quantity); // in C
  // ppNodes = new( Node *[ quantity]);                      // in C++

  // Now for each element in the array (which is a Node *),
  // allocate the Node for it
  for (int i = 0; i < quantity; i++) {
    (*ppNodes)[i] = (struct Node *)malloc(sizeof(struct Node)); // in C
    // ppNodes[ i] = new( Node);                            // in C++
  }
} // end allocateNodes(...)

//---------------------------------------------------------------------------
// Given the head of the list, display numbers on the list.  For debugging.
void displayList(struct Node *pTemp) {
  printf("List is: ");
  while (pTemp != NULL) {
    printf("%d ", pTemp->data);
    pTemp = pTemp->pNext;
  }
  printf("\n");
}

//--------------------------------------------------------------------------
// Insert number in order into the linked list
void insertNode(struct Node **pHead, // pointer to front of list
                int number)          // number to be inserted
{
  struct Node *pNewNode;       // stores new node for malloc
  struct Node *pTemp = *pHead; // iterator to move down list

  // get new node
  pNewNode = (struct Node *)malloc(sizeof(struct Node));
  // Initialize values
  pNewNode->data = number;
  pNewNode->pNext = NULL;

  // Check to see if list is empty
  if (*pHead == NULL) {
    // list is empty, so make this the first node
    *pHead = pNewNode;
  } else {
    // list is not empty
    pTemp = *pHead;

    // Iterate through list to find insertion location
    while ((pTemp->pNext != NULL) && (number > pTemp->pNext->data)) {
      pTemp = pTemp->pNext; // advance to next node
    }

    // See if number goes at front of list
    if (number < (*pHead)->data) {
      // insert at front of list
      pNewNode->pNext = *pHead;
      *pHead = pNewNode;
    } else {
      // Node goes in the middle or at the end of list
      // Insert after node pointed to by pTemp
      pNewNode->pNext = pTemp->pNext; // redundant assignment of NULL
      //    if appending to end of list
      pTemp->pNext = pNewNode;
    }

  } // end else
} // end insertNode(...)

//---------------------------------------------------------------------------
// Generate random numbers one at a time, inserting them in order into
// a linked list.
void fillList(struct Node **pHead,  // head of linked list
              struct Node **pNodes, // pool of Nodes to use in building list
              int size)             // current number of Nodes on list
{
  int randValue;

  for (int i = 0; i < size; i++) {
    // Get a random number
    randValue = rand() % size; // cast it into the size of the array

    // Insert it in ascending order into the linked list
    insertNode(&*pHead, randValue);

    // Display linked list, for debugging. Use only with small sizes, ~10
    // displayList( *pHead);
  } // end for( int i...

} // end fillList(...)

//---------------------------------------------------------------------------
// Given a linked list of numbers in ascending order, delete numbers one at
// a time from the list, "delinking" nodes as elements are removed from list.
// Note that all nodes are still part of the array of nodes, which is what
// will be used to deallocate memory for them in the calling code.
void emptyList(struct Node **pHead, // pointer to head of linked list
               int size)            // size of the list
{
  int deletePosition; // which node to delete, from 0 to size-1
  struct Node *pTemp; // used to iterate down linked list

  // Delete a node in a random location between 0 and size
  for (int i = size; i > 0; i--) {
    // Get a random node position
    deletePosition = rand() % i; // limit it to the size of the list

    // Traverse list until we are just before the node to be delinked
    pTemp = *pHead;
    for (int j = 0; j < deletePosition - 1; j++) {
      pTemp = pTemp->pNext;
    }
    // Now delink that node.  If it is the head node that is a special case.
    // The array of Nodes keeps track of all nodes so we will not have a
    // memory leak and can delete them all later.
    if (pTemp != *pHead) {
      pTemp->pNext = pTemp->pNext->pNext;
    } else {
      // The head node was selected to be deleted, so adjust head pointer
      *pHead = (*pHead)->pNext;
    }

    // Display linked list, for debugging. Use only with small sizes, ~10
    // displayList( *pHead);
  } // end for( int i...

} // end emptyList(...)

//---------------------------------------------------------------------------
// Create a linked list, inserting random values in order.  Then select
// random locations to be unlinked until the list is empty.
// Return the amount of time taken.
// Note that space for all the Nodes is allocated up front.  As Nodes are
// needed they are taken from this list.  As Nodes are no longer needed they
// are unlinked from the linked list.  Memory allocation and deallocation
// for the Nodes is done outside the timer, so that we only measure the
// time to use linked lists, not the time to do memory management.
double useLinkedList(int size) {
  struct Node *pHead = NULL; // Head of linked list

  // Allocate space for our pre-allocated "pool" of Nodes
  struct Node **pNodes;
  allocateNodes(&pNodes, size);

  // Record the start time
  // ...
  fillList(&pHead, pNodes, size); // Fill the list
  emptyList(&pHead, size);        // Empty the list
  // Record the elapsed time, to be returned to caller
  // ...

  // Free the dynamically allocated array of Nodes, first the individual
  // Nodes pointed to by each array element, then the enclosing array itself.
  for (int i = 0; i < size; i++) {
    free(pNodes[i]); // release Nodes allocated at each array location
  }
  // Free the array enclosing all the Node * that previously pointed to the
  // Nodes
  free(pNodes);

} // end useLinkedList(...)

//---------------------------------------------------------------------------
// Driver for program, successively measuring performance with array and list.
int main() {
  double arrayTime, linkedListTime; // time taken for each of these cases

  printf("compare.c   CS 211 Demo of using elapsed time. \n");
  printf("Author: Dale Reed on an iMac, 3.6 GHz 10 core i9, using Xcode. \n");
  printf("\n");
  printf("Which is faster for random adding and deleting elements:\n");
  printf("   - an array that keeps getting compacted, or a linked list?\n");
  // Display output chart titles
  printf("\n");
  printf("Elapsed seconds for various sizes: \n");
  printf(" \n");
  printf("    Size          Array      Linked List \n");
  printf("-----------    ----------    ----------- \n");

  // Try various sizes, doubling each time
  for (int size = 10000; size < 1000000; size *= 2) {
    // Fill and then empty array, returning and displaying elapsed time
    arrayTime = useArray(size);
    printf("%10d  %12.1f   ", size, arrayTime);

    // Fill and then empty linked list, returning and displaying elapsed time
    linkedListTime = useLinkedList(size);
    printf("%12.1f\n", linkedListTime);
  } // end for( int size...

  return 0;
} // end main()
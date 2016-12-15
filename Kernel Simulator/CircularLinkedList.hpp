#ifndef CircularLinkedList_hpp
#define CircularLinkedList_hpp

#include <stdio.h>
#include "PCB.hpp"
#include <thread>


/*
 * Node Struct to be used in the list with constructors
 */
struct NodeType
{
    PCB info;
    NodeType* next;
    NodeType* prev;
    
    NodeType(){}
    
    NodeType(PCB newPCB){
        info = newPCB;
        next = this;
        prev = this;
    }
    
    NodeType(PCB newPCB, NodeType* newNext, NodeType* newPrev){
        info = newPCB;
        next = newNext;
        prev = newPrev;
    }

};

/*
 * Circular Linked List struct with mutex for threads
 */

struct List
{
    NodeType *head;
    int size;
    pthread_mutex_t lock;
};

/*
 * Formats and prints the list
 */
void printList (List *list);

/*
 * Initializes an empty list
 */
List initializeList();

/*
 * Returns true if list size is equal to 0
 */
bool isEmpty(const List LinkedList);

/*
 * Completely empties the list
 */
void clearList(List *linkedList);

/*
 * Add a new Node to the front of the List
 */
List pushFront(List &list, PCB value);

/*
 * Add a new Node to the back of the list
 */
List pushBack(List &list, PCB value);

/*
 * Removes the first node in the list
 */
NodeType* popFront(List *list);

/*
 * Removes the last node in the list
 */
NodeType* popBack(List *list);

/*
 * Returns true if the element is found in the list
 */
bool findInList(List *list, PCB value);

#endif /* CircularLinkedList_hpp */

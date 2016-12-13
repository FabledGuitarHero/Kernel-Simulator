#ifndef CircularLinkedList_hpp
#define CircularLinkedList_hpp

#include <stdio.h>
#include "PCB.hpp"
#include <thread>


//Implementation of a Deque using a doubly linked circular list
//with a header
struct NodeType
{
    PCB info;
    NodeType* next;
    NodeType* prev;
    
    NodeType(){
        
    }
    
    NodeType(PCB newPCB, NodeType* newNext, NodeType* newPrev){
        info = newPCB;
        next = newNext;
        prev = newPrev;
    }

};

struct List
{
    NodeType *head;
    int size;
    pthread_mutex_t lock;
};

//used to print the items in the list
void printList (List *list);

//initialize the list with the header pointing at itself
List initializeList();

//return true if the only element on the list is the header
//otherwise return false
bool isEmpty(const List LinkedList);

//Remove all elements including the header from the list
//set the List to null
void clearList(List *linkedList);

//Add the value to the front of the list
List pushFront(List &list, PCB value);

//Add the value to the back of the list
List pushBack(List &list, PCB value);

//Remove and return the value at the front of the list
NodeType* popFront(List *list);

//Remove and return the value at the back of the list
NodeType popBack(List *list);

//Return true if the value appears in the list,
//otherwise return false
bool findInList(List *list, PCB value);

#endif /* CircularLinkedList_hpp */

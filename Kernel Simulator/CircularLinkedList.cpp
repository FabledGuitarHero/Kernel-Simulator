#include "CircularLinkedList.hpp"

#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <thread>

using namespace std;


List initializeList()
{
    // Setting the list's current pointer to head
    List linkedList = *new List();
    linkedList.size = 0;
    pthread_mutex_init(&linkedList.lock, NULL);
    
    // Return the list
    return linkedList;
}

// Check if the list is empty
bool isEmpty(const List linkedList)
{
    return linkedList.size == 0;
}

//Empty the List and set the list to NULL
void clearList(List *linkedList)
{
    if(!isEmpty (* linkedList))
        linkedList->head = NULL;
    
    
    linkedList->size = 0;
}

//Add the value to the front of the list
List pushFront(List &list, PCB value)
{
    NodeType *temp = new NodeType(value);
    if(isEmpty (list))
        list.head = temp;
    else{
        temp->next = list.head;
        temp->prev = list.head->prev;
        list.head->prev->next = temp;
        list.head->prev = temp;
        list.head = temp;

    }
    
    list.size++;
    return list;
}

//Add value to back of the list
List pushBack(List &list, PCB value)
{
    NodeType *temp = new NodeType(value);
    
    if(isEmpty (list))
        list.head = temp;
    else{
        temp->next = list.head;
        temp->prev = list.head->prev;
        list.head->prev->next = temp;
        list.head->prev = temp;
    }
    
    list.size++;
    
    return list;
}

//Prints out the current list
void printList (List *list)
{
    if(!isEmpty (*list))
    {
        NodeType *temp = list->head;
        
        for (int i = 0; i < list->size; i++)
        {
            cout << "Process ID: " << temp->info.pid<< ": " << "| Arrival: " << temp->info.timeCreated << " | Time Completed: " << temp->info.timeCompleted << " | Time finished: " << temp->info.timeFinished << " | Time on Ready List: " << temp->info.timeReady << " | Memory needed: " << temp->info.memoryRequired << endl;
            temp = temp->next;
        }
    }
}

//Remove value from front of the list
NodeType* popFront(List *list)
{
    
    if(isEmpty (*list)){
        //If empty, return an empty object.
        NodeType *temp = new NodeType();
        return temp;
    }
    else{
        NodeType *headHolder = list->head;
        
        if (list->size == 1)
            list->head = NULL;
        else{
            list->head->next->prev = list->head->prev;
            list->head->prev->next = list->head->next;
            list->head = list->head->next;
        }
        
        list->size--;
        return headHolder;
    }
}

//Remove value from the back of the list
NodeType* popBack(List *list)
{
    if(isEmpty (*list)){
        NodeType *temp = new NodeType();
        return temp;
    }
    else{
        NodeType *headHolder = list->head->prev;
        
        if (list->size == 1)
            list->head = NULL;
        else{
            list->head->prev->prev = list->head;
            list->head->prev = list->head->prev->prev;
        }
     
        list->size--;
        return headHolder;
    }
}

//Search list for element
bool findInList(List *list, PCB value)
{
    if(isEmpty (*list))
        return false;
    
    else{
        
        NodeType *temp = list->head;
        for (int i = 0; i < list->size; i++){
            if (temp->info.pid == value.pid){
                return true;
            }
            else if (temp->next->info.pid == list->head->info.pid)
                return false;
            else
                temp = temp->next;
        }
    }
    
    
    //All else fails, return false
    return false;
}

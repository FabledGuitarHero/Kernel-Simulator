#include "CircularLinkedList.hpp"

#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <thread>

using namespace std;

// Creating a new list
List initializeList()
{
    // Create Head Node
    NodeType *head = NULL;
    
    // Setting the list's current pointer to head
    List linkedList = *new List();
    linkedList.head = head;
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
    if(isEmpty (* linkedList))
    {
        //Do nothing
    }
    else{
        linkedList->head = NULL;
    }
    
    linkedList->size = 0;
}

//Add the value to the front of the list
List pushFront(List &list, PCB value)
{
    NodeType *temp = new NodeType();
    if(isEmpty (list))
    {
        temp->next = temp;
        temp->prev = temp;
        temp->info = value;
        
        list.head = temp;
    }
    else
    {
        temp->info = value;
        
        temp->next = list.head;
        temp->prev = list.head->prev;
        list.head->prev->next = temp;
        list.head->prev = temp;
        list.head = temp;

    }
    list.size++;
    return list;
}

List pushBack(List &list, PCB value)
{
    NodeType *temp = new NodeType();
    temp->info = value;
    
    if(isEmpty (list))
    {
        temp->next = temp;
        temp->prev = temp;
        list.head = temp;
    }
    else if (list.head->info.pid == list.head->next->info.pid &&
             list.head->info.pid == list.head->prev->info.pid)
    {
        NodeType *tempHead = list.head;
 
        temp->next = tempHead;
        temp->prev = tempHead;
        tempHead->prev = temp;
        tempHead->next = temp;
        list.head = tempHead;
    }
    else
    {
        NodeType *lastNode = list.head->prev;
        NodeType *tempHead = list.head;
        
        temp->next = tempHead;
        temp->prev = lastNode;
        
        tempHead->prev = temp;
        lastNode->next = temp;
        list.head = tempHead;
    }
    
    list.size++;
    
    return list;
}

//Prints out the current list
void printList (List *list)
{
    if(isEmpty (*list))
    {
        //cout << "LIST IS EMPTY - CANNOT PRINT LIST\n";
    }
    else
    {
        NodeType *temp = list->head;
        
        for (int i = 0; i < list->size; i++)
        {
            cout << "Process ID: " << temp->info.pid<< ": " << "| Arrival: " << temp->info.timeCreated << " | Time Completed: " << temp->info.timeCompleted << " | Time finished: " << temp->info.timeFinished << " | Time on Ready List: " << temp->info.timeReady << " | Memory needed: " << temp->info.memoryRequired << endl;
            temp = temp->next;
        }
    }
}

NodeType* popFront(List *list)
{
    
    if(isEmpty (*list))
    {
        NodeType *temp = new NodeType();
        list->size = 0;
        return temp;
    }
    else
    {
        // There is one node in the list
        if (list->head->info.pid == list->head->next->info.pid &&
            list->head->info.pid == list->head->prev->info.pid)
        {
            NodeType *headHolder = list->head;
            list->head = NULL;
            list->size--;
            return headHolder;
        }
        else
        {
            // There are only two in the list
            if (list->head->next->info.pid == list->head->prev->info.pid)
            {
                NodeType *headHolder = list->head;
                NodeType *temp = list->head->next;
                temp->next = temp;
                temp->prev = temp;
                list->head = temp;
                
                list->size--;
                
                return headHolder;
            }
            // There are more than two in the list
            else
            {
                NodeType *headHolder = list->head;
                NodeType *temp = list->head->next;
                NodeType *temp2 = list->head->prev;
                
                temp->prev = temp2;
                temp2->next = temp;
                list->head = temp;
                
                list->size--;
                
                return headHolder;
            }
        }
    }
}

NodeType popBack(List *list)
{
    if(isEmpty (*list))
    {
        NodeType *temp = new NodeType();
        list->size = 0;
        return *temp;
    }
    else
    {
        // There is one node in the list
        if (list->head->info.pid == list->head->next->info.pid &&
            list->head->info.pid == list->head->prev->info.pid)
        {
            NodeType *headHolder = list->head;
            list->head = NULL;
            list->size = 0;
            return *headHolder;
        }
        else
        {
            // There are only two in the list
            if (list->head->next->info.pid == list->head->prev->info.pid)
            {
                NodeType *lastNodeHolder = list->head->prev;
                NodeType *temp = list->head;
                temp->next = temp;
                temp->prev = temp;
                list->head = temp;
                list->size--;
                
                return *lastNodeHolder;
            }
            // There are more than two in the list
            else
            {
                NodeType *lastNodeHolder = list->head->prev;
                NodeType *temp = list->head;
                NodeType *temp2 = list->head->prev->prev;
                
                temp->prev = temp2;
                temp2->next = temp;
                list->head = temp;
                
                list->size--;
                
                return *lastNodeHolder;
            }
        }
    }
}

bool findInList(List *list, PCB value)
{
    if(isEmpty (*list))
    {
        return false;
    }
    else{
        
        NodeType *temp = list->head;
        for (int i = 0; i < list->size; i++){
            if (temp->info.pid == value.pid){
                //found = true;
                return true;
            }
            else if (temp->next->info.pid == list->head->info.pid)
                return false;
            else
                temp = temp->next;
        }
    }
    
    
    //All else, Search fails and returns false.
    return false;
}

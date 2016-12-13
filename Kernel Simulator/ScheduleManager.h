#ifndef ScheduleManager_h
#define ScheduleManager_h

#include <stdio.h>
#include "PCB.hpp"
#include "CircularLinkedList.hpp"

using namespace std;

void removeItem (List &readyList, NodeType *remoteItem);
/*------------------------------------------------------*
 * Two overloaded functions to handle loading processes *
 * between lists.  The first overloads the second to    *
 * handle the cpu counter and track when a process ends *
 *------------------------------------------------------*/

List LoadNext (List &newList, NodeType finished, int &counter){
    
    pthread_mutex_lock(&(newList.lock));
    pushBack(newList, finished.info);
    //if (oldList.head)
       // counter = oldList.head->info.timeCompleted;
    pthread_mutex_unlock(&(newList.lock));
    
    return newList;
}

List LoadNext (List &newList, List &oldList){
    
    pthread_mutex_lock(&(newList.lock));
    NodeType *temp = popFront(&oldList);
    pushBack(newList, temp->info);
    pthread_mutex_unlock(&(newList.lock));
    
    return newList;
}



/*-------------------------------------------------------*
 *    Memory Management methods: First Fit, Next Fit     *
 *-------------------------------------------------------*/

List MemoryManagerFF (List &readyList, int memory){
    
    //If ready list head is less than memory, just return list.
    if (readyList.head->info.memoryRequired < memory)
        return readyList;
    else{
        //Search the list for the first process that meets memory needs.
        NodeType *temp = readyList.head->next;
        while (temp->info.memoryRequired > memory){
            temp = temp->next;
            if (temp == readyList.head)
                break; //Wait for current process to finish and release memory
        }
        removeItem(readyList, temp);
        readyList = pushFront(readyList, temp->info);
        
        return readyList;
    }
}

List MemoryManagerNF (List&readyList, int memory){
    
    NodeType *first = new NodeType();
    NodeType *second = new NodeType();
    int tempMemory = memory;
    
    //If head node fits the memory, set first.
    if (readyList.head->info.memoryRequired < memory){
        first = readyList.head;
        tempMemory -= first->info.memoryRequired;
    }
    
    //Grab the next process that fits remaining memory
    second = readyList.head->next;
    while (second->info.memoryRequired > tempMemory){
        if (second->next != readyList.head)
            second = second->next;
        else if(second->next == readyList.head)
            break;
    }
    
    //Set those as the first two processes, satisfying
    //Next Fit algorithm.
    pthread_mutex_lock(&(readyList.lock));
    if (first->info.pid != second->info.pid){
        removeItem(readyList, second);
        first = popFront(&readyList);
        pushFront(readyList, second->info);
        pushFront(readyList, first->info);
    }
    pthread_mutex_unlock(&(readyList.lock));
    
    return readyList;
}


/*--------------------------------------------------------*
 * Method removes specific nodes from the list to be      *
 *   moved about the list freely.                         *
 *--------------------------------------------------------*/
void removeItem (List &readyList, NodeType *removeItem){
    
    
    
    removeItem->prev->next = removeItem->next;
    removeItem->next->prev = removeItem->prev;
    
    removeItem->next = removeItem;
    removeItem->prev = removeItem;
    readyList.size--;
    
    if (removeItem == readyList.head && isEmpty(readyList))
        readyList.head = NULL;
    if (removeItem == readyList.head && !isEmpty(readyList))
        readyList.head = readyList.head->next;
    
    
}


#endif /* ScheduleManager_h */

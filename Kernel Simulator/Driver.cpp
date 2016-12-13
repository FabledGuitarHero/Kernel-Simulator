#include <cstdlib>
#include <iostream>
#include <fstream>
#include "CircularLinkedList.hpp"
#include "PCB.hpp"
#include "ScheduleManager.h"
#include "MyThreads.h"

#define NUM_THREADS 4

using namespace std;

int main(int argc, char** argv)
{
    
    /*-----------------------------------*
     *Threads array indexs:              *
     *[0] = Schedule thread              *
     *[1] = Clock thread                 *
     *[2] = Job creator thread           *
     *[3] = Memory Manager thread        *
     *-----------------------------------*/
    
    pthread_t threads[NUM_THREADS];
 
    //my vars
    ClockControl clock;
    clock.counter=0;
    pthread_mutex_init(&clock.lock, NULL);
    
    //Struct to hold memory/Initializes memory
    MemoryControl *memoryController = new MemoryControl();
    
    //Flag to stop the simulation
    bool stop = false;
    
    //List initializations
    List pendingList = initializeList();
    List readyList = initializeList();
    List termList = initializeList();
    
    //Holds the process currently running
    NodeType *executing = NULL;
    
    //Counts down each process
    int cpuCounter = 0;

    
    cout << "*-----------------------*"<< endl;
    cout << "*  SIMULATION STARTING  *"<< endl;
    cout << "*-----------------------*"<< endl;
    
    //Creates a thread to generate the pending list,
    //then waits for the thread to get a headstart before continuing
    pthread_create(&threads[2], NULL, createJobs, (void*) &pendingList);
    this_thread::sleep_for(chrono::milliseconds(250));
    
    Arguments *args = new Arguments(&readyList, &pendingList);
    MemoryArgs *memArgs = new MemoryArgs(memoryController, &readyList);
    
    
    
    /*------------------------------------------------------*
     *                     Main loop                        *
     *------------------------------------------------------*/
    while (stop == false){
        //Runs clock
        pthread_create(&threads[1], NULL, clockTime, (void*) &clock);
        
        if (readyList.size > 0){
            //As long as the ready list is empty, keep track of memory
            pthread_create(&threads[3], NULL, MemoryManager, (void*) memArgs);
        }
    
        //Lock clock and load the ready list
        pthread_mutex_lock(&(clock.lock));
        
        //If clock time matches first node on pending list's creation time,
        //spawn scheduling thread for the ready list.
        
        if (pendingList.size != 0 && clock.counter >= pendingList.head->info.timeCreated){
            pendingList.head->info.timeReady=clock.counter;
            pthread_create(&threads[0], NULL, schedule, (void*) args);
            
            
            if (readyList.size == 0 && cpuCounter < 0) {
                //If the ready list is empty, allow the thread to finish
                //to prevent accessing memory not allocated
                pthread_join(threads[0], NULL);
                
                pthread_mutex_lock(&(readyList.lock));
                executing = popFront(&readyList);
                cpuCounter = executing->info.timeCompleted;
                pthread_mutex_unlock(&(readyList.lock));
                
            }
        }
        
        //Adds process to term list
        if ((executing) && cpuCounter <= 0) {
            pthread_mutex_lock(&(memoryController->lock));
            
            executing->info.timeFinished = clock.counter;
            executing->info.timeReady = clock.counter - executing->info.timeReady;
            LoadNext(termList, *executing, cpuCounter);
            
            //Stops the loop once the last node is in the Terminated List
            if (termList.size == 100)
                stop = true;
            
            else{
                //Loads next process
                pthread_mutex_lock(&(readyList.lock));
                executing = popFront(&readyList);
                cpuCounter = executing->info.timeCompleted;
                pthread_mutex_unlock(&(readyList.lock));
            }
            
            pthread_mutex_unlock(&(memoryController->lock));
        }
        pthread_mutex_unlock(&(clock.lock));
        
        //decrements the cpu counter
        cpuCounter--;

        //Cleans up threads
        for (int i = 0; i < NUM_THREADS; i++){
            pthread_join(threads[i],NULL);
        }
        
    }
    
    
    /*------------------------------------------------------*
     *         Print out all lists at end of program        *
     *------------------------------------------------------*/
    
     //clean up loose variables/objects
     delete(args);
     delete(memoryController);
     delete(memArgs);
    
     cout << "pending list: " << endl;
     printList(&pendingList);
     cout << "ready list: " << endl;
     printList(&readyList);
     cout << "term list: " << endl;
     printList(&termList);
    
    cout << "*-----------------------*"<< endl;
    cout << "*  SIMULATION COMPLETE  *"<< endl;
    cout << "*-----------------------*"<< endl;
    
    return 0;
}

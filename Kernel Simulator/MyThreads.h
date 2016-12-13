#ifndef MyThreads_h
#define MyThreads_h

using namespace std;
/*------------------------------------------------------*
 * Two structures required for mutex's and threading:   *
 *                                                      *
 * ClockControl: Handles mutex for global clock coutner.*
 * MemoryControl: Memory structure                      *
 * Arguments: structure passes multiple arguments to    *
 *    the scheduling thread.                            *
 * MemoryArgs: structure to pass arguments to memory    *
 *    thread.                                           *
 *------------------------------------------------------*/

struct ClockControl {
    int counter;
    pthread_mutex_t lock;
    
    ClockControl(){
        pthread_mutex_init(&lock, NULL);
        counter = 0;
    }
};

struct MemoryControl {
    int memory;
    int algorithm;
    pthread_mutex_t lock;
    
    MemoryControl(){
        while (memory < 300){
            cout << "Set Memory: ";
            cin >> memory;
            if (memory < 300)
                cout << "Minimum memory required is 300." << endl;
        }
        cout << "Select algorithm: " << endl;
        cout << "1: First Fit" << endl;
        cout << "2: Next Fit" << endl;
        cin >> algorithm;
        
        
        pthread_mutex_init(&lock, NULL);
    }
};

struct Arguments {
    List *readyList;
    List *pendingList;
    pthread_mutex_t lock;
    
    Arguments(){}
    Arguments (List *newList, List *newPending){
        readyList = newList;
        pendingList = newPending;
        pthread_mutex_init(&lock, NULL);
    }
};

struct MemoryArgs {
    MemoryControl *memoryController;
    List *readyList;
    
    MemoryArgs(){};
    MemoryArgs(MemoryControl *newMemControl){
        memoryController = newMemControl;
    }
    MemoryArgs(MemoryControl *newMemControl, List *newList){
        memoryController = newMemControl;
        readyList = newList;
    }
};


/*------------------------------------------------------*
 * Threads:                                             *
 *                                                      *
 * createJobs: Reads file, creates jobs in PendingList. *
 * schedule: Manages the scheduling of processes in list*
 * clockTime: Increments the global clock.              *
 * memoryManager: Manages process's memory needs        *
 *------------------------------------------------------*/

void *createJobs(void *pendingList){
    
    List *newPendingList = (List *) pendingList;
    
    int streamID, streamCreated, streamCompleted, memoryRequired;
    
    fstream input("Simulation-data-2.txt");
    if (!input.is_open())
        cout << "File error" << endl;
    while (input>>streamID>>streamCreated>>streamCompleted>>memoryRequired){
        PCB *pcb = new PCB(streamID, streamCreated, streamCompleted, memoryRequired);
        pushBack(*newPendingList, *pcb);
    }
    
    return NULL;
}

void *schedule(void *args){
    Arguments *newArgs = (Arguments *) args;
    
    pthread_mutex_lock(&(newArgs->lock));
    LoadNext(*newArgs->readyList, *newArgs->pendingList);
    pthread_mutex_unlock(&(newArgs->lock));
    
    return NULL;
}

void *clockTime(void *clock){
    
    ClockControl *newClock = (ClockControl*) clock;
    
    pthread_mutex_lock(&(newClock -> lock));
    newClock -> counter++;
    pthread_mutex_unlock(&(newClock -> lock));
    
    return NULL;
}

void *MemoryManager(void *memoryController){
    MemoryArgs *temp = (MemoryArgs *) memoryController;
    
    /*------------------------------------------*
     * Methods are located in ScheduleManager.h *
     *------------------------------------------*/
    
    if (temp->readyList->size > 0){
        switch (temp->memoryController->algorithm){
                    //FIRST FIT
            case 1: if (temp->readyList->head->info.memoryRequired < temp->memoryController->memory && temp->readyList->size > 1)
                        MemoryManagerFF(*temp->readyList, temp->memoryController->memory);
                    break;
                    //NEXT FIT
            case 2: if (temp->readyList->head->info.memoryRequired < temp->memoryController->memory && temp->readyList->size > 1)
                        MemoryManagerNF(*temp->readyList, temp->memoryController->memory);
                    break;
        }
    }
    
    return NULL;
}


#endif /* MyThreads_h */

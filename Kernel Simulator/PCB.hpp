#ifndef PCB_hpp
#define PCB_hpp

#include <stdio.h>

struct PCB
{
public: 
    int pid;
    int timeCreated;
    int timeCompleted;
    int timeFinished;
    int timeReady;
    int memoryRequired;
    
    PCB(){
    }
    PCB(int newPid, int newTimeCreated, int newTimeCompleted, int newMemory){
        pid = newPid;
        timeCreated = newTimeCreated;
        timeCompleted = newTimeCompleted;
        memoryRequired = newMemory;
    }
    
};
#endif /* PCB_hpp */

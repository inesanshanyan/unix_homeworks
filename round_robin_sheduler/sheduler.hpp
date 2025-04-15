#pragma once

#include "process_queue.hpp"
#include "executor.hpp"

class Scheduler {
public:
    Scheduler(int quantum) : timeQuantum(quantum), timer(0) { }
    
    void run(ProcessQueue& processQueue) {
        while (!processQueue.isEmpty()) {
            Process current = processQueue.dequeue();
    
            int execTime;
            if (current.getRemainingTime() < timeQuantum) {
                execTime = current.getRemainingTime();
            } else {
                execTime = timeQuantum;
            }
    
            current.reduceRemainingTime(execTime);
            executor.run(current, execTime);
            timer += execTime;
    
            if (!current.isFinished()) {
                processQueue.enqueue(current); 
            } else {
                std::cout << "Process PID " << current.getPID()<< " finished, time: " << timer << "\n";
            }
        }
    
        std::cout << "\nProcesses are completed, time: " << timer << "\n";
    }
private:
    int timeQuantum;
    int timer;
    Executor executor;
};
    
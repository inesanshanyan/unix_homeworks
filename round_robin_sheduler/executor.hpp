#pragma once

#include <iostream>
#include "process.hpp"

class Executor {
public:
    void run(const Process& process, int executedTime) const {
        std::cout << "\n\n";
        std::cout << "Executing Process PID: " << process.getPID() << "\n";
        std::cout << "Job: " << process.getJobDescription() << "\n";
        std::cout << "Executed Time: " << executedTime << "\n";
        std::cout << "Burst Time: " << process.getBurstTime() << "\n";
        std::cout << "Remaining Time: " << process.getRemainingTime() << "\n";
    }
};
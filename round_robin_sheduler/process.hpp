#pragma once

#include <iostream>
#include <string>

class Process {
public:
    Process(int id, int burst, const std::string& job = "")
        : pid(id), burstTime(burst), remainingTime(burst), jobDescription(job) {}

    int getPID() const { return pid; }
    int getBurstTime() const { return burstTime; }
    int getRemainingTime() const { return remainingTime; }
    std::string getJobDescription() const { return jobDescription; }

    void reduceRemainingTime(int amount) {
        if (amount > 0 && remainingTime >= amount)
            remainingTime -= amount;
    }

    bool isFinished() const {
        return remainingTime <= 0;
    }

private:
    int pid;
    int burstTime;
    int remainingTime;
    std::string jobDescription;
};
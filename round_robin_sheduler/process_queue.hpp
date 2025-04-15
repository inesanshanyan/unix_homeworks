#pragma once

#include <queue>
#include "process.hpp"

class ProcessQueue {
public:
    void enqueue(const Process& process) {
        queue.push(process);
    }

    Process dequeue() {
        if (!queue.empty()) {
            Process front = queue.front();
            queue.pop();
            return front;
        }
        throw std::runtime_error("Queue is empty!");
    }

    Process& front() {
        return queue.front();
    }

    bool isEmpty() const {
        return queue.empty();
    }

    size_t size() const {
        return queue.size();
    }

private:
    std::queue<Process> queue;
};

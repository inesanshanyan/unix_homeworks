#include "sheduler.hpp"

int main() {
    ProcessQueue pq;
    pq.enqueue(Process(1, 6, "Reading"));
    pq.enqueue(Process(2, 4, "Writing"));
    pq.enqueue(Process(3, 9, "Executing"));

    Scheduler scheduler(3);
    scheduler.run(pq);
}
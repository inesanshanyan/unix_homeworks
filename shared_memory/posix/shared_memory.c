#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <semaphore.h>

const int SHM_SIZE = 1024;

void* create_shared_memory(const char* name) {
    int shm_fd = shm_open(name, O_CREAT | O_RDWR, 0666);
    if (shm_fd == -1) {
        perror("shm_open failed");
        exit(1);
    }
    
    if (ftruncate(shm_fd, SHM_SIZE) == -1) {
        perror("ftruncate failed");
        exit(1);
    }
    
    void* ptr = mmap(0, SHM_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0);
    if (ptr == MAP_FAILED) {
        perror("mmap failed");
        exit(1);
    }
    
    close(shm_fd);
    return ptr;
}


sem_t* create_semaphore(const char* name) {
    sem_t* sem = sem_open(name, O_CREAT, 0666, 1);
    if (sem == SEM_FAILED) {
        perror("sem_open failed");
        exit(1);
    }
    return sem;
}

void parent_write_to_memory(char* shm_ptr, sem_t* sem) {
    sem_wait(sem);  
    printf("Parent process is writing in shared memory\n");
    strcat(shm_ptr, " Text added by parent process. ");
    sem_post(sem);  
}

void child_write_to_memory(char* shm_ptr, sem_t* sem) {
    sem_wait(sem);  
    printf("Child process is writing in shared memory \n");
    strcat(shm_ptr, " Text added by child process. ");
    sem_post(sem);   
}

void cleanup(const char* shm_name, const char* sem_name, char* shm_ptr) {
    munmap(shm_ptr, SHM_SIZE);
    shm_unlink(shm_name);
    sem_unlink(sem_name);
    printf("\n\nShared memory and semaphore deleted.\n");
}

int main() {
    const char* shm_name = "/shared_memory";
    const char* sem_name = "/semaphore";


    char* shm_ptr = (char*)create_shared_memory(shm_name);
    *shm_ptr = '\0'; 
    

    sem_t* sem = create_semaphore(sem_name);

    pid_t pid = fork();

    if (pid < 0) {
        perror("Fork failed");
        exit(1);
    } else if (pid == 0) {
        child_write_to_memory(shm_ptr, sem);
        printf("Child: shared memory content: %s\n", shm_ptr);
        
        munmap(shm_ptr, SHM_SIZE);
        exit(0);

    } else {
        parent_write_to_memory(shm_ptr, sem);
        wait(NULL);
        parent_write_to_memory(shm_ptr, sem);
        printf("Parent: shared memory content: %s\n", shm_ptr);
    }

    cleanup(shm_name, sem_name, shm_ptr);
    sem_close(sem);

    return 0;
}
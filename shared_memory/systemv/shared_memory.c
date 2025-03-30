#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>

const int SHM_SIZE = 1024;

int create_shared_memory(key_t key) {
    int shm_id = shmget(key, SHM_SIZE, IPC_CREAT | 0666);
    if (shm_id == -1) {
        perror("shmget failed");
        exit(1);
    }
    return shm_id;
}

char* attach_shared_memory(int shm_id) {
    char* shm_ptr = (char*)shmat(shm_id, NULL, 0);
    if (shm_ptr == (char*)-1) {
        perror("shmat failed");
        exit(1);
    }
    return shm_ptr;
}

int create_semaphore(key_t key) {
    int sem_id = semget(key, 1, IPC_CREAT | 0666);
    if (sem_id == -1) {
        perror("semget failed");
        exit(1);
    }
    return sem_id;
}

void init_semaphore(int sem_id) {
    semctl(sem_id, 0, SETVAL, 1);
}

void semop_operation(int sem_id, short sem_op) {
    struct sembuf op = {0, sem_op, SEM_UNDO}; 
    semop(sem_id, &op, 1);
}

void parent_write_to_memory(char* shm_ptr, int sem_id) {
    semop_operation(sem_id, -1);  
    printf("Parent process is writing in shared memory\n");
    strcat(shm_ptr, " Text added by parent process. ");
    semop_operation(sem_id, 1);  
}

void child_write_to_memory(char* shm_ptr, int sem_id) {
    semop_operation(sem_id, -1);  
    printf("Child process is writing in shared memory \n");
    strcat(shm_ptr, " Text added by child process. ");
    semop_operation(sem_id, 1);   
}


void cleanup(int shm_id, int sem_id, char* shm_ptr) {
    shmdt(shm_ptr);
    shmctl(shm_id, IPC_RMID, NULL);
    semctl(sem_id, 0, IPC_RMID);
    printf("\n\nShared memory and semaphore deleted.\n");
}


int main() {
    key_t shm_key = ftok("shmfile", 65);  
    int shm_id = create_shared_memory(shm_key);
    int sem_id = create_semaphore(shm_key);

    init_semaphore(sem_id);

    char* shm_ptr = attach_shared_memory(shm_id);

    pid_t pid = fork();

    if (pid < 0) {
        perror("Fork failed");
        exit(1);
    } else if (pid == 0) {
        //sleep(1); 
        child_write_to_memory(shm_ptr, sem_id);
        printf("Child: Final shared memory content: %s\n", shm_ptr);
        shmdt(shm_ptr);
        exit(0);
    } else {
        parent_write_to_memory(shm_ptr, sem_id);
        //wait(NULL); 
        parent_write_to_memory(shm_ptr, sem_id);
        printf("Parent: Final shared memory content: %s\n", shm_ptr);
    }

    cleanup(shm_id, sem_id, shm_ptr);

    return 0;
}

#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdlib.h>

double countRectangleArea(double width, double height) {
    return width * height;
}

int main() {
    pid_t pid;
    pid = fork(); 

    if (pid == -1) {
        perror("fork failed");
        exit(EXIT_FAILURE);
    }
    else if (pid == 0) {
        printf("Child process: fork_id = %d, pid = %d, ppid = %d\n", pid, getpid(), getppid());
        
        const char *args[] = {"./triangle", NULL};  
        int return_value = execv("./triangle", (char *const *)args);  
        
        if (return_value == -1) {
            perror("execv failed");
            exit(EXIT_FAILURE);
        }

    } else {
        int status;
        pid_t child_pid = waitpid(pid, &status, 0);  

        if (child_pid == -1) {
            perror("waitpid failed");
            exit(EXIT_FAILURE);

        } else {
            if (WIFEXITED(status)) {
                printf("child process exited normally: %d\n", WEXITSTATUS(status));
            } else {
                printf("child process has failed\n");
            }
        }

        printf("Parent process: fork_id = %d, pid = %d, ppid = %d\n", pid, getpid(), getppid());
        
        double width, height;
        printf("Insert: rectangle width = ");
        scanf("%lf", &width);
        printf("height = ");
        scanf("%lf", &height);

        double area = countRectangleArea(width, height);
        printf("Rectangle area = %f\n", area);
    }

    return 0;
}

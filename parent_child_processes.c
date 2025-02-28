#include <stdio.h>
#include <unistd.h> 
#include <sys/types.h>
#include <sys/wait.h>

int main() {
    pid_t pid = fork(); 

    if (pid == -1) {
        perror("fork \n");
    }
    else if (pid == 0) {
        printf("child : fork_id = %d, pid = %d, ppid = %d \n", pid, getpid(), getppid());
        double base, height;
        printf("\nInsert: triangle base = ");
        scanf("%lf", &height);
        printf("height = ");
        scanf("%lf", &base);
        double area = 0.5 * base * height;
        printf("triangle area = %f\n", area);
    }
    else {
        wait(NULL);
        printf("\nparent : fork_id = %d, pid = %d, ppid = %d \n", pid, getpid(), getppid());
        double width, height;
        printf("\nInsert: rectangle width = ");
        scanf("%lf", &height);
        printf("height = ");
        scanf("%lf", &width);
        double area = width * height;
        printf("rectangle area = %f\n", area);
    }

    return 0;
}


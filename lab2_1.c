#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <pthread.h>

int flag1 = 0;
int flag2 = 0;

void* proc1(void* args1) {
    int* flag1 = (int*) args1;
    printf("Proc1 started\n");
    while (*flag1 == 0) {
        for(size_t i = 0; i < 10; i++) {
            printf("1\n");
            sleep(1);
        }
        sleep(1);
    }
    printf("Proc1 ended\n");
    pthread_exit((void*)1);
}

void* proc2(void* args2) {
    int* flag2 = (int*) args2;
    printf("Proc2 started\n");
    while (*flag2 == 0) {
        for(size_t i = 0; i < 10; i++) {
            printf("2\n");
            sleep(1);
        }
        sleep(1);
    }
    printf("Proc2 ended\n");
    pthread_exit((void*)2);
}

int main() {
    printf("Start of the main program\n");
    pthread_t id1, id2;
    int end1, end2;

    pthread_create(&id1,NULL, proc1, &flag1);
    pthread_create(&id2,NULL, proc2, &flag2);

    printf("Press a button\n");
    getchar();
    printf("Button pressed\n");

    flag1 = 1;
    flag2 = 1;

    pthread_join(id1, (void**) &end1);
    pthread_join(id2, (void**) &end2);

    printf("Exit code for proc1: %d\n", end1);
    printf("Exit code for proc2: %d\n", end2);

    printf("End of the main program\n");

    return 0;
}
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <time.h>


typedef struct{
    int flag;
    pthread_mutex_t* mutex;
} vars;

void initVars(vars* vars, int flag,pthread_mutex_t* mutex){
    vars->flag = flag;
    vars->mutex = mutex;
}

void* proc1(void* arg) {
    printf("Proc1 started\n");
    sleep(1);
    vars* args = (vars*) arg;

    while (args->flag == 0) {
        struct timespec tp;
        clock_gettime(CLOCK_REALTIME,&tp);
        tp.tv_sec +=3;
        int rv = pthread_mutex_timedlock(args->mutex,&tp);

        if (rv ==0 ){
            for(size_t i = 0; i < 10; i++) {
                printf("1\n");
                sleep(1);
            }
            pthread_mutex_unlock(args->mutex);
            sleep(1);
        }
        else{
            printf("Resourse busy right now [proc1] \n");
        }

    }
    printf("Proc1 ended\n");
    pthread_exit((void*)1);
}

void* proc2(void* arg) {
    printf("Proc2 started\n");
    sleep(1);
    vars* args = (vars*) arg;

    while (args->flag == 0) {
        struct timespec tp;
        clock_gettime(CLOCK_REALTIME, &tp);
        tp.tv_sec +=3;
        int rv = pthread_mutex_timedlock(args->mutex,&tp);

        if (rv ==0){
            for(size_t i = 0; i < 10; i++) {
                printf("2\n");
                sleep(1);
            }
            pthread_mutex_unlock(args->mutex);
            sleep(1);
        }
        else{
            printf("Resourse busy right now [proc2] \n");
        }

    }
    printf("Proc2 ended\n");
    pthread_exit((void*)2);
}

int main() {
    printf("Start of the main program\n");
    pthread_t id1, id2;
    int end1, end2;
    vars args1,args2;

    //mutex
    pthread_mutex_t mutex;
    pthread_mutex_init(&mutex, NULL);

    initVars(&args1,0,&mutex);
    initVars(&args2,0,&mutex);

    pthread_create(&id1, NULL, proc1, &args1);
    pthread_create(&id2, NULL, proc2, &args2);

    printf("Press a button\n");
    getchar();
    printf("Button pressed\n");

    args1.flag = 1;
    args2.flag = 1;

    pthread_join(id1, (void**) &end1);
    pthread_join(id2, (void**) &end2);

    printf("Exit code for proc1: %d\n", end1);
    printf("Exit code for proc2: %d\n", end2);

    //destroy mutex
    pthread_mutex_destroy(&mutex);

    printf("End of the main program\n");

    return 0;
}
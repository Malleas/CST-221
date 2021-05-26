//
// Created by matt on 5/26/21.
//
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>
#include <pthread.h>
#include <errno.h>
#include <sys/mman.h>
#include <semaphore.h>
#include <time.h>
#include <sys/wait.h>

//globals
sem_t *semaphore;
pid_t globalPid;
pid_t childPid;
time_t time_to_run;
sigset_t sigset;
int KILL_HUNG_PROC = SIGUSR1;
int count = 0;
int killSig = 0;

void signalHandler(int sig){
    killSig = sig;
    printf("Entering the signal handler\n");
}

void childProcess() {
    int sig;
    sigemptyset(&sigset);
    sigaddset(&sigset, KILL_HUNG_PROC);

    printf("Entering Child process!\n");
    sem_wait(semaphore);
    while (time(NULL) < time_to_run) {
        if(killSig == 0) {
            count++;
            printf("Counter in child: %d\n", count);
            sleep(1);
        } else{
            printf("Signal received to kill child process, exiting now\n");
            sem_post(semaphore);
            exit(0);
        }
    }
    sem_post(semaphore);
    exit(0);
}

int parentProcess() {
    sleep(10);
    int status = sem_trywait(semaphore);
    printf("Status of trywait in parent process is: %d\n", status);
    if (status == 0) {
        //do stuff
        printf("The parent process was able to lock counter and its current value is: %d\n", count);
        sem_post(semaphore);
        return 0;
    } else {
        printf("Parent process was unable to lock the counter, exiting now.\n");
        return 1;
    }
}

int main(int argc, char const *argv[]) {
    time_to_run = time(NULL) + 60;
    semaphore = (sem_t *) mmap(0, sizeof(sem_t), PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
    if (sem_init(semaphore, 1, 1) != 0) {
        printf("Failed to create semaphore\n");
        exit(EXIT_FAILURE);
    }
    pid_t pid;
    signal(SIGUSR1, signalHandler);

    pid = fork();

    if (pid == -1) {
        printf("Can't fork, error%d\n", errno);
        exit(EXIT_FAILURE);
    }
    if (pid == 0) {
        //child process
        globalPid = getppid();
        childPid = getpid();
        childProcess();
    } else {
        //parent process
        globalPid = pid;
        if (parentProcess() == 1) {
            printf("Killing child process!\n");
            kill(childPid, KILL_HUNG_PROC);
            int value;
            sem_getvalue(semaphore, &value);
            printf("The ending status of the semaphore is: %d\n", value);
        }
    }

    sem_destroy(semaphore);
    wait(NULL);

}

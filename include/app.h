#ifndef APP_H
#define APP_H

#include "library.h"

#define SLAVE_NUM 8
#define SLAVE_PATH "./slave"

typedef struct {
    pid_t pid;
    int in;
    int out;
    int fileNum;
} slave;

void createSlave(slave slavesArray[], int slaveAmount, char *path, char *const argv[]);
void sendFiles(int slaveNum,int filesPerSlave, slave *slavesArray, char ** argv, int taskNum, void * shMemory, FILE * outpFile,sem_t *sem);
void killSlave(slave slavesArray[], int slaveAmount);

#endif
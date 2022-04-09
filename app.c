#include "app.h"

int main(int argc, char *argv[]) {

    if (argc < 2) {
        errorHandler("Error: incorrect amount of arguments");
    }

    int taskNum = argc - 1; // Cantidad de archivos
    int slaveNum =  MIN(SLAVE_NUM,taskNum); // Cantidad de procesos esclavos
    int filesPerSlave = (taskNum > SLAVE_NUM * 2)? 2 : 1; //Cant de archivos que se le asignan a cada proceso esclavo

    slave slavesArray[slaveNum];

    void *shMemory;
    void *shMemCopy;
    int smFd; 
    off_t sizeSM = taskNum * BUFFER_SIZE; //Tamaño de la shared memory

    FILE *outpFile = fopen("result.txt", "w");
    if (outpFile == NULL) {
        errorHandler("Error opening result file (app)");
    }
    setBuffer(stdout,BUFFER_SIZE);

    createSM(shMemory, sizeSM, smFd);

    shMemCopy = shMemory;

    sem_t *sem;
    sem = (semOpen(SEM_NAME, O_CREAT | O_EXCL, S_IRUSR | S_IWUSR, INIT_VAL_SEM) == SEM_FAILED);

    printf("%d\n", (int)sizeSM);

    sleep(2); //Espera de 2 segundos por el proceso vista

    createSlave(slavesArray, slaveNum, SLAVE_PATH, argv);
    endApp(outpFile, slavesArray, slaveNum, sem, smFd, shMemCopy, sizeSM);
}

void createSlave(slave slavesArray[], int slaveNum, char *path, char *const argv[]) {
    pid_t pid;

    for (int i = 0; i < slaveNum; i++) {
       // Creo dos pipes para cada proceso esclavo
        int fdMasterToSlave[2], fdSlaveToMaster[2];

        if (pipe(fdMasterToSlave) == ERROR_CODE || pipe(fdSlaveToMaster) == ERROR_CODE) {
            errorHandler("Error creating pipe");
        }

        if ((pid = fork()) != ERROR_CODE) {

            if (pid == 0) { // en este caso estamos en un esclavo

                close(fdMasterToSlave[WRITE_FD]);
                close(fdSlaveToMaster[READ_FD]);

                if (dup2(fdMasterToSlave[READ_FD], 0) == ERROR_CODE || dup2(fdSlaveToMaster[WRITE_FD], 1) == ERROR_CODE) {
                    errorHandler("Error performing dup2 in function createChildren");
                }

                close(fdMasterToSlave[READ_FD]);
                close(fdSlaveToMaster[WRITE_FD]);

                if (execv(path, argv) == ERROR_CODE) {
                    errorHandler("Error performing execv in function createChildren");
                }


            } else { // en este caso estamos en el master
                close(fdMasterToSlave[READ_FD]);
                close(fdSlaveToMaster[WRITE_FD]);

                slavesArray[i].pid = pid;
                slavesArray[i].in = fdSlaveToMaster[READ_FD];
                slavesArray[i].out = fdMasterToSlave[WRITE_FD];
                slavesArray[i].fileNum = 0;
            }
        } else {
            errorHandler("Error in fork");
        }
    }
}

void killSlave(slave slavesArray[], int slaveNum) {

    for(int i = 0; i < slaveNum; i++) {

        if(close(slavesArray[i].in) == ERROR_CODE) {
            errorHandler("Error closing read end of fdSlaveToMaster");
        }

        if(close(slavesArray[i].out) == ERROR_CODE) {
            errorHandler("Error closing write end of fdMasterToSlave");
        }

    }
}

void sendFiles(int slaveNum,int filesPerSlave, slave *slavesArray, char ** argv, int taskNum, void * shMemory, FILE * outpFile,sem_t *sem) {
    /* envio la primera tanda de archivos */
    int tasksSent = 0;
    int tasksFinished = 0;

    for(int currentTask = 0, i = 1; currentTask < (filesPerSlave * slaveNum); currentTask++, i++) {

        char fileSent[BUFFER_SIZE] = {0};
        strcat(fileSent, argv[i]);
        strcat(fileSent, "\n\0");

        if(write(slavesArray[currentTask % slaveNum].out, fileSent, strlen(fileSent)) == ERROR_CODE) {
            errorHandler("Error writing in fdPath");
        }

        tasksSent++;
        slavesArray[currentTask % slaveNum].fileNum++;
    }

    /* envio el resto de archivos revisando cuando un esclavo se libera */

    char buffer[BUFFER_SIZE] = {0};

    while(tasksFinished < taskNum) {
        
        fd_set readFdSet;
        FD_ZERO(&readFdSet);

        int max = -1;

        for(int i = 0; i < slaveNum; i++) {
            if(slavesArray[i].in > 0) {
                max = MAX(max, slavesArray[i].in);
                FD_SET(slavesArray[i].in, &readFdSet);
            }
        }

        int ready;
        if((ready = select(max + 1, &readFdSet, NULL, NULL, NULL)) == ERROR_CODE) {
            errorHandler("Error in select");
        }

        for(int i = 0; i < slaveNum && ready > 0; i++) {

            int fd = slavesArray[i].in;

            if(FD_ISSET(fd, &readFdSet)) {
                
                //Recibo un archivo
                int dimRead = read(fd, buffer, BUFFER_SIZE);
                if (dimRead == ERROR_CODE) {
                    errorHandler("Error reading from fdData");
                } else if (dimRead <= 0) {
                    slavesArray[i].in= -1; //El hijo termino
                } else {
                    tasksFinished++;

                    //Escribo el resultado en result.txt
                    fprintf(outpFile, "%s\n", buffer);
                    slavesArray[i].fileNum--;
                    buffer[dimRead + 1] = '\0';

                    //Envio respuesta al view
                    if (sprintf((char*)(shMemory), "%s\n", buffer) == ERROR_CODE) {
                        errorHandler("Error performing sprintf while sending files");
                    }
                    shMemory += JUMP;
                    postSemaphore(sem);

                }

                /* envio nuevos archivos a los esclavos */
                if(slavesArray[i].fileNum == 0 && tasksSent < taskNum) {
                    char fileToSlave[BUFFER_SIZE] = {0};
                    strcat(fileToSlave, argv[tasksSent + 1]);
                    strcat(fileToSlave, "\n\0");
                    if(write(slavesArray[i].out, fileToSlave, strlen(fileToSlave)) == ERROR_CODE) {
                        errorHandler("Error sending files to slaves");
                    }
                    tasksSent++;
                    slavesArray[i].fileNum++;
                }
                ready--;
            }

        }

    }
}

void endApp(FILE * file, slave *slavesArray, int slaveNum, sem_t * sem, int smFd, void * mem, int sizeSM) {

    if (fclose(file) != 0) {
        errorHandler("Error closing result file in main (app)");
    }

    killSlave(slavesArray, slaveNum);

    semClose(sem);
    semUnlink();

    close(smFd);
    unmapSM(mem,sizeSM);
    unlinkSM();
}

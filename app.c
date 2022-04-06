#include "app.h"

int main(int argc, char *argv[]) {

    if (argc < 2) {
        errorHandler("Error: incorrect amount of arguments (app)");
    }

     /* If I have less arguments than SLAVE_AMOUNT, I create
    ** as many children as arguments, since I do not want more children than arguments
    */
    int taskNum = argc - 1; //cant de archivos
    int slaveNum =  MIN(SLAVE_AMOUNT,taskNum); // cant de procesos esclavos
    int filesPerSlave = (taskNum > SLAVE_AMOUNT * 2)? 2 : 1; //cant de archivos que se le asignan a cada proceso esclavo

    slave slavesArray[slaveNum];

    createSlave(slavesArray, slaveNum, SLAVE_PATH, argv);
    killSlave(slavesArray, slaveNum);
}

void createSlave(slave slavesArray[], int slaveNum, char *path, char *const argv[]) {
    pid_t pid;

    for (int i = 0; i < slaveNum; i++) {
        // creo los pipes
        // fdMasterToSlave va de master a esclavo
        // fdSlaveToMaster va de esclavo a maestro

       // le creo fd nuevos a cada esclavo
        int fdMasterToSlave[2], fdSlaveToMaster[2];

        if (pipe(fdMasterToSlave) == ERROR_CODE || pipe(fdSlaveToMaster) == ERROR_CODE) {
            errorHandler("Error creating pipe");
        }

        if ((pid = fork()) != ERROR_CODE) {

            if (pid == 0) { // en este caso estamos en un esclavo

                close(fdMasterToSlave[WRITE_FD]);
                close(fdSlaveToMaster[READ_FD]);

                if (dup2(fdMasterToSlave[READ_FD], 0) == ERROR_CODE || dup2(fdSlaveToMaster[WRITE_FD], 1) == ERROR_CODE) {
                    errorHandler("Error performing dup2 in function createChildren (app)");
                }

                close(fdMasterToSlave[READ_FD]);
                close(fdSlaveToMaster[WRITE_FD]);

                if (execv(path, argv) == ERROR_CODE) {
                    errorHandler("Error performing execv in function createChildren (app)");
                }


            } else { // es este caso estamos en el master
                close(fdMasterToSlave[READ_FD]);
                close(fdSlaveToMaster[WRITE_FD]);

                slavesArray[i].pid = pid;
                slavesArray[i].in = fdSlaveToMaster[READ_FD];
                slavesArray[i].out = fdMasterToSlave[WRITE_FD];
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


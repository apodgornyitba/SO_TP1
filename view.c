#include "library.h"
#define MEM_LEN 50

int main(int argc, char *argv[]) {

    setBuffer(stdin,BUFFER_SIZE);

    int smSize;

    if (argc == 1) {
        char buff[MEM_LEN] = {0};
        if(read(STDIN, buff, MEM_LEN) == ERROR_CODE) {
            errorHandler("Error reading data");
        }
        smSize = atoi(buff);
    } else if (argc == 2) {
        smSize = atoi(argv[1]);
    } else {
        errorHandler("Error: invalid amount of arguments");
    }

    if(smSize <= 0) {
        errorHandler("Error defining size of shared memory");
    }

    printf("%d\n", smSize);

    /* Abriendo memoria compartida y semaforos */
    int smFd;
    void * shMemory;
    sem_t * sem;

//    smFd = openSM(SHM_NAME, O_RDWR, 0666);

    if((smFd = shm_open(SHM_NAME, O_RDWR, 0666)) == ERROR_CODE) {
        errorHandler("Error opening shared memory");
    }

    if((shMemory = mmap(NULL, smSize, PROT_READ, MAP_SHARED, smFd, 0)) == MAP_FAILED) {
        errorHandler("Error mapping shared memory");
    }

//    sem = semOpen(SEM_NAME, O_CREAT, 0600, INIT_VAL_SEM);
    if ((sem = sem_open(SEM_NAME, O_CREAT, 0600, INIT_VAL_SEM)) == SEM_FAILED) {
        errorHandler("Error opening semaphore");
    }
    
    void *smCopy = shMemory;

    /* Imprimir los resultados guardados en la memoria */        
    int i = 0;
    while(i < smSize) {

        semWait(sem);

        printf("%s", (char *) shMemory);
        shMemory += JUMP;
        i += JUMP;
    }

    semClose(sem);
    close(smFd);
    unmapSM(smCopy,smSize);

    return 0;
}
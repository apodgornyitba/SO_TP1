#include "view.h"

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

    smFd = openSM(SHM_NAME, O_RDWR, 0666);

    if((shMemory = mmap(NULL, smSize, PROT_READ, MAP_SHARED, smFd, 0)) == MAP_FAILED) {
        errorHandler("Error mapping shared memory");
    }

    sem = semOpen(SEM_NAME, O_CREAT, 0600, INIT_VAL_SEM);
    
    void *shmCopy = shMemory;
        
    int i = 0;

    while(i < smSize) {

        waitSemaphore(sem);

        printf("%s", shMemory);
        shMemory += JUMP;
        i += JUMP;
    }

    closeSemaphore(sem);
    close(smFd);

    unmapSharedMemory(shMemory,smSize);

    return 0;
}



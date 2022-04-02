#include "library.h"

/*** *** *** ERROR HANDLING *** *** *** *** *** *** ***/

void errorHandler(const char *errorMsg) {
    char finalMsg[ERROR_MSG] = {0};
    strcat(finalMsg,errorMsg);
    strcat(finalMsg,"\n");
    perror(finalMsg);
    exit(EXIT_FAILURE);
}


/*** *** *** SEMAHPHORE HANDLING *** *** *** *** *** *** ***/

sem_t * semOpen(const char *name, int oflag, mode_t mode, unsigned int value){
    sem_t * sem;
    if ((sem = sem_open(SEM_NAME, O_CREAT | O_EXCL, S_IRUSR | S_IWUSR, INIT_VAL_SEM)) == SEM_FAILED) {
        errorHandler("Error opening semaphore (app)");
    }
    return sem;
}

void semPost(sem_t *sem) {
    if(sem_post(sem) == ERROR_CODE) {
        errorHandler("Error in sem_post");
    }
}

void semWait(sem_t * sem) {
  if(sem_wait(sem) == ERROR_CODE) {
      errorHandler("Error: sem_wait has failed");
  }
}

void semClose(sem_t * sem) {
    if(sem_close(sem) == ERROR_CODE) {
        unlinkSemaphore();
        errorHandler("Error closing semaphore");
    }
}

void semUnlink() {
    if(sem_unlink(SEM_NAME) == ERROR_CODE) {
        errorHandler("Error unlinking semaphore");
    }
}


/*** *** *** SHARED MEMORY HANDLING *** *** *** *** *** *** ***/

int openSM(const char* name, int oflag, mode_t mode){
    int fd;
    if ((fd = shm_open(name, oflag, mode)) == ERROR_CODE) {
        errorHandler("Error opening shared memory (app)");
    }
    return fd;
}

void unmapSM(void * memory, int size) {
    if(munmap( memory,size) == ERROR_CODE) {
        errorHandler("Error unmapping shared memory");
    }
}


void unlinkSM() {
    if(shm_unlink(SHM_NAME)==ERROR_CODE) {
        errorHandler("Error unlinking shared memory");
    }
}

void truncateSM(int fd, off_t lenght){
    if (ftruncate(fd, lenght) == ERROR_CODE) {
        errorHandler("Error setting size to shared memory (app)");
    }
}

void setBuffer(FILE * stream,size_t size) {
        if (setvbuf(stream, NULL, _IONBF, size) != 0) {
        errorHandler("Error setting buffer in process application");
    }
}
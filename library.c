// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include "library.h"

void errorHandler(const char *errorMsg) {
    char finalMsg[ERROR_MSG] = {0};
    strcat(finalMsg,errorMsg);
    strcat(finalMsg,"\n");
    perror(finalMsg);
    exit(EXIT_FAILURE);
}

void semOpen(const char *name, int oflag, mode_t mode, unsigned int value, sem_t ** sem){
    if ((*sem = sem_open(name, oflag, mode, value)) == SEM_FAILED) {
        errorHandler("Error opening semaphore");
    }
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
        semUnlink();
        errorHandler("Error closing semaphore");
    }
}

void semUnlink() {
    if(sem_unlink(SEM_NAME) == ERROR_CODE) {
        errorHandler("Error unlinking semaphore");
    }
}

void createSM(void ** shMemory, off_t sizeSM, int * smFd){

    if ((*smFd = shm_open(SHM_NAME, O_CREAT | O_RDWR, 0666)) == ERROR_CODE) {
        errorHandler("Error opening shared memory");
    }

    if (ftruncate(*smFd, sizeSM) == ERROR_CODE) {
        errorHandler("Error setting size to shared memory");
    }

    *shMemory = mmap(NULL, sizeSM, PROT_WRITE, MAP_SHARED, *smFd, 0);
    if (*shMemory == MAP_FAILED) {
        errorHandler("Error mapping shared memory");
    }
}

void openSM(const char* name, int oflag, mode_t mode, int * smFd){
    if ((*smFd = shm_open(name, oflag, mode)) == ERROR_CODE) {
        errorHandler("Error opening shared memory");
    }
}

void mmapSM(void * addr, size_t smSize, int prot, int flags, int smFd, off_t offset, void ** shMemory){
    *shMemory = mmap(NULL, smSize, PROT_READ, MAP_SHARED, smFd, 0);
    if(*shMemory == MAP_FAILED) {
        errorHandler("Error mapping shared memory");
    }
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

void setBuffer(FILE * stream,size_t size) {
        if (setvbuf(stream, NULL, _IONBF, size) != 0) {
        errorHandler("Error setting buffer in process application");
    }
}


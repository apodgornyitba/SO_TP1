#ifndef LIBRARY_H
#define LIBRARY_H

#define _DEFAULT_SOURCE
#define  _GNU_SOURCE

#include <errno.h>
#include <fcntl.h>           /* For O_* constants */
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/select.h>
#include <sys/stat.h>        /* For mode constants */
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#define ERROR_CODE -1
#define ERROR_MSG 100

#define INIT_VAL_SEM 0
#define SEM_NAME "/semaphore"
#define SHM_NAME "/shMemory"

#define STDIN 0
#define STDOUT 1

#define READ_FD 0
#define WRITE_FD 1

#define JUMP 1024
#define BUFFER_SIZE 1024

#define MAX(a, b) ((a) > (b) ? (a) : (b))
#define MIN(a, b) ((a) > (b) ? (b) : (a))

void errorHandler(const char *);

sem_t * semOpen(const char *name, int oflag, mode_t mode, unsigned int value);
void semPost(sem_t *);
void semWait(sem_t *);
void semClose(sem_t *);
void semUnlink();

int openSM(const char* name, int oflag, mode_t mode);
void unmapSM(void *, int );
void unlinkSM();
void truncateSM(int fd, off_t lenght);

void setBuffer(FILE * ,size_t );

#endif
#ifndef VIEW_H
#define VIEW_H

#include "library.h"

#define MEM_LEN 50

void handleData(sem_t *,char *, int);
void closeView(sem_t *, int, int , void * );

#endif
// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include "library.h"
#define MEM_LEN 50

int main(int argc, char *argv[]) {

    setBuffer(stdin,BUFFER_SIZE);

    // el write es atomico hasta la constante maxima
    // d

    /* Imprimir los resultados guardados en la memoria */        
    int i = 0;
    while(i < MAXIMO) {

        // semWait(sem);


        printf("%s", (char *) shMemory);
        shMemory += JUMP;
        i += JUMP;
    }

    semClose(sem);
    close(smFd);
    unmapSM(smCopy,smSize);

    return 0;
}
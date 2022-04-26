// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include "slave.h"

int main(int argc, const char *argv[]) {

    setBuffer(stdout,0);
    char *fileName = NULL;
    size_t fileSize = 0;

    while (getline(&fileName, &fileSize, stdin) > 0) {
        
        fileName[strcspn(fileName, "\n")] = 0;
        char command[BUFFER_SIZE] = {0};

        if (sprintf(command,COMMAND_FORMAT,fileName) < 0) {
            errorHandler("Error in sprintf");
        }

        FILE * fd = popen(command, "r"); /*abriendo el pipe para ejecutar comando*/
        if(fd == NULL) {
            errorHandler("Error performing popen");
        }

        char input[BUFFER_SIZE/2]={0};

        fread(input,sizeof(char),(BUFFER_SIZE/2)-1,fd);

        char output[BUFFER_SIZE];
        sprintf(output,"PID: %d\nFile: %s\n%s\n",getpid(),fileName,input);
        write(WRITE_FD,output,strlen(output)+1);

        int fd2 = open("nPipe", 0_WRONLY);
        write(fd2, output, strlen(output)+1);

        if(pclose(fd) == ERROR_CODE) {
            errorHandler("Error performing pclose in function solver (slave)");
        }

    }
    return 0;

}
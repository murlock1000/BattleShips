#include <unistd.h>
#include <string>
#include <iostream>

using namespace std;

int stdConnect (int childIO [2], const char* childPath, const char* childProcName) {
    
    int childInputPipe [2], childOutputPipe [2];

    if (pipe (childInputPipe) < 0) {
        return -1;
    }

    if (pipe (childOutputPipe) < 0) {
        return -1;
    }

    childIO [0] = childInputPipe [0];
    childIO [1] = childOutputPipe [1];

    pid_t pid;
    pid = fork();
    if (pid == pid_t(0)) {
        //parent
        close (childInputPipe [1]);
        close (childOutputPipe [0]);
        return 0;
    }
    else if (pid > pid_t(0)) {
        //child
        close (childInputPipe [0]);
        close (childOutputPipe [1]);
        dup2 (childInputPipe [1], 1);
        dup2 (childOutputPipe [0], 0);
        execl(childPath, childProcName, (char*)NULL);
        exit (0);
    }
    else {
        return -1;
    }
}

int stdRead (int fileDesc) {
    int buffer[1];
    int number = 0;
    while (true) {
        buffer[0] = 0;
        if (read (fileDesc, buffer, 1) < 0) {
            return -1;
        }
        if (buffer[0] == 32) {
            break;
        }
        number *= 10;
        number += (buffer[0] - 48);
    }
    return number;
}

int stdWrite (int fileDesc, int buffer) {
    char cBuffer [1];
    cBuffer [0] = buffer + 48;
    if (write (fileDesc, cBuffer, 1) < 0) {
        return -1;
    }
    return 0;
}


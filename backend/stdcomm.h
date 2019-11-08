#ifndef _STDCOMM
#define _STDCOMM 1

int stdConnect (int childIO [2], int* childPid, const char* childPath, const char* childProcName);

//This function should start a child process which would communicate with parent via stdin/stdout.
//It requires an int [2] array, a path of an executable and the process name of an executable (name of an executable without its path).
//It stores two file descriptors in the array; childIO [0] would contain child's input (which parent reads from), childIO [1] would contain child's output (which parent writes to)
//It returns 0 on sucess and -1 on failure if parent is the one which returns. It should also return 1 if child is the one which returns.

int stdRead (int fileDesc);

//This function reads a single positive whole number from child's input.
//It requires a file descriptor which corresponds to child's input.
//It returns a number on success and -1 on failure.
//Numbers sent by child should be space (ASCII 32) terminated.

int stdWrite (int fileDesc, int buffer);

//This function writes a single digit (a whole number from 0 to 9) to child's output.
//It requires a file descriptor which corresponds to child's output and a digit which should be sent to child.
//It returns 0 on success and -1 on failure.

#endif





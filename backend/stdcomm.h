#ifndef _STDCOMM
#define _STDCOMM 1

int stdConnect (int childIO [2], int* childPid, const char* childPath, const char* childProcName, const char* argument);
//HANDLE[0] = In_Wr write to child's cin
//HANDLE[1] = Out_Rd read child's cout
int stdConnectWin (HANDLE childIO [2], int* childPid, const char* childPath, const char* argument);

//This function should start a child process which would communicate with parent via stdin/stdout.
//It requires an empty int [2] array, an empty int pointer, a path of an executable and the process name of an executable (name of an executable without its path).
//It should store two file descriptors in the array; childIO [0] would contain child's input (which parent reads from), childIO [1] would contain child's output (which parent writes to)
//It should also store a child process id in a memory address referred to by a pointer childPid
//The child program should be executed with an argument stored in const char* argument
//It returns 0 on sucess and -1 on failure if parent is the one which returns. It should also return 1 if child is the one which returns.

int stdRead (int fileDesc);
int stdReadWin (HANDLE child_OUT_Rd);

//This function reads a single positive whole number from child's input.
//It requires a file descriptor which corresponds to child's input.
//It returns a number on success and -1 on failure.
//Numbers sent by child should be space (ASCII 32) terminated.

int stdWrite (int fileDesc, int buffer);
int stdWriteWin (HANDLE child_IN_Wr, int buffer);

//This function writes a single digit (a whole number from 0 to 9) to child's output.
//It requires a file descriptor which corresponds to child's output and a digit which should be sent to child.
//It returns 0 on success and -1 on failure.

int stdDisconnect (int childPid);
int stdDisconnectWin (int childPid);

//This function should terminate a process referred to by a provided process id.
//It requires child's process id.
//It should return 0 on success and -1 on failure.



#endif

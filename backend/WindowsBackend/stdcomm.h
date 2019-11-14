#ifndef STDCOMM_H
#define STDCOMM_H

#include <windows.h>


int stdConnectWin (HANDLE childIO [2], int* childPid, const char* childPath, const char* argument);
//HANDLE[0] = In_Wr write to child's cin
//HANDLE[1] = Out_Rd read child's cout

int stdReadWin (HANDLE child_OUT_Rd);

int stdWriteWin (HANDLE child_IN_Wr, int buffer);

int stdDisconnectWin (int childPid);

#endif

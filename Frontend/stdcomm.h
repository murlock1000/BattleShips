#pragma once

#include <windows.h>


int stdConnect (HANDLE childIO [2], int* childPid, const char* childPath, const char* childProcName, const char* argument);

int stdRead (HANDLE fileDesc);

int stdWrite (HANDLE fileDesc, int buffer);

int stdDisconnect (int childPid);

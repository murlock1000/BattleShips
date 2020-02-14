#pragma once

#ifdef __WIN32
#include <windows.h>
#endif

#ifndef HANDLE
#define HANDLE int
#endif

int stdConnect (HANDLE childIO [2], int* childPid, const char* childPath, const char* childProcName, const char* argument);

int stdRead (HANDLE fileDesc);

int stdWrite (HANDLE fileDesc, int buffer);

int stdDisconnect (int childPid);

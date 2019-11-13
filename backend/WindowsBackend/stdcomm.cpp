#include <windows.h>
#include <tchar.h>
#include <stdio.h>
#include <strsafe.h>
#include <iostream>

#include "stdcomm.h"

#define BUFSIZE 4096

using namespace std;


int stdConnectWin(HANDLE childIO [2], int* childPid, const char* childPath, const char* argument){
    //HANDLE[0] = In_Wr write to child's cin
    //HANDLE[1] = Out_Rd read child's cout
HANDLE g_hChildStd_IN_Rd = NULL;
HANDLE g_hChildStd_OUT_Wr = NULL;

SECURITY_ATTRIBUTES saAttr;

   saAttr.nLength = sizeof(SECURITY_ATTRIBUTES);
   saAttr.bInheritHandle = TRUE;
   saAttr.lpSecurityDescriptor = NULL;

// Create a pipe for the child process's STDOUT.

   if ( ! CreatePipe(&childIO[1], &g_hChildStd_OUT_Wr, &saAttr, 0) )
      {cout<<"StdoutRd CreatePipe"<<endl; return 0;}

// Ensure the read handle to the pipe for STDOUT is not inherited.

   if ( ! SetHandleInformation(childIO[1], HANDLE_FLAG_INHERIT, 0) )
     { cout<<"Stdout SetHandleInformation"<<endl;return 0;}
// Create a pipe for the child process's STDIN.

   if (! CreatePipe(&g_hChildStd_IN_Rd, &childIO[0], &saAttr, 0))
     { cout<<"Stdin CreatePipe"<<endl;return 0;}

// Ensure the write handle to the pipe for STDIN is not inherited.

   if ( ! SetHandleInformation(childIO[0], HANDLE_FLAG_INHERIT, 0) )
     { cout<<"Stdin SetHandleInformation"<<endl;return 0;}

    int processID;
    PROCESS_INFORMATION piProcInfo;
    STARTUPINFO siStartInfo;
    BOOL bSuccess = FALSE;

    ZeroMemory( &piProcInfo, sizeof(PROCESS_INFORMATION) );
    ZeroMemory( &siStartInfo, sizeof(STARTUPINFO) );

    siStartInfo.cb = sizeof(STARTUPINFO);
   siStartInfo.hStdError = g_hChildStd_OUT_Wr;
   siStartInfo.hStdOutput = g_hChildStd_OUT_Wr;
   siStartInfo.hStdInput = g_hChildStd_IN_Rd;
   siStartInfo.dwFlags |= STARTF_USESTDHANDLES;

   bSuccess = CreateProcess(childPath,
      const_cast<char *> (argument),     // command line
      NULL,          // process security attributes
      NULL,          // primary thread security attributes
      TRUE,          // handles are inherited
      0,             // creation flags
      NULL,          // use parent's environment
      NULL,          // use parent's current directory
      &siStartInfo,  // STARTUPINFO pointer
      &piProcInfo);  // receives PROCESS_INFORMATION

       if ( ! bSuccess ){
      cout<<"failed to create process, exited with error code "<<GetLastError()<<endl;
      return -1;
      }
   else
   {
        *childPid= (int)piProcInfo.dwProcessId;

      CloseHandle(piProcInfo.hProcess);
      CloseHandle(piProcInfo.hThread);
   }

}

int stdReadWin (HANDLE child_OUT_Rd){
    DWORD dwRead;
   char chBuf[BUFSIZE];
   BOOL bSuccess = FALSE;

   for (;;)
   {
      bSuccess = ReadFile( child_OUT_Rd, chBuf, BUFSIZE, &dwRead, NULL);
      if( ! bSuccess || dwRead == 0 ) return -1;
        else break;
   }

   return atoi(chBuf);
}


int stdWriteWin (HANDLE child_IN_Wr, int data){
    char chBuf[BUFSIZE];

    sprintf(chBuf, "%d", data);

    BOOL bSuccess = FALSE;

   for (;;)
   {

      bSuccess = WriteFile(child_IN_Wr, chBuf, strlen(chBuf), NULL, NULL);

      if ( ! bSuccess ) return -1;
        else break;

   }

      return 0;
}


int stdDisconnectWin (int childPid){
     bool bSuccess;
    HANDLE tmpHandle = OpenProcess(PROCESS_ALL_ACCESS, TRUE, childPid);
    if (NULL != tmpHandle)
    {
       bSuccess = TerminateProcess(tmpHandle, 0);
    }

      if ( ! bSuccess ){
      cout<<"failed to create process"<<endl;
      return -1;
      }

    return 0;
}






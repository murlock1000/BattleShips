
#include <windows.h>
#include <tchar.h>
#include <stdio.h>
#include <strsafe.h>
#include <iostream>

#include "stdcomm.h"

#define BUFSIZE 4096

using namespace std;


int stdConnect(HANDLE childIO[2], int* childPid, const char* childPath, const char* childProcName, const char* argument) {
	//HANDLE[0] = Out_Rd read child's cout
	//HANDLE[1] = In_Wr write to child's cin
	HANDLE g_hChildStd_IN_Rd = NULL;
	HANDLE g_hChildStd_OUT_Wr = NULL;

	SECURITY_ATTRIBUTES saAttr;
	OVERLAPPED oOverlap;

	saAttr.nLength = sizeof(SECURITY_ATTRIBUTES);
	saAttr.bInheritHandle = TRUE;
	saAttr.lpSecurityDescriptor = NULL;

	// Create a pipe for the child process's STDOUT.

	if (!CreatePipe(&childIO[0], &g_hChildStd_OUT_Wr, &saAttr, 0))
	{
		cout << "StdoutRd CreatePipe" << endl; return -1;
	}

	// Ensure the read handle to the pipe for STDOUT is not inherited.

	if (!SetHandleInformation(childIO[0], HANDLE_FLAG_INHERIT, 0))
	{
		cout << "Stdout SetHandleInformation" << endl; return -1;
	}
	// Create a pipe for the child process's STDIN.

	if (!CreatePipe(&g_hChildStd_IN_Rd, &childIO[1], &saAttr, 0))
	{
		cout << "Stdin CreatePipe" << endl; return -1;
	}

	// Ensure the write handle to the pipe for STDIN is not inherited.

	if (!SetHandleInformation(childIO[1], HANDLE_FLAG_INHERIT, -1))
	{
		cout << "Stdin SetHandleInformation" << endl; return -1;
	}

	int processID;
	PROCESS_INFORMATION piProcInfo;
	STARTUPINFO siStartInfo;
	BOOL bSuccess = FALSE;

	ZeroMemory(&piProcInfo, sizeof(PROCESS_INFORMATION));
	ZeroMemory(&siStartInfo, sizeof(STARTUPINFO));

	siStartInfo.cb = sizeof(STARTUPINFO);
	siStartInfo.hStdError = g_hChildStd_OUT_Wr;
	siStartInfo.hStdOutput = g_hChildStd_OUT_Wr;
	siStartInfo.hStdInput = g_hChildStd_IN_Rd;
	siStartInfo.dwFlags |= STARTF_USESTDHANDLES;

	bSuccess = CreateProcess(childPath,
		const_cast<char*>(argument), // const_cast<char *> (argument),     // command line
		NULL,          // process security attributes
		NULL,          // primary thread security attributes
		TRUE,          // handles are inherited
		0,             // creation flags
		NULL,          // use parent's environment
		NULL,          // use parent's current directory
		&siStartInfo,  // STARTUPINFO pointer
		&piProcInfo);  // receives PROCESS_INFORMATION

	if (!bSuccess) {
		cout << "failed to create process, exited with error code " << GetLastError() << endl;
		return -1;
	}
	else
	{
		*childPid = (int)piProcInfo.dwProcessId;

		CloseHandle(piProcInfo.hProcess);
		CloseHandle(piProcInfo.hThread);

		return 0;
	}

}

int stdRead(HANDLE fileDesc) {
	DWORD dwRead;
	DWORD dwToRead;
	char chBuf[1];
	BOOL bSuccess = FALSE;
	int number = 0;
	char s;

	int TimeElapsed = 0;
	int t = 0;
	for (;;)
	{
		//cout << t<<" - Reading" << endl;
		//t++;


		bSuccess = PeekNamedPipe(fileDesc, NULL, NULL, NULL, &dwToRead, NULL); //peek pipe output

		if (!bSuccess) {
			cout << "Error code: " << GetLastError() << endl;
			return -1;
		}

		if (dwToRead == 0) { //if pipe is empty
			//cout << 'b' << endl;
			if (TimeElapsed > 100) {
				return -1; //if AI did not respond in 1 second - end game
			}
			else {
				TimeElapsed++;
				Sleep(10);//wait for 10ms
			}
		}
		else {

			bSuccess = ReadFile(fileDesc, chBuf, 1, &dwRead, NULL); //nuskaitome 1 char = 1 byte i chBuf

			if (!bSuccess) {
				cout << "Error code: " << GetLastError() << endl;
				return -1;
			}

			else {
				s = chBuf[0];
				if (s == ' ') {
					break;
				}
				else {
					number *= 10; //konstruojame skaiciu is skaitmenu
					number += s - '0';
				}
			}
		}

	}
	return number;

}


int stdWrite(HANDLE fileDesc, int data) {
	char chBuf[BUFSIZE];

	sprintf_s(chBuf, "%d%c", data, '\n');

	BOOL bSuccess = FALSE;

	for (;;)
	{

		bSuccess = WriteFile(fileDesc, chBuf, strlen(chBuf), NULL, NULL);

		if (!bSuccess) { cout << "Error code: " << GetLastError() << endl; return -1; }
		else break;

	}

	return 0;
}


int stdDisconnect(int childPid) {
	bool bSuccess;
	HANDLE tmpHandle = OpenProcess(PROCESS_ALL_ACCESS, TRUE, childPid);
	if (NULL != tmpHandle)
	{
		bSuccess = TerminateProcess(tmpHandle, 0);
	}

	if (!bSuccess) {//welp can't really do anything
	//	cout << "failed to create process" << endl;
		//return -1;
		return 0;
	}

	return 0;
}






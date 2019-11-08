#include <iostream> //Hmm, I wonder what this header provides...
#include <unistd.h> //Provides various system calls, such as pipe() or fork()
#include <errno.h> //Provides error handling functionality
#include <string.h> //Provides strerror()
#include <signal.h> // Provides kill()
#include "stdcomm.h" //Provides AI communication functions prototypes

using namespace std;

int stdConnect (int childIO [2], int* childPid, const char* childPath, const char* childProcName) {
    
    //This function creates a child process which can communicate with parent process via stdin/stdout.
    //It writes some file descriptors to childIO which you can use to communicate with child. More on that later.
    //It requires an int [2] array (obviously), a file path of an executable you wish to execute as a to-be-executed executable, and its process name (basically the name of the executable without its path)
    //It returns 0 on success and -1 on failure for parent. It always returns 1 for child.

    int childInputPipe [2], childOutputPipe [2]; 

    if (pipe (childInputPipe) < 0) { //creating a pipe and handling errors
        cout << "ERROR: " << strerror(errno) << "\n"; //errno is set to the last error code. strerror() converts it into a human-readable string.
        return -1;
    }

    if (pipe (childOutputPipe) < 0) { //ditto
        cout << "ERROR: " << strerror(errno) << "\n";
        return -1;
    }

    //So, we have created two pipes. A pipe is an unidirectional means of communication (which is why we need two of them). Whatever is written to the pipe's output can be read from its input.
    //The pipe() system call writes some file descriptors into a provided int [2] array. The first element is pipe's input (which you can read from), the second is its output (which you can write to).

    childIO [0] = childInputPipe [0]; //This is an input end of the input pipe.
    childIO [1] = childOutputPipe [1]; //This is an output end of the output pipe.

    pid_t pid;
    pid = fork(); //This system call creates a child process which starts from the next instruction. Since they now execute the same code, we need to separate them.
   
    if (pid > pid_t(0)) { //This code will only be executed by parent.
        *childPid = pid;

        //Now we have a problem. These pipes we created earlier are also open in child and they have the same file descriptors. It is not a good idea to have the same fd open several times, so we'll have to close some of them.
 
        close (childInputPipe [1]); //Closing an output end of the input pipe.
        close (childOutputPipe [0]); //Closing an input end of the output pipe.

        //Child will close the file descriptors left open by parent. That way each fd will be open only once and parent will still be able to communicate with child via these pipes we created.
        
        //Before returning success, we need to check if AI is actually running. 0 will be sent by AI, otherwise the sent number is errno.

        int execSuccess = stdRead (childIO [0]);

        if (execSuccess == 0) {
            return 0;
        }
        else {
            //We have to prevent child from leaving before reading, otherwise read will fail due to closed fds.
            //We can stop child from exiting by making it wait for input and then writing literally anything after we're done reading.
            
            stdWrite (childIO [1], 0); //now we can finally fail

            cout << "ERROR: " << strerror(execSuccess) << "\n";
            return -1;
        }
    }

    else if (pid == pid_t(0)) { //This code will only be executed by child.

        close (childInputPipe [0]); //Closing an input end of the input pipe.
        close (childOutputPipe [1]); //Closing an output end of the output pipe.

        //Now all the data sent from the output pipe in the parent will end up read from the output pipe in the child, and vice versa with the input pipe.
        //The problem is that these fds are not connected to stdin/stdout.
        //It turns out linux has this dup2() system call which can copy the existing fds into new fds, closing the latter if they were open beforehand. This is exactly what we need!

        dup2 (childInputPipe [1], 1); //1 is the handle of stdout
        dup2 (childOutputPipe [0], 0); //0 is the handle of stdin

        //Now any attempt to read or write stdin/stdout from the child will end up in the pipes connected to the parent.
        //However, child here is still executing this code instead of running an executable file we need.
        //You guessed it, time for another system call! execl() will give the shell used by the child to an executable, which means our pipes and fd changes are still intact.
        
        if (execl(childPath, childProcName, (char*)NULL) < 0) { //Note that execl requires an executable file path, its process name, and the rest of arguments terminated by (char*)NULL.
            cout << errno << " "; //number other that 0 tells parent that AI execution was unsuccessful.
         
            int useless;
            cin >> useless; //this is where we tell child to wait for parent output. Again, the output itself is completely useless, it's only used to make child wait.
        }

        //close fds we no longer need.

        close (0);
        close (1);
        close (childInputPipe [1]);
        close (childOutputPipe [0]);
        
        return 1; //child returns 1 to be killed by the main function in game.cpp.
    }

    else {
        cout << "ERROR: " << strerror(errno) << "\n";
        return -1; 
    }
}

int stdRead (int fileDesc) {

    //This function reads a space (ASCII 32) terminated number from a provided fd.
    //It returns a read number on success and -1 on failure.
    
    //Please be kind and ignore this mess. It has got an efficiency of O(log(x*y)), which is good enough for our purposes, okay? 

    int buffer [1];
    int number = 0;

    while (true) {

        //I know, it's terrible. Anyway, this function reads the number digit by digit, which is why it needs to be terminated. I chose space (ASCII 32) because read() doesn't like ASCII 0.

        buffer[0] = 0;

        if (read (fileDesc, buffer, 1) < 0) { //both read and write system calls can only read from/write to a char array.
            cout << "ERROR: " << strerror(errno) << "\n";
            return -1;
        }
        if (buffer[0] == 32) { //YOU ARE TERMINATED!
            break;
        }
        number *= 10;
        number += (buffer[0] - 48); //converting char to int
    }
    return number;
}

int stdWrite (int fileDesc, int buffer) {

    //This function writes a provided single digit to a provided fd.
    //It returns 0 on success and -1 on failure.
    //Again, it only writes a single digit from 0 to 9, so keep that in mind.

    char cBuffer [1];
    cBuffer [0] = buffer + 48; //converting int to char.

    if (write (fileDesc, cBuffer, 1) < 0) {
        cout << "ERROR: " << strerror(errno) << "\n";
        return -1;
    }

    return 0;
}

int stdDisconnect (int childPid) {
    
    //This function terminates a child process if it doesn't want to kill itself
    //It requires a process id of a child
    //It returns 0 on success and -1 on failure
    
    if (kill (childPid, 0) < 0) { //sends null signal to the process (which doesn't do anything), checks if it exists
        if (errno == 3) { //error code for "No such process"
            return 0; //Child already terminated itself, safe to return
        }
        else { //Permission denied
            cout << "Unexpected error while attempting to terminate a child process:\n";
            cout << "ERROR: " << strerror(errno) << "\n";
            cout << "Please kill process " << childPid << " manually\n";
            return -1;
        }
    }

    kill (childPid, 15); //If child is still alive, we have to ask it to die

    if (kill (childPid, 0) < 0) { //same code as above, checking if child still exists
        if (errno == 3) {
            return 0;
        }
        else { 
            cout << "Unexpected error while attempting to terminate a child process:\n";
            cout << "ERROR: " << strerror(errno) << "\n";
            cout << "Please kill process " << childPid << " manually\n";
            return -1;
        }
    }
    
    kill (childPid, 9); //Child is being stubborn, we'll have to ask kernel to kill it instead

    if (kill (childPid, 0) < 0) { //Apparently that child is a literal Satan's offspring. This program does not have enough godly power to destroy it.
        cout << "ERROR: Unable to kill child process\n";
        cout << "Please kill process " << childPid << " manually\n";
        return -1;
    }

    return 0;
}

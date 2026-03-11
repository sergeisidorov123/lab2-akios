cmake_minimum_required(VERSION 4.0)
project(lab2)

set(CMAKE_CXX_STANDARD 20)

add_executable(lab2 main.cpp
        grid/Grid.h
        grid/Grid.cpp
        Colors/Colors.h
        Colors/Colors.cpp
        Config/Config.h
        Config/Config.cpp
        WindowsProcedures/WindowProc.h
        WindowsProcedures/WindowProc.cpp)

if(WIN32)
    set_target_properties(${PROJECT_NAME} PROPERTIES LINK_FLAGS "-static -static-libgcc -static-libstdc++")
endif()
#ifndef MAIN_H
#define MAIN_H

#include <windows.h>
#include <vector>
#include "Colors/Colors.h"

extern std::vector<HWND> Windows;

extern HANDLE hMapFile;
extern SharedData* sharedData;
extern HANDLE hMutex;

#endif
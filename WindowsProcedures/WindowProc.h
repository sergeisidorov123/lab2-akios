#ifndef WINDOWPROC_H
#define WINDOWPROC_H

#include <windows.h>
extern UINT WM_UPDATE_GRID;
LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
#endif
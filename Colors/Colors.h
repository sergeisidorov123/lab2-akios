#ifndef COLORS_H
#define COLORS_H

#include <windows.h>
#include <cstdlib>
#include <ctime>

extern int GRID_COLOR_R;
extern int GRID_COLOR_G;
extern int GRID_COLOR_B;
extern COLORREF BACKGROUND_COLOR;
extern HBRUSH hBackgroundBrush;

void InitRandomSeed();
COLORREF GetRandomColor();
void UpdateGridColor(int delta);
void UpdateBackgroundBrush(HWND hwnd);
void InitBackgroundBrush(HWND hwnd);
void CleanupBackgroundBrush();

#endif
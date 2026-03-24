#ifndef COLORS_H
#define COLORS_H

#include <windows.h>
#include <cstdlib>
#include <ctime>
#include "../Grid/Grid.h"


extern int GRID_COLOR_R;
extern int GRID_COLOR_G;
extern int GRID_COLOR_B;
extern COLORREF BACKGROUND_COLOR;
extern HBRUSH hBackgroundBrush;

struct SharedData {
    Cell cells[MAX_CELLS];
    COLORREF backgroundColor;
    int GRID_COLOR_R;
    int GRID_COLOR_G;
    int GRID_COLOR_B;
    int WindowsCreated;
};

void InitRandomSeed();
COLORREF GetRandomColor();
void UpdateGridColor(int delta);
void UpdateBackgroundBrush(HWND hwnd);
void InitBackgroundBrush(HWND hwnd);
void CleanupBackgroundBrush();

#endif
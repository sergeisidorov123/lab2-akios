#ifndef GRID_H
#define GRID_H

#include <windows.h>
#include <vector>
const int MAX_CELLS = 5000;

struct Cell {
    bool hasCircle;
    bool hasCross;
};

extern Cell cells[MAX_CELLS];

extern int CELL_SIZE;
extern int GRID_SIZE_X;
extern int GRID_SIZE_Y;

void ResizeGrid(int newGridSizeX, int newGridSizeY);
void RecalculateGrid(HWND hwnd);
int GetCellIndex(int row, int col);
void AddCircle(int row, int col);
void AddCross(int row, int col);
bool HasCircle(int row, int col);
bool HasCross(int row, int col);
void DrawCircleInCell(HDC hdc, int row, int col, int cellWidth, int cellHeight);
void DrawCrossInCell(HDC hdc, int row, int col, int cellWidth, int cellHeight);

#endif
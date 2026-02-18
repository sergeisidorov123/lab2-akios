#include "Grid.h"
#include <algorithm>

int CELL_SIZE = 20;
int GRID_SIZE_X = 0;
int GRID_SIZE_Y = 0;
std::vector<Cell> cells;

// Цвета фигур
const COLORREF CIRCLE_COLOR = RGB(255, 255, 0);
const COLORREF CROSS_COLOR = RGB(0, 255, 0);

// Функция для изменения размера сетки (полная перезапись)
void ResizeGrid(int newGridSizeX, int newGridSizeY) {
    GRID_SIZE_X = newGridSizeX;
    GRID_SIZE_Y = newGridSizeY;
    cells.resize(GRID_SIZE_X * GRID_SIZE_Y);

    for (int i = 0; i < GRID_SIZE_X * GRID_SIZE_Y; i++) {
        cells[i].hasCircle = false;
        cells[i].hasCross = false;
    }
}

// Функция для пересчета размера сетки на основе размера окна
void RecalculateGrid(HWND hwnd) {
    RECT clientRect;
    GetClientRect(hwnd, &clientRect);

    int newGridSizeX = clientRect.right / CELL_SIZE;
    int newGridSizeY = clientRect.bottom / CELL_SIZE;

    if (newGridSizeX < 1) newGridSizeX = 1;
    if (newGridSizeY < 1) newGridSizeY = 1;

    if (newGridSizeX != GRID_SIZE_X || newGridSizeY != GRID_SIZE_Y) {
        int oldGridSizeX = GRID_SIZE_X;
        int oldGridSizeY = GRID_SIZE_Y;
        std::vector<Cell> oldCells = cells;

        GRID_SIZE_X = newGridSizeX;
        GRID_SIZE_Y = newGridSizeY;

        cells.clear();
        cells.resize(GRID_SIZE_X * GRID_SIZE_Y);

        for (int i = 0; i < GRID_SIZE_X * GRID_SIZE_Y; i++) {
            cells[i].hasCircle = false;
            cells[i].hasCross = false;
        }

        if (oldGridSizeX > 0 && oldGridSizeY > 0 && !oldCells.empty()) {
            for (int row = 0; row < std::min(oldGridSizeY, GRID_SIZE_Y); row++) {
                for (int col = 0; col < std::min(oldGridSizeX, GRID_SIZE_X); col++) {
                    int oldIndex = row * oldGridSizeX + col;
                    int newIndex = row * GRID_SIZE_X + col;

                    if (oldIndex < oldCells.size() && newIndex < cells.size()) {
                        cells[newIndex].hasCircle = oldCells[oldIndex].hasCircle;
                        cells[newIndex].hasCross = oldCells[oldIndex].hasCross;
                    }
                }
            }
        }
    }
}

// Получение индекса в массиве по координатам ячейки
int GetCellIndex(int row, int col) {
    return row * GRID_SIZE_X + col;
}

// Добавление круга в ячейку
void AddCircle(int row, int col) {
    if (row >= 0 && row < GRID_SIZE_Y && col >= 0 && col < GRID_SIZE_X) {
        int index = GetCellIndex(row, col);
        cells[index].hasCircle = true;
        cells[index].hasCross = false;
    }
}

// Добавление крестика в ячейку
void AddCross(int row, int col) {
    if (row >= 0 && row < GRID_SIZE_Y && col >= 0 && col < GRID_SIZE_X) {
        int index = GetCellIndex(row, col);
        cells[index].hasCross = true;
        cells[index].hasCircle = false;
    }
}

// Проверка наличия круга в ячейке
bool HasCircle(int row, int col) {
    if (row >= 0 && row < GRID_SIZE_Y && col >= 0 && col < GRID_SIZE_X) {
        int index = GetCellIndex(row, col);
        return cells[index].hasCircle;
    }
    return false;
}

// Проверка наличия крестика в ячейке
bool HasCross(int row, int col) {
    if (row >= 0 && row < GRID_SIZE_Y && col >= 0 && col < GRID_SIZE_X) {
        int index = GetCellIndex(row, col);
        return cells[index].hasCross;
    }
    return false;
}

// Рисование круга в ячейке
void DrawCircleInCell(HDC hdc, int row, int col, int cellWidth, int cellHeight) {
    int centerX = col * cellWidth + cellWidth / 2;
    int centerY = row * cellHeight + cellHeight / 2;
    int radius = std::min(cellWidth, cellHeight) / 3;

    HBRUSH circleBrush = CreateSolidBrush(CIRCLE_COLOR);
    HBRUSH oldBrush = (HBRUSH)SelectObject(hdc, circleBrush);

    Ellipse(hdc,
            centerX - radius, centerY - radius,
            centerX + radius, centerY + radius);

    SelectObject(hdc, oldBrush);
    DeleteObject(circleBrush);
}

// Рисование крестика в ячейке
void DrawCrossInCell(HDC hdc, int row, int col, int cellWidth, int cellHeight) {
    int left = col * cellWidth;
    int top = row * cellHeight;
    int right = left + cellWidth;
    int bottom = top + cellHeight;

    int margin = std::min(cellWidth, cellHeight) / 6;

    HPEN crossPen = CreatePen(PS_SOLID, std::max(1, cellWidth / 20), CROSS_COLOR);
    HPEN oldPen = (HPEN)SelectObject(hdc, crossPen);

    MoveToEx(hdc, left + margin, top + margin, NULL);
    LineTo(hdc, right - margin, bottom - margin);

    MoveToEx(hdc, right - margin, top + margin, NULL);
    LineTo(hdc, left + margin, bottom - margin);

    SelectObject(hdc, oldPen);
    DeleteObject(crossPen);
}

// Очистка всей сетки
void ClearGrid() {
    for (int i = 0; i < cells.size(); i++) {
        cells[i].hasCircle = false;
        cells[i].hasCross = false;
    }
}
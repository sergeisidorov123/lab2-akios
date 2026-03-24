#include "Colors.h"
#include "../main.h"
#include "../WindowsProcedures/WindowProc.h"

// Цвет сетки по умолчанию красный
int GRID_COLOR_R = 255;
int GRID_COLOR_G = 0;
int GRID_COLOR_B = 0;

// Цвет фона - по умолчанию синий
COLORREF BACKGROUND_COLOR = RGB(0, 0, 255);

// Кисть для изменения цвета фона
HBRUSH hBackgroundBrush = NULL;

// Генератора случайных чисел текущим временем
void InitRandomSeed() {
    srand(static_cast<unsigned>(time(NULL)));
}


// Получением случайного цвета ргб формате
COLORREF GetRandomColor() {
    return RGB(rand() % 256, rand() % 256, rand() % 256);
}

// Изменение цвета сетки
void UpdateGridColor(int delta) {
    sharedData->GRID_COLOR_R = (sharedData->GRID_COLOR_R + delta) % 256;
    sharedData->GRID_COLOR_G = (sharedData->GRID_COLOR_G + delta) % 256;
    sharedData->GRID_COLOR_B = (sharedData->GRID_COLOR_B + delta) % 256;
}


// Кисть фона
void InitBackgroundBrush(HWND hwnd) {
    if (sharedData->backgroundColor == 0) {
        sharedData->backgroundColor = RGB(0, 0, 255);
    }

    hBackgroundBrush = CreateSolidBrush(sharedData->backgroundColor);
    SetClassLongPtrA(hwnd, GCLP_HBRBACKGROUND, (LONG_PTR)hBackgroundBrush);
}

void UpdateBackgroundBrush(HWND hwnd) {
    if (!sharedData) return;
    if (hBackgroundBrush) DeleteObject(hBackgroundBrush);
    hBackgroundBrush = CreateSolidBrush(sharedData->backgroundColor);
    SetClassLongPtrA(hwnd, GCLP_HBRBACKGROUND, (LONG_PTR)hBackgroundBrush);
}


// Очиска кисти фона
void CleanupBackgroundBrush() {
    if (hBackgroundBrush) DeleteObject(hBackgroundBrush);
}
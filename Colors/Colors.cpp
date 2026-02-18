#include "Colors.h"

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
    GRID_COLOR_R = (GRID_COLOR_R + delta + 256) % 256;
    GRID_COLOR_G = (GRID_COLOR_G + delta + 256) % 256;
    GRID_COLOR_B = (GRID_COLOR_B + delta + 256) % 256;
}


// Кисть фона
void InitBackgroundBrush(HWND hwnd) {
    hBackgroundBrush = CreateSolidBrush(BACKGROUND_COLOR);
    SetClassLongPtrA(hwnd, GCLP_HBRBACKGROUND, (LONG_PTR)hBackgroundBrush);
}

// Обновление кисти фона
void UpdateBackgroundBrush(HWND hwnd) {
    if (hBackgroundBrush) DeleteObject(hBackgroundBrush);
    hBackgroundBrush = CreateSolidBrush(BACKGROUND_COLOR);
    SetClassLongPtrA(hwnd, GCLP_HBRBACKGROUND, (LONG_PTR)hBackgroundBrush);
    InvalidateRect(hwnd, NULL, TRUE);
}


// Очиска кисти фона
void CleanupBackgroundBrush() {
    if (hBackgroundBrush) DeleteObject(hBackgroundBrush);
}
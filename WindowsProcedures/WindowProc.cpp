#include "WindowProc.h"
#include "../Grid/Grid.h"
#include "../Colors/Colors.h"
#include "../Config/Config.h"
#include <shellapi.h>

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    switch (uMsg) {
        
        // Обработка изменения размера окна
        case WM_SIZE: {
            // При изменении размера окна пересчитываем сетку
            RecalculateGrid(hwnd);
            InvalidateRect(hwnd, NULL, TRUE);
            return 0;
        }

        // Обработка кликов мышки
        case WM_LBUTTONDOWN: {
            // Левый клик - рисование круга
            int xPos = LOWORD(lParam);
            int yPos = HIWORD(lParam);

            RECT clientRect;
            GetClientRect(hwnd, &clientRect);

            int cellWidth = clientRect.right / GRID_SIZE_X;
            int cellHeight = clientRect.bottom / GRID_SIZE_Y;

            int col = xPos / cellWidth;
            int row = yPos / cellHeight;

            if (row >= 0 && row < GRID_SIZE_Y && col >= 0 && col < GRID_SIZE_X) {
                AddCircle(row, col);
                InvalidateRect(hwnd, NULL, TRUE);
            }
            return 0;
        }

        case WM_RBUTTONDOWN: {
            // Правый клик - рисование крестика
            int xPos = LOWORD(lParam);
            int yPos = HIWORD(lParam);

            RECT clientRect;
            GetClientRect(hwnd, &clientRect);

            int cellWidth = clientRect.right / GRID_SIZE_X;
            int cellHeight = clientRect.bottom / GRID_SIZE_Y;

            int col = xPos / cellWidth;
            int row = yPos / cellHeight;

            if (row >= 0 && row < GRID_SIZE_Y && col >= 0 && col < GRID_SIZE_X) {
                AddCross(row, col);
                InvalidateRect(hwnd, NULL, TRUE);
            }
            return 0;
        }

        case WM_MOUSEWHEEL: {
            // Колесико мыши - изменение цвета сетки
            int delta = GET_WHEEL_DELTA_WPARAM(wParam);
            int steps = delta / WHEEL_DELTA;
            UpdateGridColor(steps * 16);
            InvalidateRect(hwnd, NULL, TRUE);
            return 0;
        }

        // Обработка клавиатуры
        case WM_KEYDOWN: {
            bool ctrlPressed = (GetKeyState(VK_CONTROL) & 0x8000) != 0;
            bool shiftPressed = (GetKeyState(VK_SHIFT) & 0x8000) != 0;

            if (ctrlPressed && wParam == 'Q') {
                DestroyWindow(hwnd);
            }
            else if (shiftPressed && wParam == 'C') {
                ShellExecuteA(NULL, "open", "notepad.exe", NULL, NULL, SW_SHOWNORMAL);
            }
            else if (wParam == VK_RETURN) {
                BACKGROUND_COLOR = GetRandomColor();
                UpdateBackgroundBrush(hwnd);
            }
            return 0;
        }

        // Обработка рисовки
        case WM_PAINT: {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hwnd, &ps);

            // Получаем размеры области
            RECT clientRect;
            GetClientRect(hwnd, &clientRect);
            int width = clientRect.right;
            int height = clientRect.bottom;

            // Вычисляем реальный размер клетки
            int cellWidth = width / GRID_SIZE_X;
            int cellHeight = height / GRID_SIZE_Y;

            // Рисуем сетку текущим цветом
            HPEN gridPen = CreatePen(PS_SOLID, 1, RGB(GRID_COLOR_R, GRID_COLOR_G, GRID_COLOR_B));
            HPEN oldPen = (HPEN)SelectObject(hdc, gridPen);

            // Вертикальные линии
            for (int i = 1; i < GRID_SIZE_X; i++) {
                int x = i * cellWidth;
                MoveToEx(hdc, x, 0, NULL);
                LineTo(hdc, x, height);
            }

            // Горизонтальные линии
            for (int i = 1; i < GRID_SIZE_Y; i++) {
                int y = i * cellHeight;
                MoveToEx(hdc, 0, y, NULL);
                LineTo(hdc, width, y);
            }

            SelectObject(hdc, oldPen);
            DeleteObject(gridPen);

            // Рисуем все фигуры
            for (int row = 0; row < GRID_SIZE_Y; row++) {
                for (int col = 0; col < GRID_SIZE_X; col++) {
                    if (HasCircle(row, col)) {
                        DrawCircleInCell(hdc, row, col, cellWidth, cellHeight);
                    }
                    if (HasCross(row, col)) {
                        DrawCrossInCell(hdc, row, col, cellWidth, cellHeight);
                    }
                }
            }

            EndPaint(hwnd, &ps);
            return 0;
        }

        // Обработка выхода
        case WM_DESTROY:
            // Сохраняем конфигурацию перед выходом
            SaveConfigStream(hwnd);
            PostQuitMessage(0);
            return 0;

        // По умолчанию
        default:
            return DefWindowProcA(hwnd, uMsg, wParam, lParam);
    }
}
#include "WindowProc.h"

#include <format>
#include <iostream>

#include "../Grid/Grid.h"
#include "../Colors/Colors.h"
#include "../Config/Config.h"
#include <shellapi.h>
#include "../main.h"
int win_count = 1;
UINT WM_UPDATE_GRID = RegisterWindowMessageA("Update");

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    switch (uMsg) {
        
        // Обработка изменения размера окна
        case WM_SIZE: {
            // При изменении размера окна пересчитываем сетку
            //RecalculateGrid(hwnd);
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

            if (hMutex) {
                WaitForSingleObject(hMutex, INFINITE);

                if (row >= 0 && row < GRID_SIZE_Y && col >= 0 && col < GRID_SIZE_X) {
                    AddCircle(row, col);
                }

                ReleaseMutex(hMutex);
            }

            PostMessage(HWND_BROADCAST, WM_UPDATE_GRID, 0, 0);
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

            if (hMutex) {
                WaitForSingleObject(hMutex, INFINITE);

                if (row >= 0 && row < GRID_SIZE_Y && col >= 0 && col < GRID_SIZE_X) {
                    AddCross(row, col);
                }

                ReleaseMutex(hMutex);
            }

            PostMessage(HWND_BROADCAST, WM_UPDATE_GRID, 0, 0);
            return 0;
        }

        case WM_MOUSEWHEEL: {
            // Колесико мыши - изменение цвета сетки
            if (hMutex) {
                WaitForSingleObject(hMutex, INFINITE);

                int delta = GET_WHEEL_DELTA_WPARAM(wParam);
                int steps = delta / WHEEL_DELTA;
                steps = std::abs(steps);
                UpdateGridColor(steps * 16);

                ReleaseMutex(hMutex);
            }

            PostMessage(HWND_BROADCAST, WM_UPDATE_GRID, 0, 0);
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
                if (hMutex) {
                    WaitForSingleObject(hMutex, INFINITE);

                    sharedData->backgroundColor = GetRandomColor();
                    UpdateBackgroundBrush(hwnd);

                    ReleaseMutex(hMutex);
                }
                PostMessage(HWND_BROADCAST, WM_UPDATE_GRID, 0, 0);
            }
            else if (ctrlPressed && wParam == 'N') {
                WaitForSingleObject(hMutex, INFINITE);
                int currentIdx = sharedData->WindowsCreated;
                sharedData->WindowsCreated++;
                ReleaseMutex(hMutex);

                int screenWidth = GetSystemMetrics(SM_CXSCREEN);
                int windowW = WINDOW_WIDTH_CONFIG;
                int windowH = WINDOW_HEIGHT_CONFIG;

                int cols = screenWidth / windowW;


                int x = (currentIdx % cols) * windowW;
                int y = (currentIdx / cols) * windowH;

                int screenHeight = GetSystemMetrics(SM_CYSCREEN);
                y %= (screenHeight - windowH);

                HWND newHwnd = CreateWindowA(
                    "MainWindowClass",
                    std::format("Window {}", currentIdx + 1).c_str(),
                    WS_OVERLAPPEDWINDOW | WS_VISIBLE,
                    x, y,
                    windowW, windowH,
                    NULL, NULL, GetModuleHandle(NULL), NULL
                );
                Windows.push_back(newHwnd);
                InitBackgroundBrush(newHwnd);
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
            HPEN gridPen = CreatePen(PS_SOLID, 1, RGB(sharedData->GRID_COLOR_R,
                                          sharedData->GRID_COLOR_G,
                                          sharedData->GRID_COLOR_B));
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
        case WM_DESTROY: {
            // Ищем текущее окно в векторе и удаляем его
            for (auto it = Windows.begin(); it != Windows.end(); ++it) {
                if (*it == hwnd) {
                    Windows.erase(it);
                    break;
                }
            }

            if (Windows.empty()) {
                SaveConfig(hwnd);
                PostQuitMessage(0);
            }
            break;
        }
        case WM_GETMINMAXINFO: {
            LPMINMAXINFO lpMMI = (LPMINMAXINFO)lParam;

            lpMMI->ptMinTrackSize.x = WINDOW_WIDTH_CONFIG;
            lpMMI->ptMinTrackSize.y = WINDOW_HEIGHT_CONFIG;

            lpMMI->ptMaxTrackSize.x = WINDOW_WIDTH_CONFIG;
            lpMMI->ptMaxTrackSize.y = WINDOW_HEIGHT_CONFIG;

            return 0;
        }


        // По умолчанию
        default:
            if (uMsg == WM_UPDATE_GRID) {
                UpdateBackgroundBrush(hwnd);
                InvalidateRect(hwnd, NULL, TRUE);
                return 0;
            }
            return DefWindowProcA(hwnd, uMsg, wParam, lParam);
    }
    return 0;
}
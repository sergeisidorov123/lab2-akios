#include <windows.h>
#include "Grid/Grid.h"
#include "Config/Config.h"
#include "Colors/Colors.h"

const int WINDOW_WIDTH = 320;
const int WINDOW_HEIGHT = 240;
const int MAX_CELL_SIZE = 100;

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR args, int CmdShow) {
    // Загружаем конфиг
    LoadConfigFile();

    // Обработка аргументов командной строки
    int argc = __argc;
    char** argv = __argv;
    if (argc > 1) {
        int cellSize = atoi(argv[1]);
        if (cellSize >= MIN_CELL_SIZE & cellSize <= MAX_CELL_SIZE) {
            CELL_SIZE = cellSize;
        }
    }

    // Регистрация класса окна
    WNDCLASSA wc = {0};
    wc.lpfnWndProc = WindowProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = "MainWindowClass";
    wc.hCursor = LoadCursorA(NULL, IDC_ARROW);
    wc.hIcon = LoadIconA(NULL, IDI_APPLICATION);
    wc.hbrBackground = NULL;
    wc.style = CS_HREDRAW | CS_VREDRAW;

    if (!RegisterClassA(&wc)) {
        MessageBoxA(NULL, "Failed to register window class!", "Error", MB_OK);
        return -1;
    }

    // Создание окна
    HWND hwnd = CreateWindowA(
        "MainWindowClass",
        "Grid Application",
        WS_OVERLAPPEDWINDOW | WS_VISIBLE,
        CW_USEDEFAULT, CW_USEDEFAULT,
        WINDOW_WIDTH_CONFIG, WINDOW_HEIGHT_CONFIG,
        NULL, NULL, hInstance, NULL
    );

    if (!hwnd) {
        MessageBoxA(NULL, "Failed to create window!", "Error", MB_OK);
        return -1;
    }

    // Инициализация кисти фона
    InitBackgroundBrush(hwnd);

    // Главный цикл
    MSG msg = {0};
    while (GetMessageA(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessageA(&msg);
    }

    // Очистка
    CleanupBackgroundBrush();
    return 0;
}
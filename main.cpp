#include <iostream>
#include <ostream>
#include <string>
#include <windows.h>
#include "Grid/Grid.h"
#include "Config/Config.h"
#include "Colors/Colors.h"
#include <chrono>
#include <numeric>
#include <fstream>

const int WINDOW_WIDTH = 320;
const int WINDOW_HEIGHT = 240;
const int MAX_CELL_SIZE = 100;

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

void RunBenchmark() {
    std::string benchName = "bench.txt";
    std::vector<char> Data(1024 * 1024, 'A');
    std::ofstream ofs("bench.txt", std::ios::binary);
    ofs.write(Data.data(), Data.size());
    ofs.close();

    std::string originalFile = CURRENT_FILE;
    CURRENT_FILE = benchName;

    for (int method = 1; method <= 4; ++method) {
        std::vector<double> times;
        std::cout << "\n--- Method " << method << " ---" << std::endl;

        for (int i = 1; i <= 20; ++i) {
            auto start = std::chrono::high_resolution_clock::now();

            if (method == 1) LoadConfigMappingBench();
            else if (method == 2) LoadConfigFile();
            else if (method == 3) LoadConfigStream();
            else if (method == 4) LoadConfigWinAPI();

            auto end = std::chrono::high_resolution_clock::now();
            std::chrono::duration<double, std::milli> elapsed = end - start;

            times.push_back(elapsed.count());
            std::cout << "Iteration " << i << ": " << elapsed.count() << " ms" << std::endl;
        }

        double sum = std::accumulate(times.begin(), times.end(), 0.0);
        std::cout << ">> AVERAGE for Method " << method << ": " << sum / 20.0 << " ms" << std::endl;
    }
    CURRENT_FILE = originalFile;
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR args, int CmdShow) {
    LoadConfig();
    bool runBench = false;
    int argc = __argc;
    char** argv = __argv;
    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];

        if (arg == "-m" && i + 1 < argc) {
            int m = atoi(argv[i + 1]);
            if (m >= 1 && m <= 4) {
                currentMethod = (ConfigMethod)m;
            }
            i++;
        }
        else if (arg == "-s" && i + 1 < argc) {
            int s = atoi(argv[i + 1]);
            if (s >= MIN_CELL_SIZE && s <= MAX_CELL_SIZE) {
                CELL_SIZE = s;
            }
            i++;
        }
        else if (std::string(argv[i]) == "-bench") {
            runBench = true;
            break;
        }
    }
    std::string method = "";
    switch (currentMethod) {
        case MAPPING:
            method =  "File Mapping";
            break;
        case CFILE:
            method =  "FILE";
            break;
        case STREAM:
            method = "STREAM";
            break;
        case WinAPI:
            method = "WinAPI";
            break;
    }

    AllocConsole();
    freopen("CONOUT$", "w", stdout);
    std::cout << "Method:" << method << std::endl;
    std::cout << "Cell size:" << CELL_SIZE << std::endl;



    if (runBench) {
        AllocConsole();
        freopen("CONOUT$", "w", stdout);
        freopen("CONIN$", "r", stdin);

        RunBenchmark();

        return 0;
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
#include "Config.h"
#include "../Grid/Grid.h"
#include "../Colors/Colors.h"
#include <fstream>
#include <string>
#include <sstream>
#include <windows.h>

const char* CONFIG_FILE = "config.txt";
const int MIN_CELL_SIZE = 10;
const int DEFAULT_CELL_SIZE = 20;
const int MAX_DEFAULT_SIZE = 100;
int WINDOW_WIDTH_CONFIG = 320;
int WINDOW_HEIGHT_CONFIG = 240;

void ParseData(std::istream& file) {
    std::string line;
    while (std::getline(file, line)) {
        std::istringstream iss(line);
        std::string key;
        iss >> key;

        if (key == "CELL_SIZE:") {
            iss >> CELL_SIZE;
            if (CELL_SIZE < MIN_CELL_SIZE) CELL_SIZE = DEFAULT_CELL_SIZE;
            if (CELL_SIZE > MAX_DEFAULT_SIZE) CELL_SIZE = MAX_DEFAULT_SIZE;
        }
        else if (key == "WINDOW_WIDTH:") {
            iss >> WINDOW_WIDTH_CONFIG;
        }
        else if (key == "WINDOW_HEIGHT:") {
            iss >> WINDOW_HEIGHT_CONFIG;
        }
        else if (key == "BACKGROUND_COLOR:") {
            int r, g, b;
            iss >> r >> g >> b;
            BACKGROUND_COLOR = RGB(r, g, b);
        }
        else if (key == "GRID_COLOR:") {
            iss >> GRID_COLOR_R >> GRID_COLOR_G >> GRID_COLOR_B;
        }
    }
}

void SaveData(RECT windowRect, std::ostream& file) {
    file << "CELL_SIZE: " << CELL_SIZE << std::endl;
    file << "WINDOW_WIDTH: " << (windowRect.right - windowRect.left) << std::endl;
    file << "WINDOW_HEIGHT: " << (windowRect.bottom - windowRect.top) << std::endl;
    file << "BACKGROUND_COLOR: "
         << (int)GetRValue(BACKGROUND_COLOR) << " "
         << (int)GetGValue(BACKGROUND_COLOR) << " "
         << (int)GetBValue(BACKGROUND_COLOR) << std::endl;
    file << "GRID_COLOR: "
         << GRID_COLOR_R << " "
         << GRID_COLOR_G << " "
         << GRID_COLOR_B << std::endl;
}
// 1 Вариант


// Загрузка конфига при помощи отображения файлов на память
void LoadConfigMapping() {
    HANDLE hFile = CreateFileA(CONFIG_FILE, GENERIC_READ, FILE_SHARE_READ, NULL,
        OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
    if (hFile == INVALID_HANDLE_VALUE) return;

    DWORD fileSize = GetFileSize(hFile, NULL);
    if (fileSize == INVALID_FILE_SIZE) { CloseHandle(hFile); return; }

    HANDLE hMapping = CreateFileMapping(hFile, NULL, PAGE_READONLY, 0,
        0, NULL);
    if (hMapping == NULL) { CloseHandle(hFile); return; }

    char* pData = (char*)MapViewOfFile(hMapping, FILE_MAP_READ, 0, 0, 0);
    if (pData) {
        std::string content(pData, fileSize);
        std::istringstream file(content);

        ParseData(file);

        UnmapViewOfFile(pData);
    }

    CloseHandle(hMapping);
    CloseHandle(hFile);
}

// Создание конфига при помощи отображения файлов на память
void SaveConfigMapping(HWND hwnd) {
    RECT windowRect;
    GetWindowRect(hwnd, &windowRect);

    std::ostringstream file;
    SaveData(windowRect, file);
    std::string data = file.str();

    HANDLE hFile = CreateFileA(CONFIG_FILE, GENERIC_READ | GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
    DWORD fileSize = (DWORD)data.size();
    SetFilePointer(hFile, fileSize, NULL, FILE_BEGIN);
    SetEndOfFile(hFile);


    HANDLE hMapping = CreateFileMapping(hFile, NULL, PAGE_READWRITE, 0, fileSize, NULL);

    char* pData = (char*)MapViewOfFile(hMapping, FILE_MAP_WRITE, 0, 0, fileSize);
    if (pData) {
        memcpy(pData, data.c_str(), fileSize);
        UnmapViewOfFile(pData);
    }

    CloseHandle(hMapping);
    CloseHandle(hFile);
}


// 2 Вариант


// Загрузка конфига при помощи файловых переменных
void LoadConfigFile() {
    FILE* file = fopen(CONFIG_FILE, "rb");
    if (!file) return;

    fseek(file, 0, SEEK_END);
    long fileSize = ftell(file);
    fseek(file, 0, SEEK_SET);

    if (fileSize > 0) {
        std::string buffer(fileSize, '\0');
        fread(&buffer[0], 1, fileSize, file);

        std::istringstream file(buffer);
        ParseData(file);
    }

    fclose(file);
}

// Чтение конфига при помощи файловых переменных
void SaveConfigFile(HWND hwnd) {
    RECT windowRect;
    GetWindowRect(hwnd, &windowRect);

    std::ostringstream file;
    SaveData(windowRect, file);
    std::string data = file.str();

    FILE* save = fopen(CONFIG_FILE, "wb");
    if (!save) return;

    fwrite(data.c_str(), sizeof(char), data.size(), save);

    fclose(save);
}

// 3 Вариант


// Загрузка конфига при помощи потоков ввода-вывода
void LoadConfigStream() {
    std::ifstream file(CONFIG_FILE);
    if (!file.is_open()) return;

    ParseData(file);
    file.close();
}


// Сохранение конфига при помощи потоков ввода-вывода
void SaveConfigStream(HWND hwnd) {
    RECT windowRect;
    GetWindowRect(hwnd, &windowRect);

    std::ofstream file(CONFIG_FILE);
    if (!file.is_open()) return;

    SaveData(windowRect, file);

    file.close();
}


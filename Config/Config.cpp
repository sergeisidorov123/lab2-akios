#include "Config.h"
#include "../Grid/Grid.h"
#include "../Colors/Colors.h"
#include <fstream>
#include <string>
#include <sstream>

const char* CONFIG_FILE = "config.txt";
const int MIN_CELL_SIZE = 10;
const int DEFAULT_CELL_SIZE = 20;
int WINDOW_WIDTH_CONFIG = 320;
int WINDOW_HEIGHT_CONFIG = 240;

// Загрузка конфига с построчным прочтением параметров
void LoadConfig() {
    std::ifstream file(CONFIG_FILE);
    if (!file.is_open()) return;

    std::string line;
    while (std::getline(file, line)) {
        std::istringstream iss(line);
        std::string key;
        iss >> key;

        if (key == "CELL_SIZE:") {
            iss >> CELL_SIZE;
            if (CELL_SIZE < MIN_CELL_SIZE) CELL_SIZE = DEFAULT_CELL_SIZE;
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
    file.close();
}


// Сохранение конфига
void SaveConfig(HWND hwnd) {
    RECT windowRect;
    GetWindowRect(hwnd, &windowRect);

    std::ofstream file(CONFIG_FILE);
    if (!file.is_open()) return;

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

    file.close();
}
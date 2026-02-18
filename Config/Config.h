#ifndef CONFIG_H
#define CONFIG_H

#include <windows.h>

extern const char* CONFIG_FILE;
extern const int MIN_CELL_SIZE;
extern const int DEFAULT_CELL_SIZE;

void LoadConfig();
void SaveConfig(HWND hwnd);

#endif
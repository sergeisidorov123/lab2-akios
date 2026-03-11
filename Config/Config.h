#ifndef CONFIG_H
#define CONFIG_H

#include <windows.h>

extern const char* CONFIG_FILE;
extern const int MIN_CELL_SIZE;
extern const int DEFAULT_CELL_SIZE;
extern int WINDOW_WIDTH_CONFIG;
extern int WINDOW_HEIGHT_CONFIG;

void LoadConfigStream();
void SaveConfigStream(HWND hwnd);
void LoadConfigMapping();
void SaveConfigMapping(HWND hwnd);

#endif
#ifndef CONFIG_H
#define CONFIG_H

#include <windows.h>
#include <string>

extern std::string CURRENT_FILE;
extern const int MIN_CELL_SIZE;
extern const int DEFAULT_CELL_SIZE;
extern int WINDOW_WIDTH_CONFIG;
extern int WINDOW_HEIGHT_CONFIG;
enum ConfigMethod {
    MAPPING = 1,
    CFILE = 2,
    STREAM = 3,
    WinAPI  = 4
};
extern ConfigMethod currentMethod;


void LoadConfig();
void SaveConfig(HWND hwnd);
void LoadConfigMapping();
void SaveConfigMapping(HWND hwnd);
void LoadConfigStream();
void SaveConfigStream(HWND hwnd);
void LoadConfigFile();
void SaveConfigFile(HWND hwnd);
void LoadConfigWinAPI();
void SaveConfigWinAPI(HWND hwnd);
void LoadConfigMappingBench();

#endif

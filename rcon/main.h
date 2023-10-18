#pragma once

#pragma comment(lib, "wsock32.lib")

#include <windows.h>
#include <conio.h>
#include <stdio.h>

extern bool bQuitApp;

void logprintf(char* format, ...);
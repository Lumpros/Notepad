#pragma once

#include <Windows.h>

void HandleMenuCommand(HWND hWnd, WPARAM wParam, LPARAM lParam);

CHOOSEFONT GetChooseFontFromDialog(HWND hWnd);
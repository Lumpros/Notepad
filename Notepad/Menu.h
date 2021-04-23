#pragma once

#include <Windows.h>

#include "Resource.h"

void InitializeMenu(HWND hWnd);
void HandleMenuCommand(HWND hWnd, WPARAM wParam, LPARAM lParam);

CHOOSEFONT GetChooseFontFromDialog(HWND hWnd);
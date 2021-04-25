#pragma once

#include <Windows.h>

#define DEFAULT_FONT_STR L"Consolas"

void CreateControls(HWND hWnd);

void HandleWindowResize(HWND hWnd, WPARAM wParam, LPARAM lParam);

void EnableStatusBar(HWND hWnd);
void DisableStatusBar(HWND hWnd);
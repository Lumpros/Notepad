#pragma once

#include <Windows.h>

#define DEFAULT_FONT_STR L"Consolas"

void CreateControls(HWND hWnd);

void HandleWindowResize(HWND hWnd, WPARAM wParam, LPARAM lParam);

void EnableStatusBar(HWND hWnd);

void DisableStatusBar(HWND hWnd);

BOOL HasChangedOriginalText(void);

void DecrementChangeCount(HWND hWnd);

TEXTMETRIC GetSystemFontMetrics(HWND hWnd);

UINT GetStatusBarHeight(HWND hWnd);

void SaveHFont(HFONT hFont);

HFONT RetrieveHFont(void);

void ResizeMainEditControl(HWND hWnd, int width, int height);

void SetEditControlWindProc(HWND hEditControl);
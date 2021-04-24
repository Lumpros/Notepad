#pragma once

#include <Windows.h>

void AppendText(LPCWSTR newText, HWND main_window_hWnd);
void InsertText(LPCWSTR newText, HWND main_window_hWnd);

void HandleEditMenu(HWND hWnd, WPARAM wParam, LPARAM lParam);
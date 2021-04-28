#pragma once

#include <Windows.h>

void AppendText(LPCWSTR newText, HWND main_window_hWnd);
void InsertText(LPCWSTR newText, HWND main_window_hWnd);
void HandleEditMenu(HWND hWnd, WPARAM wParam, LPARAM lParam);
void HandlePossibleTextSelect(HWND hWnd, WPARAM wParam, LPARAM lParam);
void EnableTextEditMenuItems(HWND hWnd, BOOL enabled);
void SetLineColumnStatusBar(HWND hWnd);

BOOL SelectionHasChanged(UINT message, LPARAM lParam);
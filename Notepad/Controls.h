#pragma once

#include <Windows.h>

#define IDC_TEXT_EDIT 1
#define DEFAULT_FONT_STR L"Consolas"

extern HWND text_edit_handle;

void CreateControls(HWND hWnd);
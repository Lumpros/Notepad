#pragma once

#include <Windows.h>

void HandleViewMenu(HWND hWnd, WPARAM wParam, LPARAM lParam);

void RefreshZoom(HWND hWnd);

void UpdateZoom(HWND hWnd, SHORT bWheelDelta);
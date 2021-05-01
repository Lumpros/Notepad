#include "ViewMenu.h"
#include "Identifiers.h"
#include "Controls.h"

#include <CommCtrl.h>

// Because it's a bit zoomed out by default for some reason...
#define ZOOM_DENOMINATOR 90

int zoom_numerator = 100;

static void SetStatusBarZoomText(HWND hWnd)
{
	HWND hStatusBar = GetDlgItem(hWnd, IDC_STATUS_BAR);

	WCHAR buf[8];
	wsprintf(buf, L" %d%%", zoom_numerator);

	SendMessage(hStatusBar, SB_SETTEXT, 2, (LPARAM)buf);
}

static void RequestZoom(HWND hWnd)
{
	HWND hControl = GetDlgItem(hWnd, IDC_TEXT_EDIT);
	LRESULT bAccepted = SendMessage(hControl, EM_SETZOOM, (WPARAM)zoom_numerator, (LPARAM)ZOOM_DENOMINATOR);

	if (!bAccepted)
		MessageBox(NULL, L"Request to change zoom level was denied!", L"Error", MB_OK | MB_ICONERROR);
	else
		SetStatusBarZoomText(hWnd);
}

static void ZoomIn(HWND hWnd)
{
	zoom_numerator += 10;

	if (zoom_numerator > 500)
	{
		zoom_numerator = 500;
	}

	else
	{
		RequestZoom(hWnd);
	}
}

static void ZoomOut(HWND hWnd)
{
	zoom_numerator -= 10;

	if (zoom_numerator < 10)
	{
		zoom_numerator = 10;
	}

	else // has changed
	{
		RequestZoom(hWnd);
	}
}

static void HandleStatusBarToggle(HWND hWnd)
{
	HMENU hMenu = GetMenu(hWnd);
	DWORD checkState = CheckMenuItem(hMenu, IDM_VIEW_STATUS, MF_BYCOMMAND);

	if (checkState == MF_CHECKED)
	{
		DisableStatusBar(hWnd);
		CheckMenuItem(hMenu, IDM_VIEW_STATUS, MF_UNCHECKED);
	}

	else if (checkState == MF_UNCHECKED)
	{
		EnableStatusBar(hWnd);
		CheckMenuItem(hMenu, IDM_VIEW_STATUS, MF_CHECKED);
	}
}

static void RestoreZoomLevel(HWND hWnd)
{
	zoom_numerator = 100;
	RequestZoom(hWnd);
}

void RefreshZoom(HWND hWnd)
{
	RequestZoom(hWnd);
}

void HandleViewMenu(HWND hWnd, WPARAM wParam, LPARAM lParam)
{
	switch (LOWORD(wParam))
	{
	case IDM_VIEW_STATUS:
		HandleStatusBarToggle(hWnd);
		break;

	case IDM_VIEW_ZOOM_IN:
		ZoomIn(hWnd);
		break;

	case IDM_VIEW_ZOOM_OUT:
		ZoomOut(hWnd);
		break;

	case IDM_VIEW_RESTORE:
		RestoreZoomLevel(hWnd);
		break;
	}
}
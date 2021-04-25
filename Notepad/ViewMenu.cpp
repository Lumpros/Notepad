#include "ViewMenu.h"
#include "Identifiers.h"
#include "Controls.h"

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

	InvalidateRect(hWnd, NULL, FALSE);
}

void HandleViewMenu(HWND hWnd, WPARAM wParam, LPARAM lParam)
{
	switch (LOWORD(wParam))
	{
	case IDM_VIEW_STATUS:
		HandleStatusBarToggle(hWnd);
		break;
	}
}
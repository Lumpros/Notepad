#include "AboutMenu.h"
#include "Identifiers.h"
#include "Resource.h"

INT_PTR CALLBACK AboutMenuProcedure(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_CLOSE:
		EndDialog(hWnd, EXIT_SUCCESS);
		break;

	case WM_COMMAND:
		switch (wParam)
		{
		case IDOK:
			EndDialog(hWnd, IDOK);
			break;
		}
		break;
	}

	return 0;
}

void HandleAboutMenu(HWND hWnd, WPARAM wParam, LPARAM lParam)
{
	switch (LOWORD(wParam))
	{
	case IDM_HELP_ABOUT:
		DialogBox(NULL, MAKEINTRESOURCE(IDD_ABOUT_DIALOG), hWnd, AboutMenuProcedure);
		break;
	}
}
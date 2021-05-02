#include "Menu.h"
#include "Controls.h"
#include "Identifiers.h"

#include "FileMenu.h"
#include "EditMenu.h"
#include "FormatMenu.h"
#include "ViewMenu.h"
#include "AboutMenu.h"

inline static BOOL IsInRange(WPARAM val, const WORD left_incl, const WORD right_incl)
{
	return (val >= left_incl && val <= right_incl);
}

void HandleMenuCommand(HWND hWnd, WPARAM wParam, LPARAM lParam)
{
	WPARAM ID = LOWORD(wParam);

	if (IsInRange(ID, IDM_FILE_NEW, IDM_FILE_EXIT))
	{
		HandleFileMenu(hWnd, wParam, lParam);
	}

	else if (IsInRange(ID, IDM_EDIT_UNDO, IDM_EDIT_FIND_PREV) || IsInRange(ID, IDM_EDIT_REPLACE, IDM_EDIT_TIME_DATE))
	{
		HandleEditMenu(hWnd, wParam, lParam);
	}

	else if (IsInRange(ID, IDM_FORMAT_WORDWRAP, IDM_FORMAT_FONT))
	{
		HandleFormatMenu(hWnd, wParam, lParam);
	}

	else if (IsInRange(ID, IDM_HELP_VIEW, IDM_HELP_ABOUT))
	{
		HandleAboutMenu(hWnd, wParam, lParam);
	}

	else if (IsInRange(ID, IDM_VIEW_ZOOM_IN, IDM_VIEW_RESTORE) || (ID == (WORD)IDM_VIEW_STATUS))
	{
		HandleViewMenu(hWnd, wParam, lParam);
	}
}
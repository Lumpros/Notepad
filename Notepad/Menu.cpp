#include "Menu.h"
#include "Controls.h"
#include "Identifiers.h"

#include "FileMenu.h"
#include "EditMenu.h"
#include "FormatMenu.h"
#include "ViewMenu.h"
#include "AboutMenu.h"

inline static BOOL IsInRange(WORD val, WORD left_incl, WORD right_incl)
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

static void InitializeFileSubmenu(HMENU hMenu)
{
	HMENU hFileSubmenu = CreatePopupMenu();

	AppendMenu(hFileSubmenu, MF_STRING, IDM_FILE_NEW, L"New\tCtrl+N");
	AppendMenu(hFileSubmenu, MF_STRING, IDM_FILE_NEW_WINDOW, L"New Window\tCtrl+Shift+N");
	AppendMenu(hFileSubmenu, MF_STRING, IDM_FILE_OPEN, L"Open...\tCtrl+O");
	AppendMenu(hFileSubmenu, MF_STRING, IDM_FILE_SAVE, L"Save\tCtrl+S");
	AppendMenu(hFileSubmenu, MF_STRING, IDM_FILE_SAVE_AS, L"Save as...\tCtrl+Shift+S");
	AppendMenu(hFileSubmenu, MF_SEPARATOR, NULL, NULL);
	AppendMenu(hFileSubmenu, MF_STRING, IDM_FILE_PAGE_SETUP, L"Page Setup");
	AppendMenu(hFileSubmenu, MF_STRING, IDM_FILE_PRINT, L"Print...\tCtrl+P");
	AppendMenu(hFileSubmenu, MF_SEPARATOR, NULL, NULL);
	AppendMenu(hFileSubmenu, MF_STRING, IDM_FILE_EXIT, L"E&xit");

	AppendMenu(hMenu, MF_POPUP | MF_STRING, (UINT)hFileSubmenu, L"&File");
}

static void InitializeEditSubmenu(HMENU hMenu)
{
	HMENU hEditSubmenu = CreatePopupMenu();

	AppendMenu(hEditSubmenu, MF_STRING, IDM_EDIT_UNDO, L"Undo\tCtrl+Z");
	AppendMenu(hEditSubmenu, MF_SEPARATOR, NULL, NULL);
	AppendMenu(hEditSubmenu, MF_STRING, IDM_EDIT_CUT, L"Cut\tCtrl+X");
	AppendMenu(hEditSubmenu, MF_STRING, IDM_EDIT_COPY, L"Copy\tCtrl+C");
	AppendMenu(hEditSubmenu, MF_STRING, IDM_EDIT_PASTE, L"Paste\tCtrl+V");
	AppendMenu(hEditSubmenu, MF_STRING, IDM_EDIT_DELETE, L"Delete\tDel");
	AppendMenu(hEditSubmenu, MF_SEPARATOR, NULL, NULL);
	AppendMenu(hEditSubmenu, MF_STRING, IDM_EDIT_SEARCH, L"Search with Bing...\tCtrl+E");
	AppendMenu(hEditSubmenu, MF_STRING, IDM_EDIT_FIND, L"Find...\tCtrl+F");
	AppendMenu(hEditSubmenu, MF_STRING, IDM_EDIT_FIND_NEXT, L"Find Next\tF3");
	AppendMenu(hEditSubmenu, MF_STRING, IDM_EDIT_FIND_PREV, L"Find Previous\tShift+F3");
	AppendMenu(hEditSubmenu, MF_STRING, IDM_EDIT_REPLACE, L"Replace...\tCtrl+H");
	AppendMenu(hEditSubmenu, MF_STRING, IDM_EDIT_GOTO, L"Go To...\tCtrl+G");
	AppendMenu(hEditSubmenu, MF_SEPARATOR, NULL, NULL);
	AppendMenu(hEditSubmenu, MF_STRING, IDM_EDIT_SELECTALL, L"Select All\tCtrl+A");
	AppendMenu(hEditSubmenu, MF_STRING, IDM_EDIT_TIME_DATE, L"Time/Date\tF5");

	AppendMenu(hMenu, MF_POPUP | MF_STRING, (UINT)hEditSubmenu, L"&Edit");

	EnableMenuItem(hMenu, IDM_EDIT_GOTO, MF_DISABLED | MF_GRAYED | MF_BYCOMMAND);
}

static void InitializeFormatSubmenu(HMENU hMenu)
{
	HMENU hFormatSubmenu = CreatePopupMenu();

	AppendMenu(hFormatSubmenu, MF_STRING, IDM_FORMAT_WORDWRAP, L"Word Wrap");
	AppendMenu(hFormatSubmenu, MF_STRING | MF_UNCHECKED, IDM_FORMAT_FONT, L"Font");

	AppendMenu(hMenu, MF_POPUP | MF_STRING, (UINT)hFormatSubmenu, L"F&ormat");
}

static void InitializeViewSubmenu(HMENU hMenu)
{
	HMENU hViewSubmenu = CreatePopupMenu();
	HMENU hViewZoomSubmenu = CreatePopupMenu();

	AppendMenu(hViewZoomSubmenu, MF_STRING, IDM_VIEW_ZOOM_IN, L"Zoom In\tCtrl+Plus");
	AppendMenu(hViewZoomSubmenu, MF_STRING, IDM_VIEW_ZOOM_OUT, L"Zoom Out\tCtrl+Minus");
	AppendMenu(hViewZoomSubmenu, MF_STRING, IDM_VIEW_RESTORE, L"Restore Default Zoom\tCtrl+0");
	AppendMenu(hViewSubmenu, MF_POPUP | MF_STRING, (UINT)hViewZoomSubmenu, L"Zoom");
	AppendMenu(hViewSubmenu, MF_STRING, IDM_VIEW_STATUS, L"Status Bar");

	AppendMenu(hMenu, MF_POPUP | MF_STRING, (UINT)hViewSubmenu, L"&View");

	CheckMenuItem(hViewSubmenu, IDM_VIEW_STATUS, MF_CHECKED);
}

static void InitializeHelpSubmenu(HMENU hMenu)
{
	HMENU hHelpSubmenu = CreatePopupMenu();

	AppendMenu(hHelpSubmenu, MF_STRING, IDM_HELP_VIEW, L"View Help");
	AppendMenu(hHelpSubmenu, MF_STRING, IDM_HELP_FEEDBACK, L"Send Feedback");
	AppendMenu(hHelpSubmenu, MF_SEPARATOR, NULL, NULL);
	AppendMenu(hHelpSubmenu, MF_STRING, IDM_HELP_ABOUT, L"About Notepad");

	AppendMenu(hMenu, MF_POPUP | MF_STRING, (UINT)hHelpSubmenu, L"&Help");
}

void InitializeMenu(HWND hWnd)
{
	HMENU hMenu = CreateMenu();	

	InitializeFileSubmenu(hMenu);
	InitializeEditSubmenu(hMenu);
	InitializeFormatSubmenu(hMenu);
	InitializeViewSubmenu(hMenu);
	InitializeHelpSubmenu(hMenu);

	SetMenu(hWnd, hMenu);

	EnableTextEditMenuItems(hWnd, FALSE);
	EnableMenuItem(GetMenu(hWnd), IDM_EDIT_UNDO, MF_BYCOMMAND | MF_GRAYED | MF_DISABLED);
}
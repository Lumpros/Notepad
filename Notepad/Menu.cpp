#include "Menu.h"
#include "Controls.h"

CHOOSEFONT GetChooseFontFromDialog(HWND hWnd)
{
	CHOOSEFONT cf = { 0 };
	LOGFONT lf = { 0 };
	static LOGFONT oldlf;
	static bool initialized = false;

	cf.lStructSize = sizeof(CHOOSEFONT);
	cf.hwndOwner = hWnd;
	cf.nFontType = SCREEN_FONTTYPE;
	cf.Flags =  CF_INITTOLOGFONTSTRUCT;
	cf.rgbColors = RGB(0, 0, 0);

	/* Do all this dumb stuff to save previous choices */
	if (initialized)
	{
		cf.lpLogFont = &oldlf;
		lstrcpyW(cf.lpLogFont->lfFaceName, oldlf.lfFaceName);
	}

	else
	{
		cf.lpLogFont = &lf;
		cf.lpLogFont->lfHeight = 20;
		lstrcpyW(cf.lpLogFont->lfFaceName, DEFAULT_FONT_STR);
	}

	ChooseFont(&cf);

	if (!initialized)
	{
		oldlf = lf;
		initialized = true;
	}

	return cf;
}

static void SetTextEditFont(HWND hWnd)
{
	CHOOSEFONT cf = GetChooseFontFromDialog(hWnd);
	HFONT hFont = CreateFontIndirect(cf.lpLogFont);
	SendMessage(text_edit_handle, WM_SETFONT, (WPARAM)hFont, NULL);
}

void HandleMenuCommand(HWND hWnd, WPARAM wParam, LPARAM lParam)
{
	switch (LOWORD(wParam))
	{
	case IDM_FILE_EXIT:
		PostQuitMessage(EXIT_SUCCESS);
		break;

	case IDM_FORMAT_FONT: 
		SetTextEditFont(hWnd);
		break;
	}
}

static void InitializeFileSubmenu(HMENU hMenu)
{
	HMENU hFileSubmenu = CreatePopupMenu();

	AppendMenu(hFileSubmenu, MF_STRING, IDM_FILE_NEW, L"New\t\tCtrl+N");
	AppendMenu(hFileSubmenu, MF_STRING, IDM_FILE_NEW_WINDOW, L"New Window\t\tCtrl+Shift+N");
	AppendMenu(hFileSubmenu, MF_STRING, IDM_FILE_OPEN, L"Open...\t\tCtrl+O");
	AppendMenu(hFileSubmenu, MF_STRING, IDM_FILE_SAVE, L"Save\t\tCtrl+S");
	AppendMenu(hFileSubmenu, MF_STRING, IDM_FILE_SAVE_AS, L"Save as...\t\tCtrl+Shift+S");
	AppendMenu(hFileSubmenu, MF_SEPARATOR, NULL, NULL);
	AppendMenu(hFileSubmenu, MF_STRING, IDM_FILE_PAGE_SETUP, L"Page Setup");
	AppendMenu(hFileSubmenu, MF_STRING, IDM_FILE_PRINT, L"Print...\t\tCtrl+P");
	AppendMenu(hFileSubmenu, MF_SEPARATOR, NULL, NULL);
	AppendMenu(hFileSubmenu, MF_STRING, IDM_FILE_EXIT, L"Exit");

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
	AppendMenu(hEditSubmenu, MF_STRING, IDM_EDIT_FIND, L"Find\tCtrl+F");
	AppendMenu(hEditSubmenu, MF_STRING, IDM_EDIT_FIND_NEXT, L"Find Next\tF3");
	AppendMenu(hEditSubmenu, MF_STRING, IDM_EDIT_FIND_PREV, L"Find Previous\tShift+F3");
	AppendMenu(hEditSubmenu, MF_STRING, IDM_EDIT_REPLACE, L"Replace...\tCtrl+H");
	AppendMenu(hEditSubmenu, MF_STRING, IDM_EDIT_GOTO, L"Go To...\tCtrl+G");
	AppendMenu(hEditSubmenu, MF_SEPARATOR, NULL, NULL);
	AppendMenu(hEditSubmenu, MF_STRING, IDM_EDIT_SELECTALL, L"Select All\tCtrl+A");
	AppendMenu(hEditSubmenu, MF_STRING, IDM_EDIT_TIME_DATE, L"Time/Date\tF5");

	AppendMenu(hMenu, MF_POPUP | MF_STRING, (UINT)hEditSubmenu, L"&Edit");
}

static void InitializeFormatSubmenu(HMENU hMenu)
{
	HMENU hFormatSubmenu = CreatePopupMenu();

	AppendMenu(hFormatSubmenu, MF_STRING, IDM_FORMAT_WORDWRAP, L"Word Wrap");
	AppendMenu(hFormatSubmenu, MF_STRING, IDM_FORMAT_FONT, L"Font");

	AppendMenu(hMenu, MF_POPUP | MF_STRING, (UINT)hFormatSubmenu, L"F&ormat");
}

static void InitializeViewSubmenu(HMENU hMenu)
{
	HMENU hViewSubmenu = CreatePopupMenu();
	HMENU hViewZoomSubmenu = CreatePopupMenu();

	AppendMenu(hViewZoomSubmenu, MF_STRING, IDM_VIEW_ZOOM_IN, L"Zoom In\tCtrl+Plus");
	AppendMenu(hViewZoomSubmenu, MF_STRING, IDM_VIEW_ZOOM_OUT, L"Zoom Out\tCtrl+Minus");
	AppendMenu(hViewZoomSubmenu, MF_STRING, IDM_VIEW_RESTORE, L"Zoom Out\tCtrl+0");
	AppendMenu(hViewSubmenu, MF_POPUP | MF_STRING, (UINT)hViewZoomSubmenu, L"Zoom");
	AppendMenu(hViewSubmenu, MF_STRING, (UINT)hViewZoomSubmenu, L"Status Bar");

	AppendMenu(hMenu, MF_POPUP | MF_STRING, (UINT)hViewSubmenu, L"&View");
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
}
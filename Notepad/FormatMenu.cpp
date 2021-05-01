#include "FormatMenu.h"
#include "Controls.h"
#include "Identifiers.h"
#include "Controls.h"

CHOOSEFONT GetChooseFontFromDialog(HWND hWnd)
{
	CHOOSEFONT cf;
	LOGFONT lf;

	static LOGFONT oldlf;
	static bool initialized = false;

	ZeroMemory(&cf, sizeof(cf));

	cf.lStructSize = sizeof(CHOOSEFONT);
	cf.hwndOwner = hWnd;
	cf.nFontType = SCREEN_FONTTYPE;
	cf.Flags = CF_INITTOLOGFONTSTRUCT;
	cf.rgbColors = RGB(0, 0, 0);

	/* Do all this dumb stuff to save previous choices */
	if (initialized)
	{
		cf.lpLogFont = &oldlf;
		ChooseFont(&cf);
	}

	else
	{
		initialized = true;

		TEXTMETRIC tm = GetSystemFontMetrics(hWnd);

		cf.lpLogFont = &lf;
		cf.lpLogFont->lfHeight = tm.tmHeight + tm.tmExternalLeading;
		lstrcpyW(cf.lpLogFont->lfFaceName, DEFAULT_FONT_STR);

		ChooseFont(&cf);

		oldlf = *cf.lpLogFont;
	}

	return cf;
}

static void SetTextEditFont(HWND hWnd)
{
	CHOOSEFONT cf = GetChooseFontFromDialog(hWnd);
	HFONT hFont = CreateFontIndirect(cf.lpLogFont);

	SendMessage(GetDlgItem(hWnd, IDC_TEXT_EDIT), WM_SETFONT, (WPARAM)hFont, NULL);
	InvalidateRect(hWnd, NULL, FALSE);
}

static void HandleWordWrap(HWND hWnd)
{
	HMENU hMenu = GetMenu(hWnd);
	DWORD check_state = CheckMenuItem(hMenu, IDM_FORMAT_WORDWRAP, MF_BYCOMMAND);

	CheckMenuItem(hMenu, IDM_FORMAT_WORDWRAP, check_state == MF_CHECKED ? MF_UNCHECKED : MF_CHECKED);
}

void HandleFormatMenu(HWND hWnd, WPARAM wParam, LPARAM lParam)
{
	switch (LOWORD(wParam))
	{
	case IDM_FORMAT_FONT:
		SetTextEditFont(hWnd);
		break;

	case IDM_FORMAT_WORDWRAP:
		HandleWordWrap(hWnd);
		break;
	}
}
#include "FileMenu.h"
#include "EditMenu.h"
#include "Identifiers.h"

static OPENFILENAME CreateOFNStructInstance(HWND hWnd, LPWSTR lpstrFile, DWORD dwMaxLength)
{
	OPENFILENAME ofn;
	ZeroMemory(&ofn, sizeof(OPENFILENAME));
	ofn.lStructSize = sizeof(OPENFILENAME);
	ofn.hwndOwner = hWnd;
	ofn.lpstrFilter = L"Text Documents (*.txt)\0*.txt\0All (*.*)\0*.*\0";
	ofn.nFilterIndex = 1;
	ofn.lpstrFile = lpstrFile;
	ofn.nMaxFile = dwMaxLength;
	ofn.Flags = OFN_ENABLESIZING | OFN_FILEMUSTEXIST | OFN_NONETWORKBUTTON | OFN_PATHMUSTEXIST;
	ofn.lpstrDefExt = L".txt";

	return ofn;
}

static LPWSTR GetFilenameFromDialog(HWND hWnd, BOOL* success)
{
	const DWORD dwMaxLength = 256;
	LPWSTR lpstrFile = (LPWSTR)malloc(dwMaxLength * sizeof(WCHAR));
	lpstrFile[0] = '\0';

	OPENFILENAME ofn = CreateOFNStructInstance(hWnd, lpstrFile, dwMaxLength);

	*success = GetOpenFileName(&ofn);

	return lpstrFile;
}

static void ShowFileOpenError(LPWSTR filename)
{
	WCHAR buf[300];
	wsprintf(buf, L"Unable to open %s for reading", filename);
	MessageBox(NULL, buf, L"Error", MB_OK | MB_ICONERROR);
}

static void SetEditControlTextToFileData(HANDLE hFile, HWND hWnd)
{
	const WORD wAllocSize = 66;
	const WORD cchWide = 64;

	LPSTR buf   = (LPSTR)calloc(wAllocSize * 2, sizeof(CHAR));
	LPWSTR wBuf = (LPWSTR)calloc(wAllocSize, sizeof(WCHAR));

	DWORD bytes_read;
	BOOL read_result;

	SetWindowText(GetDlgItem(hWnd, IDC_TEXT_EDIT), L"");

	ReadFile(hFile, buf, 3, &bytes_read, NULL);

	do {
		read_result = ReadFile(hFile, buf, cchWide * 2, &bytes_read, NULL);

		MultiByteToWideChar(CP_UTF8, 0, buf, bytes_read, wBuf, bytes_read / sizeof(WCHAR));

		AppendText(wBuf, hWnd);

	} while (read_result && bytes_read != 0);

	free(buf);
	free(wBuf);
}

static void HandleOpenFile(HWND hWnd)
{
	BOOL success;
	LPWSTR filename = GetFilenameFromDialog(hWnd, &success);

	if (success)
	{
		HANDLE hFile = CreateFile(
			filename, 
			GENERIC_READ,
			0, 
			NULL, 
			OPEN_EXISTING,
			FILE_ATTRIBUTE_NORMAL,
			NULL
		);

		if (hFile == INVALID_HANDLE_VALUE)
		{
			ShowFileOpenError(filename);
		}

		else
		{
			SetEditControlTextToFileData(hFile, hWnd);
			CloseHandle(hFile);
		}
	}

	free(filename);
}

static void CreateNewNotepadProcess(void)
{
	PROCESS_INFORMATION pi = { NULL };
	STARTUPINFO si = { NULL };

	si.cb = sizeof(si);

	if (!CreateProcess(
		NULL, GetCommandLine(),
		NULL, NULL, FALSE,
		NULL, NULL, NULL,
		&si, &pi))
	{
		MessageBox(NULL, L"Failed to create new notepad process!", L"Error", MB_OK | MB_ICONERROR);
	}

	CloseHandle(pi.hProcess);
	CloseHandle(pi.hThread);
}

static UINT FindLastSlash(LPCWSTR path)
{
	UINT pos = 0;

	for (UINT i = lstrlen(path) - 1; i > 0; --i)
	{
		if (path[i] == L'\\' || path[i] == L'/')
		{
			pos = i;
			break;
		}
	}

	return pos;
}

static void WriteEditControlTextToFile(HANDLE hFile, LPWSTR lpstrFileName, HWND hWnd)
{
	HWND hEditControl = GetDlgItem(hWnd, IDC_TEXT_EDIT);
	INT iTextLength   = GetWindowTextLength(hEditControl) + 1;
	LPWSTR lpstrCopy = (LPWSTR)malloc(iTextLength * sizeof(WCHAR));
	GetWindowText(hEditControl, lpstrCopy, iTextLength);

	DWORD dwBytesWritten;
	WriteFile(hFile, lpstrCopy, iTextLength * sizeof(WCHAR), &dwBytesWritten, NULL);
	free(lpstrCopy);
}

static void SetWindowTitleAccordingToFileName(HWND hWnd, LPCWSTR lpstrTitle)
{
	WCHAR buf[296];
	UINT uiLastSlashIndex = FindLastSlash(lpstrTitle);
	wsprintfW(buf, L"%s - Notepad\0", lpstrTitle + uiLastSlashIndex + 1);
	SetWindowText(hWnd, buf);
}

static void HandleSaveFile(HWND hWnd)
{
	const DWORD dwMaxLength = 256;
	LPWSTR lpstrFile = (LPWSTR)malloc(dwMaxLength * sizeof(WCHAR));
	lpstrFile[0] = '\0';

	OPENFILENAME ofn = CreateOFNStructInstance(hWnd, lpstrFile, dwMaxLength);

	if (GetSaveFileName(&ofn))
	{
		HANDLE hFile = CreateFile(
			lpstrFile,
			GENERIC_WRITE,
			FILE_SHARE_READ,
			NULL,
			CREATE_NEW,
			FILE_ATTRIBUTE_NORMAL,
			NULL
		);

		if (hFile != INVALID_HANDLE_VALUE)
		{
			WriteEditControlTextToFile(hFile, lpstrFile, hWnd);
			SetWindowTitleAccordingToFileName(hWnd, ofn.lpstrFile);
			CloseHandle(hFile);
		}
	}

	free(lpstrFile);
}

void HandleFileMenu(HWND hWnd, WPARAM wParam, LPARAM lParam)
{
	switch (LOWORD(wParam))
	{
	case IDM_FILE_EXIT:
		PostQuitMessage(EXIT_SUCCESS);
		break;

	case IDM_FILE_OPEN:
		HandleOpenFile(hWnd);
		break;

	case IDM_FILE_NEW_WINDOW:
		CreateNewNotepadProcess();
		break;

	case IDM_FILE_SAVE:
		HandleSaveFile(hWnd);
		break;
	}
}
#include "FileMenu.h"
#include "EditMenu.h"
#include "Resource.h"

static LPWSTR GetOpenfilenameStructFromDialog(HWND hWnd, BOOL* success)
{
	OPENFILENAME ofn = { 0 };
	WCHAR* lpstrFile = new WCHAR[256];

	lpstrFile[0] = '\0';

	ofn.lStructSize = sizeof(OPENFILENAME);
	ofn.hwndOwner = hWnd;
	ofn.lpstrFilter = L"Text Documents (*.txt)\0*.txt\0All (*.*)\0*.*\0";
	ofn.nFilterIndex = 1;
	ofn.lpstrFile = lpstrFile;
	ofn.nMaxFile = 256;
	ofn.Flags = OFN_ENABLESIZING | OFN_FILEMUSTEXIST | OFN_NONETWORKBUTTON | OFN_PATHMUSTEXIST;
	ofn.lpstrDefExt = L".txt";

	*success = GetOpenFileName(&ofn);

	return lpstrFile;
}

static void HandleOpenFile(HWND hWnd)
{
	BOOL success;
	LPWSTR filename = GetOpenfilenameStructFromDialog(hWnd, &success);

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
			WCHAR buf[300];
			wsprintf(buf, L"Unable to open %s for reading", filename);
			MessageBox(NULL, buf, L"Error", MB_OK | MB_ICONERROR);
		}

		else
		{
			WCHAR buf[32];
			DWORD bytes_read;
			BOOL read_result;

			do {
				read_result = ReadFile(hFile, buf, sizeof(buf), &bytes_read, NULL);
				AppendText(buf, hWnd);
			} while (!read_result && bytes_read != 0);

			CloseHandle(hFile);
		}

		delete[] filename;
	}
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
	}
}
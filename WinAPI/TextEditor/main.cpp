﻿#include<Windows.h>
#include<iostream>
#include <Richedit.h>
#include"resource.h"
#define tab '\t'

CONST CHAR g_sz_WINDOW_CLASS[] = "TextEDITOR";
INT CALLBACK WndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
BOOL LoadTextFileToEdit(HWND hEdit, LPCSTR lpszFileName);
BOOL SaveTextFileFromEdit(HWND hEdit, LPCSTR lpszFileName);
INT WINAPI WinMain(HINSTANCE hInstance, HINSTANCE gPrevInst, LPSTR lpCmdLine, INT nCmdShow)
{

	//1)регистрация окна 
	WNDCLASSEX wClass;
	ZeroMemory(&wClass, sizeof(WNDCLASSEX));
	wClass.style = 0;
	wClass.cbSize = sizeof(WNDCLASSEX);
	wClass.cbClsExtra = 0;
	wClass.cbWndExtra = 0;
	wClass.hIcon = (HICON)LoadImage(hInstance, "ico\\TextEditor.ico", IMAGE_ICON, LR_DEFAULTSIZE, LR_DEFAULTSIZE, LR_LOADFROMFILE);
	wClass.hIconSm = (HICON)LoadImage(hInstance, "ico\\TextEditor.ico", IMAGE_ICON, LR_DEFAULTSIZE, LR_DEFAULTSIZE, LR_LOADFROMFILE);
	wClass.hCursor = LoadCursor(hInstance, IDC_ARROW);
	HBITMAP bacround = (HBITMAP)LoadImage(hInstance, "ico\\bgr.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
	wClass.hbrBackground = CreatePatternBrush(bacround);

	wClass.hInstance = hInstance;
	wClass.lpfnWndProc = (WNDPROC)WndProc;
	wClass.lpszClassName = g_sz_WINDOW_CLASS;
	wClass.lpszMenuName = MAKEINTRESOURCE(IDR_MAIN_MENU);

	if (!RegisterClassEx(&wClass))
	{
		MessageBox(NULL, "Class refirstor failed", "error", MB_OK | MB_ICONERROR);
		return 0;
	}


	//2)Создание окна 
	HWND hwnd = CreateWindowEx
	(
		NULL,
		g_sz_WINDOW_CLASS,
		g_sz_WINDOW_CLASS,
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT,
		CW_USEDEFAULT, CW_USEDEFAULT,
		NULL,
		NULL,
		hInstance,
		NULL

	);

	if (hwnd == NULL)
	{
		MessageBox(NULL, "Window creation failed", "Error", MB_OK | MB_ICONERROR);
		return 0;
	}
	ShowWindow(hwnd, nCmdShow);
	UpdateWindow(hwnd);



	//3)Запуск цикла сообщений  
	MSG msg;
	while (GetMessage(&msg, hwnd, 0, NULL) > 0)
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	FreeConsole();

	return 0;
}

INT CALLBACK WndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	static HINSTANCE hRichEdit20 = LoadLibrary("riched20.dll");
	static CHAR szFileName[MAX_PATH] = "";
	static BOOL bnChanged = FALSE;
	switch (uMsg)
	{

	case WM_CREATE:
	{
		
		RECT windowRect;
		RECT clientRect;
		GetWindowRect(hwnd, &windowRect);
		GetClientRect(hwnd, &clientRect);
		std::cout << "Window:" << windowRect.left << tab << windowRect.top << tab << windowRect.right << tab << windowRect.bottom << std::endl;
		std::cout << "Client:" << clientRect.left << tab << clientRect.top << tab << clientRect.right << tab << clientRect.bottom << std::endl;
		HWND hEdit = CreateWindowEx
		(
			NULL, RICHEDIT_CLASS, "Workspace",
			WS_CHILD | WS_VISIBLE | ES_MULTILINE | ES_AUTOVSCROLL|WS_VSCROLL,
			0, 0,
			windowRect.right - windowRect.left,
			windowRect.bottom - windowRect.top,
			hwnd,
			(HMENU)IDC_EDIT,
			NULL,
			NULL
		);
		SendMessage(hEdit,EM_SETEVENTMASK,0,ENM_CHANGE);
	}
	break;
	case WM_SIZE:
	{
		
		RECT window;
		GetClientRect(hwnd, &window);
		MoveWindow(GetDlgItem(hwnd, IDC_EDIT), 10, 10, window.right - 20, window.bottom - 20, TRUE);
	}
	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case ID_FILE_OPEN:
		{
			BOOL cancel = FALSE;
			if (bnChanged)
			{
				switch (MessageBox(hwnd, "файл не открыт ", "файл не открыт", MB_YESNOCANCEL | MB_ICONQUESTION))
				{
				case IDYES:		SendMessage(hwnd, WM_COMMAND, ID_FILE_SAVE, 0);
				case IDNO:		break;
				case IDCANCEL:	cancel = TRUE;
				}
			}
			//CHAR szFileName[MAX_PATH]{};
			if (cancel)break;
			OPENFILENAME ofn;
			ZeroMemory(&ofn, sizeof(ofn));

			ofn.lStructSize = sizeof(ofn);
			ofn.hwndOwner = hwnd;
			ofn.lpstrFilter = "Text files: (*.txt)\0*.txt\0C Plus Plus files (*.cpp | *.h)\0*.cpp;*.h\0All files (*.*)\0*.*\0";
			ofn.lpstrDefExt = "txt";
			//std::cout << "Hello" << std::endl;
			//std::cout << sizeof("Hello") << std::endl;
			ofn.lpstrFile = szFileName;
			ofn.nMaxFile = MAX_PATH;
			ofn.Flags = OFN_EXPLORER | OFN_FILEMUSTEXIST | OFN_HIDEREADONLY;

			if (GetOpenFileName(&ofn))
			{
				HWND hEdit = GetDlgItem(hwnd, IDC_EDIT);
				LoadTextFileToEdit(hEdit, szFileName);
				bnChanged = FALSE;
			}
		}
		break;
		case ID_FILE_SAVE:
		{
			if (strlen(szFileName))
				SaveTextFileFromEdit(GetDlgItem(hwnd, IDC_EDIT), szFileName);
			else
				SendMessage(hwnd, WM_COMMAND, LOWORD(ID_FILE_SAVEAS), 0);
		}
		break;
		case ID_FILE_SAVEAS:
		{
			OPENFILENAME ofn;
			ZeroMemory(&ofn, sizeof(ofn));
			ofn.lStructSize = sizeof(ofn);
			ofn.hwndOwner = hwnd;
			ofn.lpstrFilter = "Text files: (*.txt)\0*.txt\0C Plus Plus files (*.cpp | *.h)\0*.cpp;*.h\0All files: (*.*)\0*.*\0";
		
			ofn.lpstrDefExt = "txt";
			ofn.lpstrFile = szFileName;
			ofn.nMaxFile = MAX_PATH;
			ofn.Flags = OFN_EXPLORER | OFN_PATHMUSTEXIST | OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT;
			if (GetSaveFileName(&ofn))SaveTextFileFromEdit(GetDlgItem(hwnd, IDC_EDIT), szFileName);
		}
		break;
		/////////////////////////////////////////////////////////////////////
		case IDC_EDIT:
		{
			if (HIWORD(wParam) == EN_CHANGE)	//Doesn't work with MULTILINE & WM_SETTEXT simultanously.
			{
				bnChanged = TRUE;
				std::cout << "File was changed" << std::endl;
			}
		}
		break;
		}
		break;
	case WM_DESTROY:
		FreeLibrary(hRichEdit20);
		PostQuitMessage(0);
		break;
	case WM_CLOSE:
		DestroyWindow(hwnd);
		break;
	default:	return DefWindowProc(hwnd, uMsg, wParam, lParam);
	}
	return FALSE;
}

BOOL LoadTextFileToEdit(HWND hEdit, LPCSTR lpszFileName)
{
	BOOL bSuccess = FALSE;
	HANDLE hFile = CreateFile
	(lpszFileName,GENERIC_READ,FILE_SHARE_READ,NULL,OPEN_EXISTING,0, 0);

	if (hFile != INVALID_HANDLE_VALUE)
	{
		DWORD dwFileSize = GetFileSize(hFile, NULL);
		if (dwFileSize != UINT_MAX)
		{
			LPSTR lpszFiletext = (LPSTR)GlobalAlloc(GPTR, dwFileSize + 1);
			if (lpszFiletext)
			{
				DWORD dwRead=0;
				if (ReadFile(hFile, lpszFiletext, dwFileSize, &dwRead, NULL))
				{
					if(SendMessage(hEdit,WM_SETTEXT,0,(LPARAM)lpszFiletext))bSuccess=TRUE;
				}
				GlobalFree(lpszFiletext);

			}
			CloseHandle(hFile);
		}
		return bSuccess;
	}
}
BOOL SaveTextFileFromEdit(HWND hEdit, LPCSTR lpszFileName)
{
	BOOL bSuccess = FALSE;
	HANDLE hFile = CreateFile(lpszFileName,GENERIC_WRITE,0,NULL,CREATE_ALWAYS,FILE_ATTRIBUTE_NORMAL,NULL);
	if (hFile != INVALID_HANDLE_VALUE)
	{
		DWORD dwTextLength=SendMessage(hEdit,WM_GETTEXTLENGTH,0,0);
		if (dwTextLength)
		{
			LPSTR lpszText=(LPSTR)GlobalAlloc(GPTR,dwTextLength+1);
			if (lpszText)
			{
				if (SendMessage(hEdit, WM_GETTEXT, dwTextLength + 1, (LPARAM)lpszText))
				{
					DWORD dwWritten;
					if(WriteFile(hEdit,lpszText,dwTextLength,&dwWritten,NULL))bSuccess=TRUE;

				}
				GlobalFree(lpszText);
			}

		}
		CloseHandle(hFile);
	}
	return bSuccess;
}

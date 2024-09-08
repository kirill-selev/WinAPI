#include<Windows.h>
#include<iostream>
#include"resource.h"
#define tab '\t'

CONST CHAR g_sz_WINDOW_CLASS[] = "TextEDITOR";
INT CALLBACK WndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

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
	HBITMAP bacround = (HBITMAP)LoadImage(hInstance, "ico\\bgr.bmp", IMAGE_BITMAP, 0,0, LR_LOADFROMFILE);
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
			NULL, "Edit", "Workspace",
			WS_CHILD | WS_VISIBLE | ES_MULTILINE | ES_AUTOHSCROLL | ES_AUTOVSCROLL,
			0, 0,
			windowRect.right - windowRect.left,
			windowRect.bottom - windowRect.top,
			hwnd,
			(HMENU)IDC_EDIT,
			NULL,
			NULL
		);
	}
	break;
	case WM_SIZE:
	{
		HWND hEdit=GetDlgItem(hwnd,IDC_EDIT);
		RECT window;
		GetWindowRect(hwnd,&window);
		MoveWindow(hEdit,0,0,window.right-window.left,window.bottom-window.top,TRUE);
	}
	case WM_COMMAND:
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	case WM_CLOSE:
		DestroyWindow(hwnd);
		break;
	default:	return DefWindowProc(hwnd, uMsg, wParam, lParam);
	}
	return FALSE;
}
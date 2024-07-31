#include<Windows.h>
#include"resource.h"
BOOL CALLBACK DLgProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lparam);

INT WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPreveInst, LPSTR IpCmdLine, INT nCmdShow)
{
	/*MessageBox(NULL,L"Hello worl!\n это окно сообщение",L"Hello WinAPI",MB_YESNOCANCEL|MB_ICONQUESTION| MB_DEFBUTTON3);*/
	
	DialogBoxParam(hInstance, MAKEINTRESOURCE(IDD_DIALOG1), NULL, (DLGPROC)DLgProc, 0);

	return 0;
}

BOOL CALLBACK DLgProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lparam)
{
	switch (uMsg)
	{

	case WM_INITDIALOG:
	{
		HICON hIcon = LoadIcon(GetModuleHandle(NULL), MAKEINTRESOURCE(IDI_ICON2));
		SendMessage(hwnd, WM_SETICON, 0, (LPARAM)hIcon);

		//HWND hEditlogin=GetDlgItem(hwnd,IDC_EDIT_LOGIN);
		//SetFocus(hEditlogin);
		/*SetFocus(GetDlgItem(hwnd,IDC_EDIT_LOGIN));*/
	}
	break;
	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case IDC_BUTTON_COPY:

		{
			HWND hEdit = GetDlgItem(hwnd, IDC_EDIT_LOGIN);
			HWND hEditPassword = GetDlgItem(hwnd, IDC_EDIT_PASSWORD);

			CONST INT SIZE = 256;
			CHAR sz_buffer[SIZE]{};

			SendMessage(hEdit, WM_GETTEXT, SIZE, (LPARAM)sz_buffer);
			SendMessage(hEditPassword, WM_SETTEXT, 0, (LPARAM)sz_buffer);
		}

		break;



		case IDOK:MessageBox(hwnd, L"Была нажата кнопка ОК", L"Info", MB_OK | MB_ICONINFORMATION);break;
		case IDCANCEL: EndDialog(hwnd, 0);
		}
		break;
	
	
	case WM_CLOSE:
		EndDialog(hwnd, 0);

	}


	return FALSE; 
}

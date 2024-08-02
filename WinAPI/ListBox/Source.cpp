#include<Windows.h>
#include<cstdio>
#include"resource.h"




CONST CHAR* values[] = { "This", "is", "my", "first", "Combo", "Box" };

BOOL CALLBACK DlgProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

INT WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInst, LPSTR lpCmdLine, INT nCmdShow)
{
	DialogBoxParam(hInstance, MAKEINTRESOURCE(IDD_DIALOG1), NULL, (DLGPROC)DlgProc, 0);
	return 0;
}

BOOL CALLBACK DlgProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_INITDIALOG:
	{
		HWND hList = GetDlgItem(hwnd, IDC_LIST1);
		for (int i = 0; i < sizeof(values) / sizeof(values[0]); i++)
		{
			SendMessage(hList, LB_ADDSTRING, 0, (LPARAM)values[i]);
		}



	}
	break;

	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case IDOK:
		{
			HWND hList = GetDlgItem(hwnd, IDC_LIST1);
			INT i = SendMessage(hList, LB_GETCURSEL, 0, 0);
			if (i == LB_ERR)MessageBox(hwnd, "Для начала сделайте ваш выбор", "warning", MB_OK | MB_ICONWARNING);
			{
				CONST INT SIZE = 256;
				CHAR sz_buffer[SIZE]{};
				CHAR sz_message[SIZE]{};

				SendMessage(hList, LB_GETTEXT, i, (LPARAM)sz_buffer);
				sprintf_s(sz_message, sizeof(sz_message), "Вы выбрали пункт №%i, со значением %s", i, sz_buffer);

				MessageBox(hwnd, sz_message, "your choice", MB_OK | MB_ICONINFORMATION);
			}
		}
		break;

		case IDC_BUTTON_ADD:

		 {
			HWND hList = GetDlgItem(hwnd, IDC_LIST1);
			SendMessage(hList, LB_ADDSTRING, 0, (LPARAM)"add item");

		 }
		 break;
		case IDC_BUTTON_DELETE:

		 {
			HWND hList = GetDlgItem(hwnd, IDC_LIST1);
			INT i = SendMessage(hList, LB_GETCURSEL, 0, 0);
			if (i == LB_ERR)MessageBox(hwnd, "Для начала сделайте ваш выбор", "warning", MB_OK | MB_ICONWARNING);
			{
				SendMessage(hList, LB_DELETESTRING, i,0);
			}

			
		 }


		break;


		case IDCANCEL:EndDialog(hwnd, 0);
		}
		break;

	case WM_CLOSE:EndDialog(hwnd, 0);
	}
	return FALSE;
}
#include <windows.h>
#include <CommCtrl.h>
#include <type_traits>

#ifndef UNICODE
#define UNICODE
#endif

#define IDM_FILE 0
#define IDM_FILE_QUIT 1

LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

void addMenu(HWND hWnd);
void addButton(HWND hWnd);
void addControls(HWND hWnd);

int WINAPI wWinMain(_In_ HINSTANCE hInst, _In_opt_ HINSTANCE hInstPrev, _In_ PWSTR cmdline, _In_ int cmdshow) 
{
	//Register window class
	const wchar_t FTP_SERV[] = L"ftp server";

	WNDCLASSW wc = {0};

	//bit confusing.. lp means long pointer, fn function, WndProc Windows Procedure. So.. long pointer to windows procedure function. (can use just WindowProc as well).
	wc.lpfnWndProc = &WindowProc;
	wc.hInstance = hInst;
	//sz zero terminated string
	wc.lpszClassName = FTP_SERV;

	RegisterClass(&wc);

	//create window
	HWND hWnd = CreateWindowEx(
		NULL,					//optional styles
		FTP_SERV,				//window class	
		L"FTP SERVER",			//window text
		WS_OVERLAPPEDWINDOW,	//window style

		//size and position of window
		CW_USEDEFAULT, CW_USEDEFAULT, 800, 500,

		NULL,	//parent window
		NULL,	//menu
		hInst,	//instance handle
		NULL	//additional application data

	);

	if (hWnd == NULL)
	{
		return -1;
	}

	//display window
	ShowWindow(hWnd, 1);

	//initialize message structure
	MSG msg = {0};
	//message loop
	while (GetMessage(&msg, NULL, 0, 0) > 0) 
	{
		//process dialog message (tab, for example)
		if (IsDialogMessage(hWnd, &msg) == FALSE)
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}
	return 0;
}

//process windows messages
LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg) 
	{
		//left mouse button press
		case WM_LBUTTONDOWN: 
		{
			POINT pt;
			pt.x = LOWORD(lParam);
			pt.y = HIWORD(lParam);
			return 0;
		}
		//repaint window
		case WM_PAINT:
		{
			PAINTSTRUCT ps;
			//fill painstruct with paint reqeuest info
			HDC hdc = BeginPaint(hWnd, &ps);
			//do painting
			FillRect(hdc, &ps.rcPaint, (HBRUSH)(COLOR_WINDOW + 1));
			//signal to windows painting complete
			EndPaint(hWnd, &ps);
			return 0;
		}
		//close window
		case WM_CLOSE:
		{
			//display message box on attempt to close

			// part of fun with message boxes
			//int mBoxRet = MessageBox(hWnd, L"Quit?", L"My application", MB_OKCANCEL);
			if (MessageBox(hWnd, L"Quit?", L"FTP_CLIENT", MB_OKCANCEL) == IDOK)
			{
				DestroyWindow(hWnd);
				return 0;
			}
			//fun with message boxes
			/*else if (mBoxRet == IDCANCEL)
			{
				if (MessageBox(hWnd, L"really??", L"My application", MB_OKCANCEL) == IDCANCEL) {
					DestroyWindow(hWnd);
					return 0;
				}
			}*/
		}
		//passed ONCE on parent window creation
		case WM_CREATE:
		{
			addMenu(hWnd);
			addControls(hWnd);
			addButton(hWnd);
			return 0;
		}
		//process DEESTROY message from OS (sent on DestroyWindow call)
		case WM_DESTROY:
		{
			PostQuitMessage(0);
			return 0;
		}
		//process menu selections
		case WM_COMMAND:
		{
			//0 for menu, 1 for accelerator
			switch (HIWORD(wParam)) 
			{
				case 0:
					//if HIWORD wparam is 0, switch menu ID
					switch (LOWORD(wParam))
					{
						case(IDM_FILE_QUIT):
							DestroyWindow(hWnd);
							return 0;
					}
				case 1:
					return 0;
				default:
					break;
			}
				
			return 0;
		}
		default:
		{
			return DefWindowProc(hWnd, uMsg, wParam, lParam);
		}
	}
	
	return 0;
}

//add menu
void addMenu(HWND hWnd)
{
	//create menu bar
	HMENU hMenubar = CreateMenu();
	//create menu item
	HMENU hFile = CreateMenu();

	//set menu item info for FILE menu
	MENUITEMINFO miQuit, miFile = {0};
	miFile.cbSize = sizeof(MENUITEMINFO);
	miFile.fMask = MIIM_TYPE | MIIM_DATA | MIIM_ID | MIIM_SUBMENU;
	miFile.hSubMenu = hFile;
	miFile.fType = MFT_STRING;
	miFile.wID = IDM_FILE;
	miFile.dwTypeData = (wchar_t*)L"File";

	//insert bar with item File
	InsertMenuItem(hMenubar, IDM_FILE, TRUE, &miFile);
	//insert Quit under File
	AppendMenu(hFile, MF_STRING, IDM_FILE_QUIT, L"Exit");

	//display menu
	SetMenu(hWnd, hMenubar);

	return;
}

//add button
void addButton(HWND hWnd)
{
	//ok button under ip
	CreateWindowEx(NULL, L"Button", L"OK", WS_CHILD | WS_VISIBLE, 50, 250, 30, 15, hWnd, NULL, NULL, NULL);
	return;
}

//add static control and IP address control
void addControls(HWND hWnd)
{
	//create static control above Host IP entry
	CreateWindowEx(NULL, WC_STATIC, L"Enter Host IP:", WS_VISIBLE | WS_CHILD, 50, 50, 100, 15, hWnd, NULL, NULL, NULL);
	//create edit control for host IP address
	CreateWindowEx(WS_EX_CLIENTEDGE, WC_IPADDRESS, NULL, WS_VISIBLE | WS_CHILD | WS_TABSTOP, 50, 75, 150, 20, hWnd, NULL, NULL, NULL);
	//create static control above Dest IP entry
	CreateWindowEx(NULL, WC_STATIC, L"Enter Dest IP:", WS_VISIBLE | WS_CHILD, 500, 50, 100, 15, hWnd, NULL, NULL, NULL);
	//create edit control for Dest IP address
	CreateWindowEx(WS_EX_CLIENTEDGE, WC_IPADDRESS, NULL, WS_VISIBLE | WS_CHILD | WS_TABSTOP, 500, 75, 150, 20, hWnd, NULL, NULL, NULL);
	return;
}
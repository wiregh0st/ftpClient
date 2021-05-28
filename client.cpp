#include <windows.h>
#include <CommCtrl.h>
#include <type_traits>
#include <string>
#include <iostream>

#ifndef UNICODE
#define UNICODE
#endif
#ifndef _UNICODE
#define _UNICDOE
#endif

#define IDM_FILE 0xC540
#define IDM_FILE_QUIT 0xC541

#define ID_BUTTON_OK1 0x8810
#define ID_HOST_IP 0x8811
#define ID_DEST_IP 0x8812

LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

void addMenu(HWND hWnd);
void addButton(HWND hWnd);
void addControls(HWND hWnd);
void getIPFields(DWORD dwIPAddress, unsigned int* uIPFields);

int WINAPI wWinMain(_In_ HINSTANCE hInst, _In_opt_ HINSTANCE hInstPrev, _In_ PWSTR cmdline, _In_ int cmdshow) 
{
	//Register window class
	const wchar_t FTP_CLIENT[] = L"FTP CLIENT";

	WNDCLASSW wc = {0};

	//bit confusing.. lp means long pointer, fn function, WndProc Windows Procedure. So.. long pointer to windows procedure function. (can use just WindowProc as well).
	wc.lpfnWndProc = &WindowProc;
	wc.hInstance = hInst;
	//sz zero terminated string
	wc.lpszClassName = FTP_CLIENT;

	RegisterClass(&wc);

	//create window
	HWND hWnd = CreateWindowEx(
		NULL,					//optional styles
		FTP_CLIENT,				//window class	
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
	static HBRUSH hBrushStaticCtlBkgnd = GetSysColorBrush(COLOR_WINDOW);
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
			//fill painstruct with paint request info
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
			if (MessageBox(hWnd, L"Quit?", L"FTP_CLIENT", MB_OKCANCEL | MB_ICONQUESTION) == IDOK)
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
			//delete brush created for static control transparency
			DeleteObject(hBrushStaticCtlBkgnd);
			PostQuitMessage(0);
			return 0;
		}
		////WM_NOTIFY
		//case WM_NOTIFY:
		//	if (!IPM_ISBLANK)
		//	{
		//		MessageBoxW(hWnd, L"nmh.From", L"message", 0);
		//	}
		//process menu selections
		case WM_COMMAND:
		{
			//if source is either menu or accelerator
			if (lParam == 0) {
				//0 for menu, 1 for accelerator
				switch (HIWORD(wParam))
				{
					case 0:
					{
						//if HIWORD wparam is 0, switch menu ID
						switch (LOWORD(wParam))
						{
						case(IDM_FILE_QUIT):
							DestroyWindow(hWnd);
							return 0;
						}
					}
					case 1:
					{
						return 0;
					}
					default:
					{
						break;
					}
				}
				return 0;
			}
			//if source not menu or accelerator
			else {
				//switch HMENU control identifier. defined at window creation in the only HMENU argument of CreateWindowEx
				switch (LOWORD(wParam))
				{
					//bottom left ok button
					case ID_BUTTON_OK1:
					{
						//get handle for host and dest controls
						HWND hHostIP = GetDlgItem(hWnd, ID_HOST_IP);
						HWND hDestIP = GetDlgItem(hWnd, ID_DEST_IP);
						DWORD dwHostIP, dwDestIP;
						//get number of non-blank fields from host and dest controls
						int iHostFieldCount = SendMessage(hHostIP, IPM_GETADDRESS, NULL, (LPARAM)&dwHostIP);
						int iDestFieldCount = SendMessage(hDestIP, IPM_GETADDRESS, NULL, (LPARAM)&dwDestIP);
						//check to make sure all IP host/dest fields are filled
						if (iHostFieldCount < 4 || iDestFieldCount < 4)
						{
							//if fields not filled, display message box prompting user to input more information into IP host/dest fields
							MessageBox(hWnd, L"Please fill all fields for both Host and Destination Addresses", L"Entry Error", MB_OK | MB_ICONWARNING);
						}
						else
						{
							unsigned int uHostIPFields[4];
							unsigned int uDestIPFields[4];
							getIPFields(dwHostIP, uHostIPFields);
							getIPFields(dwDestIP, uDestIPFields);
							/* OPEN CONSOLE FOR TESTING*/
							FILE* fp;
							AllocConsole();
							freopen_s(&fp, "CONOUT$", "w", stdout);
							for (int i = 0; i < 4; i++) {
								std::cout << uHostIPFields[i] << " ";
							}
							std::cout << std::endl;
							for (int i = 0; i < 4; i++) {
								std::cout << uDestIPFields[i] << " ";
							}
							FreeConsole();
						}
						return 0;
					}
					default:
					{
						break;
					}
				}
			}
		}
		//change background color for static controls to be white (255,255,255) or same as parent window
		case WM_CTLCOLORSTATIC:
		{
			HDC hdcStatic = (HDC)wParam;
			SetBkMode(hdcStatic, TRANSPARENT);
			return (INT_PTR)hBrushStaticCtlBkgnd;
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
	MENUITEMINFO miFile = {0};
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

//add ok button
void addButton(HWND hWnd)
{
	//ok button under ip
	CreateWindowEx(NULL, L"Button", L"OK", WS_CHILD | WS_VISIBLE, 50, 250, 30, 15, hWnd, HMENU(ID_BUTTON_OK1), NULL, NULL);
	return;
}

//add static control above IP address controls and IP address edit control
void addControls(HWND hWnd)
{
	//create static control above Host IP entry
	CreateWindow(WC_STATIC, L"Enter Host IP:", WS_VISIBLE | WS_CHILD, 50, 50, 100, 20, hWnd, NULL, NULL, NULL);
	//create edit control for host IP address
	CreateWindowEx(WS_EX_CLIENTEDGE, WC_IPADDRESS, NULL, WS_VISIBLE | WS_CHILD | WS_TABSTOP, 50, 75, 150, 20, hWnd, HMENU(ID_HOST_IP), NULL, NULL);
	//create static control above Dest IP entry
	CreateWindow(WC_STATIC, L"Enter Dest IP:", WS_VISIBLE | WS_CHILD, 500, 50, 100, 15, hWnd, NULL, NULL, NULL);
	//create edit control for Dest IP address
	CreateWindowEx(WS_EX_CLIENTEDGE, WC_IPADDRESS, NULL, WS_VISIBLE | WS_CHILD | WS_TABSTOP, 500, 75, 150, 20, hWnd, HMENU(ID_DEST_IP), NULL, NULL);
	return;
}


//perform bit shifting to grab IP addresses from Host and Dest controls
void getIPFields(DWORD dwIPAddress, unsigned int* uIPFields) {
	unsigned int uFourthIPField = 0xFF;
	uFourthIPField = (dwIPAddress & uFourthIPField);
	unsigned int uThirdIPField = 0xFF;
	uThirdIPField = (dwIPAddress >> 8) & uThirdIPField;
	unsigned int uSecondIPField = 0xFF;
	uSecondIPField = (dwIPAddress >> 16) & uSecondIPField;
	unsigned int uFirstIPField = 0xFF;
	uFirstIPField = (dwIPAddress >> 24) & uFirstIPField;
	uIPFields[0] = uFirstIPField;
	uIPFields[1] = uSecondIPField;
	uIPFields[2] = uThirdIPField;
	uIPFields[3] = uFourthIPField;
	return;
}
#include "FtpApp.h"
#include "controls/IpControl.h"
#include "controls/ButtonControl.h"
#include <windows.h>
#include <string>

FtpApp::FtpApp() : hwnd(NULL), wc({}), ipControlInit( {sizeof(ipControlInit), ICC_INTERNET_CLASSES}) {
  this->APP_X_WIDTH = 800;
  this->APP_Y_HEIGHT = 300;
}

FtpApp::~FtpApp() {}

std::vector<HWND> FtpApp::ipHandles;

void FtpApp::CreateAppWindow(_In_ LPCSTR lpClassName,
  _In_opt_ LPCSTR lpWindowName,
  _In_ DWORD dWindowStyle,
  _In_ int x,
  _In_ int y,
  _In_opt_ HWND hwndParent,
  _In_opt_ HMENU hMenu,
  _In_opt_ HINSTANCE hInstance,
  _In_opt_ LPVOID lpParam)
{
  this->wc.lpfnWndProc = FtpApp::WndProc;
  this->wc.hInstance = hInstance;
  this->wc.lpszClassName = lpClassName;
  RegisterClassA(&wc);
  this->hwnd = CreateWindowA(lpClassName, lpWindowName, dWindowStyle, x, y, this->APP_X_WIDTH, this->APP_Y_HEIGHT, hwndParent, hMenu, hInstance, lpParam);
}

BOOL FtpApp::InitIpControl() const {
  BOOL result = InitCommonControlsEx(&this->ipControlInit);
  if (!result) {
    OutputDebugStringA("Failed to initialize common controls for IP address control.\n");
  }
  return result;
}

ButtonControl* FtpApp::CreateChildButtonControl(_In_ LPCSTR lpWindowName,
  _In_ DWORD dWindowStyle,
  _In_ int x,
  _In_ int y,
  _In_opt_ HINSTANCE hInstance,
  _In_opt_ LPVOID lpParam) const {
  HWND parentHandle = getHwnd();
  int APP_X_WIDTH = getAppXWidth();
  int APP_Y_HEIGHT = getAppYHeight();
  HWND hwndButton = CreateWindowA("BUTTON", lpWindowName, dWindowStyle, APP_X_WIDTH / 3 + x, APP_Y_HEIGHT + y, 150, 40, parentHandle , NULL, hInstance, NULL);
  ButtonControl* pButtonControl = new ButtonControl(hwndButton);
  if (!pButtonControl) {
    this->handleControlInitErr("Could not init ButtonControl");
    return NULL;
  }

  WNDPROC oldWndProc = this->setWndProc(hwndButton, ButtonControl::ButtonControlProc);
  pButtonControl->setOldWndProc(oldWndProc);
  SetWindowLongPtrA(hwndButton, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(pButtonControl));
  return pButtonControl;
}

IpControl* FtpApp::CreateChildIpControl(_In_opt_ LPCSTR lpWindowName,
  _In_ DWORD dWindowStyle,
  _In_ int x,
  _In_ int y,
  _In_ int nWidth,
  _In_ int nHeight,
  _In_opt_ HINSTANCE hInstance,
  _In_opt_ LPVOID lpParam) const {
  HWND parentHandle = getHwnd();
  CreateWindowA("STATIC", lpWindowName, WS_VISIBLE | WS_CHILD, x, y - 30, x + 300, 20, parentHandle, NULL, hInstance, NULL);
  HWND hwndIp = CreateWindowA(WC_IPADDRESSA, "", dWindowStyle, x, y, nWidth, nHeight, parentHandle, NULL, hInstance, NULL);
  IpControl* pIpControl = new IpControl(hwndIp);
  if (!pIpControl) {
    this->handleControlInitErr("Could not init IpControl");
    return NULL;
  }
  FtpApp::setIpHandle(hwndIp);
  SetPropA(hwndIp, "IpControlInstance", reinterpret_cast<HANDLE>(pIpControl));
  WNDPROC oldWndProc = this->setWndProc(hwndIp, IpControl::IpControlProc);
  pIpControl->setOldWndProc(oldWndProc);
  return pIpControl;
}

void FtpApp::handleControlInitErr(LPCSTR error) const {
  MessageBoxA(NULL, error, "Error", MB_ICONERROR);
}

HWND FtpApp::getHwnd() const {
  return this->hwnd;
}

int FtpApp::getAppXWidth() const {
  return this->APP_X_WIDTH;
}

int FtpApp::getAppYHeight() const {
  return this->APP_Y_HEIGHT;
}

std::vector<HWND> FtpApp::getIpHandles() {
  return ipHandles;
}

void FtpApp::setIpHandle(HWND hwnd) {
  ipHandles.push_back(hwnd);
}

WNDPROC FtpApp::setWndProc(HWND hwnd, WNDPROC newWndProc) const {
  return reinterpret_cast<WNDPROC>(SetWindowLongPtrA(hwnd, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(newWndProc)));
}

LRESULT CALLBACK FtpApp::WndProc(
  _In_ HWND   hwnd,
  _In_ UINT   uMsg,
  _In_ WPARAM wParam,
  _In_ LPARAM lParam)
{
  switch (uMsg)
  {
    case WM_CLOSE:
    {
      if (MessageBox(hwnd, "Are you sure you want to exit?", "Exit", MB_OKCANCEL) == IDOK) {
        DestroyWindow(hwnd);
      }
      return 0;
    }
    case WM_COMMAND:
    {
      switch (HIWORD(wParam)) {
        case BN_CLICKED:
        {
          ButtonControl::ButtonControlProc(reinterpret_cast<HWND>(lParam), uMsg, wParam, lParam);
          break;
        }
        default:
        {
          break;
        }
      }
      /*   switch (HIWORD(wParam))
         {
           auto t = HIWORD(wParam);
           auto c = 0;
           if (HIWORD(wParam) == BN_CLICKED) {
             ;
           }
           break;
         }*/
      break;
    }
    case WM_DESTROY:
    {
      PostQuitMessage(0);
      return 0;
    }
    case WM_SIZE:
    {
      int width = LOWORD(lParam);
      int height = HIWORD(lParam);
      //OnSize(hwnd, (UINT)wParam, width, height);
      return 0;
    }
    case WM_PAINT:
    {
      PAINTSTRUCT ps;
      HDC hdc = BeginPaint(hwnd, &ps);
      FillRect(hdc, &ps.rcPaint, (HBRUSH)(COLOR_WINDOW + 1));
      EndPaint(hwnd, &ps);
      return 0;
    }
    case WM_CTLCOLORSTATIC:
    {
      HDC hdcStatic = (HDC)wParam;
      SetBkColor(hdcStatic, RGB(255, 255, 255));
      return (LRESULT)GetSysColorBrush(COLOR_WINDOW);
    }
  }
    return DefWindowProcA(hwnd, uMsg, wParam, lParam);
}

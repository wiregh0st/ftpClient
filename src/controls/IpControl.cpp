#include "IpControl.h"
#include <cstdio>


//TODO: rename all this shit to ipaddress
IpControl::IpControl(HWND hwnd) : hwnd(hwnd), oldWndProc(NULL) {
  char buffer[256];
  sprintf(buffer, "IpControl instance created");
  OutputDebugStringA(buffer);
  this->IP_X_WIDTH = 200;
  this->IP_Y_HEIGHT = 25;
}

IpControl::~IpControl() {}


LRESULT CALLBACK IpControl::IpControlProc(_In_ HWND hwnd,
  _In_ UINT   uMsg,
  _In_ WPARAM wParam,
  _In_ LPARAM lParam) {

    IpControl* pIpControl = reinterpret_cast<IpControl*>(GetProp(hwnd, "IpControlInstance"));
    if (pIpControl) {
      switch (uMsg) 
      {
        case WM_DESTROY: 
        {
          RemoveProp(pIpControl->getHwnd(), "IpControlInstance");
          delete pIpControl;
          PostQuitMessage(0);
          return 0;
        }
        default: 
        {
          return CallWindowProcA(pIpControl->oldWndProc, hwnd, uMsg, wParam, lParam);
        }
      }
    }
  return DefWindowProcA(hwnd, uMsg, wParam, lParam);
}

void IpControl::setOldWndProc(WNDPROC oldWndProc) {
  this->oldWndProc = oldWndProc;
}

HWND IpControl::getHwnd() const {
  return hwnd;
}

#include "ButtonControl.h"

ButtonControl::ButtonControl(HWND hwnd) : hwnd(hwnd), oldWndProc(NULL) {}

ButtonControl::~ButtonControl() {}

std::vector<HWND> ButtonControl::ipHandles;

LRESULT CALLBACK ButtonControl::ButtonControlProc(_In_ HWND hwnd,
  _In_ UINT   uMsg,
  _In_ WPARAM wParam,
  _In_ LPARAM lParam) {
  ButtonControl* pButtonControl = reinterpret_cast<ButtonControl*>(GetWindowLongPtrA(hwnd, GWLP_USERDATA));
  if (pButtonControl) {
    switch (uMsg)
    {
      case WM_COMMAND: {
        int notificationCode = HIWORD(wParam);
        switch (notificationCode) {
          case BN_CLICKED:
          {
            HWND hwndButton = reinterpret_cast<HWND>(lParam);
            char buttonTextBuff[256];
            GetWindowTextA(hwndButton, buttonTextBuff, sizeof(buttonTextBuff));
            //TODO: define the button text as constants
            if (strcmp(buttonTextBuff, "Initiate Transfer") == 0) {
              auto t = 0;
            }
            if (strcmp(buttonTextBuff, "Reset") == 0) {
              for (HWND ipHwnd : ipHandles) {
                SendMessageA(ipHwnd, IPM_CLEARADDRESS, NULL, NULL);
              }
              auto t = 0;
            }
            break;
          }
        }
        break;
      }
      //before child window destroy
      case WM_DESTROY:
      {
        delete pButtonControl;
        PostQuitMessage(0);
        return 0;
      }
      //after all child window destroyed
      case WM_NCDESTROY: {
        return 0;
      }
      default:
      {
        return CallWindowProcA(pButtonControl->oldWndProc, hwnd, uMsg, wParam, lParam);
      }
    }
  }
  return DefWindowProcA(hwnd, uMsg, wParam, lParam);
}

HWND ButtonControl::getHwnd() const {
  return hwnd;
}

void ButtonControl::setIpHandle(HWND hwnd) {
  ipHandles.push_back(hwnd);
}

void ButtonControl::setOldWndProc(WNDPROC wndProc) {
  this->oldWndProc = wndProc;
}
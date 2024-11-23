#include "ButtonControl.h"

ButtonControl::ButtonControl(HWND hwnd) : hwnd(hwnd), oldWndProc(NULL) {}

ButtonControl::~ButtonControl() {}


LRESULT CALLBACK ButtonControl::ButtonControlProc(_In_ HWND hwnd,
  _In_ UINT   uMsg,
  _In_ WPARAM wParam,
  _In_ LPARAM lParam) {
  ButtonControl* pButtonControl = reinterpret_cast<ButtonControl*>(GetWindowLongPtrA(hwnd, GWLP_USERDATA));
  //need button id?
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
            if (strcmp(buttonTextBuff, "Initiate Transfer") == 0) {
              auto t = 0;
            }
            //TODO: get child ip windows here. need to send IPM_CLEARADDRESS message to these windows.
            if (strcmp(buttonTextBuff, "Reset") == 0) {
              SendMessageA();
              auto t = 0;
            }
            break;
          }
        }
        break;
      }
     /* case BN_CLICKED:
      {
        char buttonText[256];
        GetWindowTextA(hwnd, buttonText, sizeof(buttonText));
        if (buttonText == "Initiate Transfer") {
          auto t = 0;
        }
        if (buttonText == "Reset") {
          auto t = 0;
        }
        break;
      }*/
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

void ButtonControl::setOldWndProc(WNDPROC wndProc) {
  this->oldWndProc = wndProc;
}
#ifndef BUTTON_CONTROL_H
#define BUTTON_CONTROL_H
#include <windows.h>
#include "ControlBase.h"

class ButtonControl : public ControlBase {
  public:
    ButtonControl(HWND hwnd);
    ~ButtonControl();

    static LRESULT CALLBACK ButtonControlProc(_In_ HWND hwnd,
      _In_ UINT   uMsg,
      _In_ WPARAM wParam,
      _In_ LPARAM lParam);
    HWND getHwnd() const override;
    void setOldWndProc(WNDPROC wndProc) override;
  private:
    WNDPROC oldWndProc;
    HWND hwnd;
};




#endif
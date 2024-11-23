#ifndef IP_CONTROL_H
#define IP_CONTROL_H
#include <windows.h>

class IpControl {
  public:
    IpControl(HWND hwnd);
    ~IpControl();

    static LRESULT CALLBACK IpControlProc(_In_ HWND hwnd,
      _In_ UINT   uMsg,
      _In_ WPARAM wParam,
      _In_ LPARAM lParam);
    HWND getHwnd() const;
    void setOldWndProc(WNDPROC wndProc);
  private: 
    int IP_X_WIDTH;
    int IP_Y_HEIGHT;
    WNDPROC oldWndProc;
    HWND hwnd;
};




#endif

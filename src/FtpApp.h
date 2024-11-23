#ifndef FTPAPP_H
#define FTPAPP_H
#include <windows.h>
#include <CommCtrl.h>
#include "controls/IpControl.h"
#include "controls/ButtonControl.h"


class FtpApp {
  public:
    FtpApp();
    ~FtpApp();

    HWND getHwnd() const;
    int getAppXWidth() const;
    int getAppYHeight() const;

    void CreateAppWindow(_In_ LPCSTR lpClassName,
      _In_opt_ LPCSTR lpWindowName,
      _In_ DWORD dwStyle,
      _In_ int x,
      _In_ int y,
      _In_opt_ HWND hWndParent,
      _In_opt_ HMENU hMenu,
      _In_opt_ HINSTANCE hInstance,
      _In_opt_ LPVOID lpParam);
    IpControl* CreateChildIpControl(_In_opt_ LPCSTR lpWindowName,
      _In_ DWORD dWindowStyle,
      _In_ int x,
      _In_ int y,
      _In_ int nWidth,
      _In_ int nHeight,
      _In_opt_ HINSTANCE hInstance,
      _In_opt_ LPVOID lpParam) const;
    ButtonControl* CreateChildButtonControl(
      _In_ LPCSTR lpWindowName,
      _In_ DWORD dWindowStyle,
      _In_ int x,
      _In_ int y,
      _In_opt_ HINSTANCE hInstance,
      _In_opt_ LPVOID lpParam
    ) const;
    WNDPROC setWndProc(HWND hwnd, WNDPROC controlProc) const;
    BOOL InitIpControl() const;

    void handleControlInitErr(LPCSTR error) const;
  private:
    WNDCLASSA wc;
    HWND hwnd;
    INITCOMMONCONTROLSEX ipControlInit;
    int APP_X_WIDTH;
    int APP_Y_HEIGHT;
  static LRESULT CALLBACK WndProc(_In_ HWND hwnd,
      _In_ UINT   uMsg,
      _In_ WPARAM wParam,
      _In_ LPARAM lParam);
};


#endif
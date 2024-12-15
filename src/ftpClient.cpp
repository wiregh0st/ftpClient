#ifndef UNICODE
#define UNICODE
#endif
#include <iostream>
#include <sstream>
#include "FtpApp.h"
#include "controls/IpControl.h"
#include "controls/ButtonControl.h"


//TODO: handle errors.. more elegantly

int WINAPI WinMain(
  _In_ HINSTANCE hInstance,
  _In_opt_ HINSTANCE hPrevInstance,
  _In_ LPSTR     lpCmdLine,
  _In_ int       nCmdShow)
{
  const char CLASS_NAME[] = "Ftp Client";

  FtpApp app;
  app.CreateAppWindow(
    CLASS_NAME,
    CLASS_NAME,
    WS_OVERLAPPEDWINDOW,
    CW_USEDEFAULT, CW_USEDEFAULT,
    NULL,
    NULL,
    hInstance,
    NULL);
  if (!app.getHwnd())
  {
    std::ostringstream oss;
    oss << "Error: app.getHwnd() returned " << app.getHwnd() << ".\n";
    OutputDebugStringA(oss.str().c_str());
    return 1;
  }
  ShowWindow(app.getHwnd(), nCmdShow);
  //UpdateWindow(app.getHwnd());  

  if (!app.InitIpControl()) {
    std::ostringstream oss;
    oss << "Error initializing IP control.\n";
    OutputDebugStringA(oss.str().c_str());
    return 1;
  }

  IpControl* pIpTo = app.CreateChildIpControl(
    "Enter IP address to transfer from:",
    WS_CHILD | WS_VISIBLE | WS_BORDER,
    50, 50, 200, 25,
    hInstance,
    NULL
    );

  IpControl* pIpFrom = app.CreateChildIpControl(
    "Enter IP address to transfer to:",
    WS_CHILD | WS_VISIBLE | WS_BORDER,
    400, 50, 200, 25,
    hInstance,
    NULL
  );
  
  //TODO: need initiate transfer button and reset fields button
  ButtonControl* pTransferBtn = app.CreateChildButtonControl(
    "Initiate Transfer", 
    WS_CHILD | WS_VISIBLE, 
    -150, -120,
    hInstance, 
    NULL
  );

  ButtonControl* pResetBtn = app.CreateChildButtonControl(
    "Reset",
    WS_CHILD | WS_VISIBLE,
    150, -120,
    hInstance,
    NULL
  );

  MSG msg;

  while (GetMessageA(&msg, NULL, 0, 0) > 0) 
  {
    TranslateMessage(&msg);
    DispatchMessageA(&msg);
  }

  return 0;
}
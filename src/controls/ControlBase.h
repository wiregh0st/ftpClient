#ifndef CONTROL_BASE_H
#define CONTROL_BASE_H
#include <windows.h>

class ControlBase {
public:
  virtual ~ControlBase(){}
  virtual HWND getHwnd() const = 0;
  virtual void setOldWndProc(WNDPROC wndProc) = 0;
};


#endif
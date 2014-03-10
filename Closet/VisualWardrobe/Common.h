#pragma once
#include <string>

//调用DLL的程序需要包含这个头文件

#include "windowsnominmax.h"
#include <ostream>
typedef int (*PCOMMANDENTRYFUNC)(const std::wstring &cmd, std::wostream &out);
typedef int (*PBINDDCFUNC)(HDC hDC);
typedef void (*PUSECURRENTDCFUNC)();

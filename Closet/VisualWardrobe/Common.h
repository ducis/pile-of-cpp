#pragma once
#include <string>

//����DLL�ĳ�����Ҫ�������ͷ�ļ�

#include "windowsnominmax.h"
#include <ostream>
typedef int (*PCOMMANDENTRYFUNC)(const std::wstring &cmd, std::wostream &out);
typedef int (*PBINDDCFUNC)(HDC hDC);
typedef void (*PUSECURRENTDCFUNC)();

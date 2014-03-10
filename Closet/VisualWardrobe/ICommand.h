#pragma once

#include <string>

#include <istream>
#include <ostream>

class ICommand{//命令,就是DLL输入的字符串命令，的接口
public:
	virtual int Invoke(class CProgram *pProgram, std::wistream &arg, std::wostream &log) const = 0;
	//执行，pProgram是该命令隶属于哪一个CProgram(实际上代码里已经到处都是直接调用的GetSingleton了，所以这个地方确实不好。不过后面都用宏代替了)。
	//arg是参数，log是输出信息，返回值若为0就是成功。
};

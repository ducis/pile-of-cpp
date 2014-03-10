#pragma once
#include <string>

template<typename T>
class CConfigVarMod{
public:
	//用于临时改变配置参数的类,只允许作为局部变量出现.创建时修改配置参数,销毁时恢复配置参数
	//name,val分别是要修改的配置参数的名字和值
	CConfigVarMod(const std::wstring &name, const T &val);
	~CConfigVarMod();
private:
	std::wstring m_name;
	T m_cachedVal;
	void *operator new(size_t);
	void *operator new[](size_t);
};

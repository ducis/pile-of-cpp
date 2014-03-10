#pragma once
#include <string>

template<typename T>
class CConfigVarMod{
public:
	//������ʱ�ı����ò�������,ֻ������Ϊ�ֲ���������.����ʱ�޸����ò���,����ʱ�ָ����ò���
	//name,val�ֱ���Ҫ�޸ĵ����ò��������ֺ�ֵ
	CConfigVarMod(const std::wstring &name, const T &val);
	~CConfigVarMod();
private:
	std::wstring m_name;
	T m_cachedVal;
	void *operator new(size_t);
	void *operator new[](size_t);
};

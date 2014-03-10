#pragma once

#include <map>
#include <stack>
#include <string>

template <typename TYPE,typename KEY = std::wstring>
class CRegistryStack{
	//TYPE���ʵ������KEY����������ջ
public:
	void Push();
	void Pop();
	void Insert(KEY key, TYPE val);
	TYPE Find(KEY key, TYPE notFound = NULL);
	CRegistryStack();
private:
	typedef std::map<KEY,TYPE> MAP;
	typedef std::stack<MAP> STACK;
	STACK m_stack;
};

#pragma once

#include <map>
#include <stack>
#include <string>

template <typename TYPE,typename KEY = std::wstring>
class CRegistryStack{
	//TYPE类的实例的用KEY索引的树的栈
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

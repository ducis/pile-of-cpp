#pragma once

#include "IObjectSource.h"

template<typename T>
class CObjectSourceCached:public IObjectSource<T>{
public:
	//抽象指针,存储并返回m_cached
	virtual const T Get() const { return m_cached; }
	CObjectSourceCached(const T &val):m_cached(val){}
private:
	T m_cached;
};

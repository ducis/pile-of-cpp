#pragma once

#include "IObjectSource.h"

template<typename T>
class CObjectSourceCached:public IObjectSource<T>{
public:
	//����ָ��,�洢������m_cached
	virtual const T Get() const { return m_cached; }
	CObjectSourceCached(const T &val):m_cached(val){}
private:
	T m_cached;
};

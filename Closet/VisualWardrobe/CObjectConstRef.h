#pragma once

template<typename T>
class CObjectConstRef{
public:
	//抽象指针,存储并返回一个物体的const引用(注意在CObjectConstRef(a)销毁之前a不能被销毁,否则会出错)
	operator const T &() const{
		return m_ref;
	}
	explicit CObjectConstRef(const T &obj):m_ref(obj){}
public:
	const T &m_ref;
};

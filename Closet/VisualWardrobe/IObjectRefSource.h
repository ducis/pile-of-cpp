#pragma once

//一个抽象的指针的接口，调用GetRef()返回所指的对象的const引用
template<typename T>
class IObjectRefSource{
public:
	virtual const T &GetRef() const;
};
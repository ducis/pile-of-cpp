#pragma once

//一个抽象的指针的接口，调用Get()返回所指的对象的拷贝
template<typename T>
class IObjectSource{
public:
	virtual const T Get() const=0;
};

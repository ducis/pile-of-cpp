#pragma once

//һ�������ָ��Ľӿڣ�����Get()������ָ�Ķ���Ŀ���
template<typename T>
class IObjectSource{
public:
	virtual const T Get() const=0;
};

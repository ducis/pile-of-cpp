#pragma once

//һ�������ָ��Ľӿڣ�����GetRef()������ָ�Ķ����const����
template<typename T>
class IObjectRefSource{
public:
	virtual const T &GetRef() const;
};
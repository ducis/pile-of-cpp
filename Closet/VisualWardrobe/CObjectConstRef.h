#pragma once

template<typename T>
class CObjectConstRef{
public:
	//����ָ��,�洢������һ�������const����(ע����CObjectConstRef(a)����֮ǰa���ܱ�����,��������)
	operator const T &() const{
		return m_ref;
	}
	explicit CObjectConstRef(const T &obj):m_ref(obj){}
public:
	const T &m_ref;
};

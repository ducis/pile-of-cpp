#pragma once
#include "mtxlib.h"
#include <cassert>


class CCoordSysAxis{//���ڽ���ʾ����ϵ�Ļ�����,ʵ���Ͼ���������0,1,2�ֱ��ʾx,y,z
	friend bool operator==(const CCoordSysAxis &lhs, const CCoordSysAxis &rhs){
		return lhs.m_id == rhs.m_id;
	}
	friend bool operator!=(const CCoordSysAxis &lhs, const CCoordSysAxis &rhs){
		return lhs.m_id != rhs.m_id;
	}
public:
	const vector3 &Vector() const;//���ػ�����(x,y,z)
	const vector4 &Vector4() const;//����(x,y,z,0)
	const int Id() const{
		return m_id;
	}
	explicit CCoordSysAxis(int id):m_id(id){
		assert(m_id>=0);
		assert(m_id<3);
	}
	const CCoordSysAxis Next() const{//����x,y,z,x,y,z,x,y,z...������˳��ѭ��
		return CCoordSysAxis((m_id+1)%3);
	}
	const CCoordSysAxis Prev() const{//����z,y,x,z,y,x,z,y,x...������˳��ѭ��
		return CCoordSysAxis((m_id+2)%3);
	}
	//const CCoordSysAxis &operator=(const CCoordSysAxis &rhs){
	//	m_id = rhs.m_id;
	//}
private:
	int m_id;
};

#pragma once
#include "mtxlib.h"
#include <cassert>


class CCoordSysAxis{//用于将表示坐标系的基的类,实际上就是用数字0,1,2分别表示x,y,z
	friend bool operator==(const CCoordSysAxis &lhs, const CCoordSysAxis &rhs){
		return lhs.m_id == rhs.m_id;
	}
	friend bool operator!=(const CCoordSysAxis &lhs, const CCoordSysAxis &rhs){
		return lhs.m_id != rhs.m_id;
	}
public:
	const vector3 &Vector() const;//返回基向量(x,y,z)
	const vector4 &Vector4() const;//返回(x,y,z,0)
	const int Id() const{
		return m_id;
	}
	explicit CCoordSysAxis(int id):m_id(id){
		assert(m_id>=0);
		assert(m_id<3);
	}
	const CCoordSysAxis Next() const{//按照x,y,z,x,y,z,x,y,z...这样的顺序循环
		return CCoordSysAxis((m_id+1)%3);
	}
	const CCoordSysAxis Prev() const{//按照z,y,x,z,y,x,z,y,x...这样的顺序循环
		return CCoordSysAxis((m_id+2)%3);
	}
	//const CCoordSysAxis &operator=(const CCoordSysAxis &rhs){
	//	m_id = rhs.m_id;
	//}
private:
	int m_id;
};

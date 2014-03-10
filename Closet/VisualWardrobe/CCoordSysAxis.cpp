#include "CCoordSysAxis.h"


const vector3 &CCoordSysAxis::Vector() const{
	static const vector3 vs[3] = {
		vector3(1,0,0),
		vector3(0,1,0),
		vector3(0,0,1)
	};
	return vs[m_id];
}


const vector4 &CCoordSysAxis::Vector4() const{
	static const vector4 vs[4] = {
		vector4(1,0,0,0),
		vector4(0,1,0,0),
		vector4(0,0,1,0)
	};
	return vs[m_id];
}
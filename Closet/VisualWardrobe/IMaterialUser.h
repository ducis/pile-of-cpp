#pragma once

#include "InterfaceBase.h"

class IMaterialUser:private InterfaceBase{//具有材质属性的对象的接口
public:
	virtual void SetMaterial( class IMaterial *pMaterial )=0;
};
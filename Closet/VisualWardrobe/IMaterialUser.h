#pragma once

#include "InterfaceBase.h"

class IMaterialUser:private InterfaceBase{//���в������ԵĶ���Ľӿ�
public:
	virtual void SetMaterial( class IMaterial *pMaterial )=0;
};
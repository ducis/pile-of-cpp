#pragma once
#include "mtxlib.h"

class ITranslate{//具有平移属性的对象的接口
public:
	virtual const vector3 GetTranslation()=0;
	virtual void SetTranslation(const vector3 &translation)=0;
};

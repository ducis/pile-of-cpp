#pragma once
#include "mtxlib.h"

class ITranslate{//����ƽ�����ԵĶ���Ľӿ�
public:
	virtual const vector3 GetTranslation()=0;
	virtual void SetTranslation(const vector3 &translation)=0;
};

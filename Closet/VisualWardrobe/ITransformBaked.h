#pragma once
//≤ª π”√
class ITransformBaked{
public:
	virtual void BakeTransform(int x_mm,int y_mm,int z_mm,int rotZ_90deg) = 0;
};

#pragma once

class IDrillingRule{//从板边沿的长度到应该生成的孔的数量,位置和形状的映射
public:
	virtual void Generate(float dim) = 0;//调用这个函数设定板边沿的长度,然后调用下面几个函数获得孔的信息
	virtual unsigned int GetHoleCount() const = 0;
	virtual class IHoleStyle *GetHoleStyle(unsigned int i) const = 0;
	virtual float GetHolePos(unsigned int i) const = 0;
};

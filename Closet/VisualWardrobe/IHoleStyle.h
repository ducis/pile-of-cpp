#pragma once

#include <utility>

class IHoleStyle{//板边缘上的孔的接口，包括了可能出现的一个与其垂直且相交的孔的信息。
public:
	virtual float GetDiameter() const =0;//主孔的直径
	virtual float GetDepth() const =0;//主孔的深度
	virtual float GetExtrude() const =0;//在主孔相对的那个板面上还要打多深
	virtual const std::pair<float,IHoleStyle*> GetSecondaryHole() const =0;//可能出现的与主孔垂直相交的副孔,first是副孔中心离板边的距离(副孔的这个方法应该总是返回(0,NULL)
protected:
	~IHoleStyle(){}
};



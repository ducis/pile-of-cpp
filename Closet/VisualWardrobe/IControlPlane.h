#pragma once
#include <utility>
class IControlPlane{//这是最开始所用的直接修改面片模型的柜体使用的,现在没有用了
public:
	virtual void Render() const = 0;
	virtual float Pick(const class vector2 &posHS,const class matrix44 &viewProj) const = 0;
	virtual int Move(int delta) = 0;
	virtual int MoveTo(int pos) = 0;
	virtual std::pair<float,float> CastRange( const class vector3 &dir ) const = 0;
};
inline float GetCPCenter(const IControlPlane &cp, const class vector3 &dir ){
	std::pair<float,float> range = cp.CastRange(dir);
	return 0.5f*(range.first+range.second);
}

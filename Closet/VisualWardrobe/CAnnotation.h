#pragma once
#include <utility>
#include "mtxlib.h"

class CAnnotation{//旧的标注,不建议使用
public:
	CAnnotation(const std::pair<class IControlPlane*,class IControlPlane*> &planePair,float u,float v)
		:m_planePair(planePair),m_u(u),m_v(v),m_length(0.0f),m_dRange(0.0f,0.0f),m_uDelta(0.0f){}
	void Render(const vector3 &mainVec,const vector3 &uBase,const vector3 &vBase,const float &uOffset);
	//void DrawText();
	//bool HasPlane(class IControlPlane *p) { return (m_planePair.first == p) || (m_planePair.second == p); }
	float GetLength() const{ return m_length; }
	const std::pair<float,float> &GetDRange() const{return m_dRange;}
	void Update(const vector3 &mainVec);
	float GetUPos() const;
	void SetUPos(float u);
	void ResetUPos();
private:
	const std::pair<class IControlPlane*,class IControlPlane*> &m_planePair;
	float m_u,m_v;
	float m_length;
	std::pair<float,float> m_dRange;
	float m_uDelta;
	//vector3 m_mid;
	//int m_length;
};

//namespace NAnnotation{
//	inline bool Shorter(const CAnnotation *lhs,const CAnnotation *rhs){
//		return lhs->GetLength()<rhs->GetLength();
//	}
//	inline bool SeparateAndLess(const CAnnotation *lhs,const CAnnotation *rhs){
//		return lhs->GetDRange().second<rhs->GetDRange().first;
//	}
//}

#pragma once

#include "mtxlib.h"
#include "DragHandle.h"
#include <memory>

class CTargetCamera{
public:
	//场景中的可控摄像机类,由位置(焦点的位置,不是眼的位置)和两个极角定义,只管视距阵不管投影矩阵
	//具有两个DragHandle分别用于拖动位置和角度
	void Apply() const;//在OpenGL中设定相应的视距阵
	void Activate();//启用
	void Deactivate();//仅用
	CTargetCamera();
	~CTargetCamera();
	void MakeOrthogonal(int iView);//设为正交视图,0<=iView<=4,前后左右俯
	void SetTarget(const vector3 &);//设定焦点的位置
private:
	float Pick(const vector2 & posHS, const matrix44 &viewProj) const{ return 0.0f; }
	void VisualizePick() const{}
	const vector2 Pan(const vector2 &displacementHS, const matrix44 &viewProj);
	const vector2 Orbit(const vector2 &displacementHS, const matrix44 &viewProj);
	typedef CTargetCamera self_type;
	typedef DragHandle<self_type,&Pick,&VisualizePick,&Pan> panner_type;
	typedef DragHandle<self_type,&Pick,&VisualizePick,&Orbit> orbiter_type;
	std::auto_ptr<panner_type> m_pPanner;
	std::auto_ptr<orbiter_type> m_pOrbiter;
	float m_azimuth;
	float m_elevation;
	float m_distance;
	vector3 m_target;
};

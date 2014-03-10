#pragma once

#include "mtxlib.h"
#include "DragHandle.h"
#include <memory>

class CTargetCamera{
public:
	//�����еĿɿ��������,��λ��(�����λ��,�����۵�λ��)���������Ƕ���,ֻ���Ӿ��󲻹�ͶӰ����
	//��������DragHandle�ֱ������϶�λ�úͽǶ�
	void Apply() const;//��OpenGL���趨��Ӧ���Ӿ���
	void Activate();//����
	void Deactivate();//����
	CTargetCamera();
	~CTargetCamera();
	void MakeOrthogonal(int iView);//��Ϊ������ͼ,0<=iView<=4,ǰ�����Ҹ�
	void SetTarget(const vector3 &);//�趨�����λ��
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

#pragma once
#include "IControlPlane.h"
#include "mtxlib.h"
#include <set>
#include <map>
class CControlPlaneSystem{//控制平面,用于旧的直接改面片模型的方式,不建议使用
public:
	void Insert(IControlPlane *);
	void Erase(IControlPlane *);
	void AddAnnotation(IControlPlane *,IControlPlane *);
	void RemoveAnnotation(IControlPlane *,IControlPlane *);
	void RenderAnnotations();
	bool Exists(IControlPlane *);
	template<class OutputIterator> void Pick(vector2 hpos,const matrix44 &viewProj,OutputIterator outItr){
		//std::replace_copy_if(m_controlPlanes.begin(),m_controlPlanes.end(),outItr,mem_fun(&IControlPlane::Pick(hpos,viewProj)));//std::bind1st(std::greater_equal<float>(),0.0f));
		for(CONTAINER::iterator iCP = m_controlPlanes.begin(); iCP != m_controlPlanes.end(); ++iCP){
			if((*iCP)->Pick(hpos,viewProj)>=0){
				*(outItr++) = *iCP;
			}
		}
	}
	CControlPlaneSystem(const vector3 &dir,const vector3 &orthoBase0,const vector3 &orthoBase1):m_dir(dir){
		m_orthoBase[0] = orthoBase0;
		m_orthoBase[1] = orthoBase1;
	}
	const vector3 &GetDir(){ return m_dir; }
private:
	const vector3 m_dir;
	vector3 m_orthoBase[2];
	typedef std::set<IControlPlane*> CONTAINER;
	typedef std::pair<IControlPlane*,IControlPlane*> PAIR;
	typedef std::set<PAIR> PAIR_CONTAINER;
	typedef std::map<PAIR *,class CAnnotation *> ANNOTATION_PTR_CONTAINER;
	CONTAINER m_controlPlanes;
	PAIR_CONTAINER m_annotatedPairs;
	ANNOTATION_PTR_CONTAINER m_annotations[2];
	PAIR MakePair(IControlPlane *,IControlPlane *);
};

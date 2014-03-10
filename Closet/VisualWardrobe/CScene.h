#pragma once 

#include <list>
#include <set>
#include "CConstraintComposite.h"
#include "CCutter.h"
#include "3d.h"

class CScene{
public:
	//����
	void Render();//��Ⱦ����
	CScene();
	void Insert(class ISceneNode *pSceneNode);//���볡�����ڵ���Ϊ���ڵ�
	void Remove(class ISceneNode *pSceneNode);//ȥ��������.
	CConstraintComposite *GetConstraintComposite() { return &m_constraintComposite; }//����Լ�����ĸ��ڵ�
	const std::set<CCutter *> &GetCutterSet(){ return m_pCutters; }//����Cutter����
	void RegisterCutter(CCutter *p){ if(p) m_pCutters.insert(p); }//���Cutter
	void UnregisterCutter(CCutter *p){ if(p) m_pCutters.erase(p); }//ȥ��Cutter
	const AABB GetAABB() const;
private:
	std::set<CCutter *> m_pCutters;//Cutter(���ڱ�ʾǽ�����ֻ�ı������״�Ķ���)�ļ���
	std::list<class ISceneNode *> m_listSceneNodes;//�����ڵ����ĸ��ڵ�
	CConstraintComposite m_constraintComposite;//Լ�����ĸ��ڵ�
};

#pragma once 

#include <list>
#include <set>
#include "CConstraintComposite.h"
#include "CCutter.h"
#include "3d.h"

class CScene{
public:
	//场景
	void Render();//渲染场景
	CScene();
	void Insert(class ISceneNode *pSceneNode);//插入场景树节点作为根节点
	void Remove(class ISceneNode *pSceneNode);//去除场景树.
	CConstraintComposite *GetConstraintComposite() { return &m_constraintComposite; }//返回约束树的根节点
	const std::set<CCutter *> &GetCutterSet(){ return m_pCutters; }//返回Cutter集合
	void RegisterCutter(CCutter *p){ if(p) m_pCutters.insert(p); }//添加Cutter
	void UnregisterCutter(CCutter *p){ if(p) m_pCutters.erase(p); }//去除Cutter
	const AABB GetAABB() const;
private:
	std::set<CCutter *> m_pCutters;//Cutter(用于表示墙角这种会改变柜体形状的东西)的集合
	std::list<class ISceneNode *> m_listSceneNodes;//场景节点树的根节点
	CConstraintComposite m_constraintComposite;//约束树的根节点
};

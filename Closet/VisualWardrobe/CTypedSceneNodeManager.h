#pragma once


#include <set>

template <class SCENENODE>
class CTypedSceneNodeManager{
	//管理场景中某个类的所有实例的类,不常用
public:
	CTypedSceneNodeManager();
	SCENENODE *GetCurrent();
	void Register(SCENENODE *p);
	void Next();
	void Prev();
	typedef std::set<SCENENODE *> CONTAINER;
	typename CONTAINER::const_iterator Begin(){return m_pNodes.begin();}
	typename CONTAINER::const_iterator End(){return m_pNodes.end();}
private:
	CONTAINER m_pNodes;
	typename CONTAINER::iterator m_itrCurrent;
};

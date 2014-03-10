#pragma once

#include "ISceneNode.h"
#include <vector>
#include <string>
#include "ptree.h"

class CSceneNodeInterfaceDef:public ISceneNode{//≤ª π”√
public:
	void Render() const{}
	void CatchControlPlanes();
	CSceneNodeInterfaceDef(const boost::property_tree::wptree &tr);

	typedef std::vector<class IControlPlane *> CP_CONTAINER;
	typedef std::vector<class IControlPlane *>::const_iterator CP_OUT_ITERATOR;
	CP_OUT_ITERATOR Begin();
	CP_OUT_ITERATOR End();
	const int GetDirection(){ return m_iDirection; }
	const int GetAxis(){ return m_iDirection%3; }
private:
	std::wstring m_polyName;
	std::vector<int> m_faces;
	std::vector<class IControlPlane *> m_pControlPlanes;
	int m_iDirection;
};

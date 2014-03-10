
#include "CSceneNodePolygonModel.h"
#include "CSceneNodeInterfaceDef.h"
#include "singleton.h"
#include "CRegistryStack.h"
#include <stack>

using namespace std;

CSceneNodeInterfaceDef::CSceneNodeInterfaceDef(const boost::property_tree::wptree &tr):
m_polyName(tr.get<wstring>(L"PolyName")),
m_iDirection(tr.get<int>(L"Direction"))
{
	wistringstream(tr.get_child(L"Faces").data())>>m_faces;
	sort(m_faces.begin(),m_faces.end());
	GetSingleton<std::stack<std::vector<CSceneNodeInterfaceDef *>>>()->top().push_back(this);
}

void CSceneNodeInterfaceDef::CatchControlPlanes(){
	//GetSingleton<CRegistryStack<CSceneNodePolygonModel *>>()
	//	->Find(m_polyName)
	//	->GetControlPlanesByFaces(m_iDirection%3,m_faces.begin(),m_faces.end(),std::back_inserter(m_pControlPlanes));
}


CSceneNodeInterfaceDef::CP_OUT_ITERATOR CSceneNodeInterfaceDef::Begin(){
	return m_pControlPlanes.begin();
}


CSceneNodeInterfaceDef::CP_OUT_ITERATOR CSceneNodeInterfaceDef::End(){
	return m_pControlPlanes.end();
}

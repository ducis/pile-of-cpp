
#include "srl.h"
#include "CSceneNodeWallDef.h"
#include "CRegistryStack.h"
#include "CSceneNodePolygonModel.h"
#include "singleton.h"

using namespace std;
CSceneNodeWallDef::CSceneNodeWallDef(const boost::property_tree::wptree &tr):
m_pPoly(NULL),
m_polyName(tr.get<wstring>(L"PolyName")),
m_iAxis(tr.get<int>(L"Axis"))
{
	wistringstream(tr.get_child(L"Verts").data())>>m_verts;
	sort(m_verts.begin(),m_verts.end());
	GetSingleton<std::stack<std::vector<CSceneNodeWallDef *>>>()->top().push_back(this);
}

void CSceneNodeWallDef::CatchVerts()
{
	//m_pPoly = GetSingleton<CRegistryStack<CSceneNodePolygonModel *>>()->Find(m_polyName);
	//assert(m_pPoly);
	//m_pPoly->RegisterWallDef(*this);
}

#include "CSceneNodeFactoryRegistry.h"

#include "CSceneNodeFactorySimple.h"
#include "CSceneNodeFactoryFunctionCall.h"

#include "CSceneNodeObjectSet.h"
#include "CSceneNodePolygonModel.h"
#include "CSceneNodeRoom.h"
#include "CSceneNodeAutoLabel.h"
#include "CSceneNodeWallDef.h"
#include "CSceneNodeInteriorDef.h"
#include "CSceneNodeInterfaceDef.h"
#include "CSceneNodeBinaryWardrobeTreeNode.h"
#include "CSceneNodeDoor.h"

#include "singleton.impl.h"
template CSceneNodeFactoryRegistry *GetSingleton();

CSceneNodeFactoryRegistry::CSceneNodeFactoryRegistry(){
#define INS( cppName,xmlName )	Insert(new CSceneNodeFactorySimple< CSceneNode##cppName >(), xmlName);
	INS(ObjectSet,L"ObjectSet");
	INS(PolygonModel,L"Poly");
	INS(Room,L"Room");
	INS(AutoLabel,L"AutoLabel");
	INS(WallDef,L"WallDef");
	INS(InteriorDef,L"InteriorDef");
	INS(InterfaceDef,L"InterfaceDef");
	INS(Door,L"Door");
#undef INS
	Insert(new CSceneNodeFactoryFunctionCall< CSceneNodeBinaryWardrobeTreeNode*, CSceneNodeBinaryWardrobeTreeNode::BuildBWTFromBoxes >(), L"BWTFromBoxes");
}

void CSceneNodeFactoryRegistry::Insert(ISceneNodeFactory *pFac, const std::wstring &name ){
	m_facs.insert(std::pair<std::wstring,ISceneNodeFactory *>(name,pFac));
}


ISceneNodeFactory *CSceneNodeFactoryRegistry::GetFactory( const std::wstring &name ){
	CONTAINER::iterator itr = m_facs.find(name);
	if(itr == m_facs.end())
		return NULL;
	return itr->second;
}


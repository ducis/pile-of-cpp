
#include "CTypedSceneNodeManager.impl.h"
#include "singleton.impl.h"

#include "CSceneNodeInteriorDef.decl.h"
template class CTypedSceneNodeManager<CSceneNodeInteriorDef>;
template CTypedSceneNodeManager<CSceneNodeInteriorDef> *GetSingleton();

#include "singleton.impl.h"
#include "CSceneNodeWallDef.decl.h"
#include "CSceneNodeInteriorDef.decl.h"
#include "CSceneNodeInterfaceDef.decl.h"
#include <stack>
#include <vector>
//���п���Singleton�Ľ���
template std::stack<std::vector<CSceneNodeWallDef *>> *GetSingleton();
template std::stack<std::vector<CSceneNodeInteriorDef *>> *GetSingleton();
template std::stack<std::vector<CSceneNodeInterfaceDef *>> *GetSingleton();

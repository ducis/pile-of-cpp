#pragma once
#include "ISceneNode.h"
#include "IBoxy.h"

class ISceneNodeBoxy:public ISceneNode, public IBoxy{
public:
	virtual ~ISceneNodeBoxy(){}
};


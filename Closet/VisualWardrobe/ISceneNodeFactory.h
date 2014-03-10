#pragma once;

#include "ptree.h"

class ISceneNodeFactory{//SceneNode工厂接口
public:
	virtual class ISceneNode *Create( const boost::property_tree::wptree &tr ) = 0;
	//virtual ISceneNode* Create() = 0;
};

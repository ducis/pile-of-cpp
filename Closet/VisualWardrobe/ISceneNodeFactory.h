#pragma once;

#include "ptree.h"

class ISceneNodeFactory{//SceneNode�����ӿ�
public:
	virtual class ISceneNode *Create( const boost::property_tree::wptree &tr ) = 0;
	//virtual ISceneNode* Create() = 0;
};

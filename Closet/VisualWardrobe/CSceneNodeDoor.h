#pragma once

#include "ISceneNodeBoxy.h"
#include "3d.h"
#include "boost/property_tree/ptree_fwd.hpp"

class CSceneNodeDoor: public ISceneNodeBoxy{
public://显示柜门的类
	virtual void SetAABB(const AABB &aabb);
	virtual const AABB GetAABB() const;
	virtual void Render() const ;
	CSceneNodeDoor( const boost::property_tree::wptree &);
private:
	AABB m_aabb;
	float m_maxSingleDoorWidth;
};
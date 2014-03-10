#pragma once

#include "CSceneNodeBoxyDecorator.h"
#include "3d.h"
#include "UnifiedCoord.h"
#include "VectorTraits.h"
#include <utility>
#include <bitset>

class CSceneNodeBoxyRepositioner: public CSceneNodeBoxyDecorator
{//在一个SceneNodeBoxy里面放一个SceneNodeBoxy的类.
public:
	virtual void SetAABB(const AABB &aabb);
	virtual const AABB GetAABB() const{ return m_aabb; }
	CSceneNodeBoxyRepositioner(ISceneNodeBoxy *pDecorated, const UnifiedCoord<vector3> &remapMin, const UnifiedCoord<vector3> &remapMax)
		:CSceneNodeBoxyDecorator(pDecorated),m_remapping(remapMin,remapMax),m_aabb(MinimumAABB()){}
	//remapMin,remapMax都是UnifiedCoord,UnifiedCoord 和CEGUI里面的Unified coordinates差不多,只不过是扩展到了三维

	enum Flags{
		Enclosing,//是否会在里面的SceneNodeBoxy移出外面的SceneNodeBoxy的时候自动弹回
		NumFlags
	};

	void Set(Flags flag);
	void Clear(Flags flag);
	void SetEnabled(Flags flag,bool enabled);
	bool Get(Flags flag);
private:
	std::pair< UnifiedCoord<vector3>,UnifiedCoord<vector3> > m_remapping;
	AABB m_aabb;
	std::bitset<NumFlags> m_flags;
};

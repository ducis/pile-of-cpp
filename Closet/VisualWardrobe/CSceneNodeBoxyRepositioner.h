#pragma once

#include "CSceneNodeBoxyDecorator.h"
#include "3d.h"
#include "UnifiedCoord.h"
#include "VectorTraits.h"
#include <utility>
#include <bitset>

class CSceneNodeBoxyRepositioner: public CSceneNodeBoxyDecorator
{//��һ��SceneNodeBoxy�����һ��SceneNodeBoxy����.
public:
	virtual void SetAABB(const AABB &aabb);
	virtual const AABB GetAABB() const{ return m_aabb; }
	CSceneNodeBoxyRepositioner(ISceneNodeBoxy *pDecorated, const UnifiedCoord<vector3> &remapMin, const UnifiedCoord<vector3> &remapMax)
		:CSceneNodeBoxyDecorator(pDecorated),m_remapping(remapMin,remapMax),m_aabb(MinimumAABB()){}
	//remapMin,remapMax����UnifiedCoord,UnifiedCoord ��CEGUI�����Unified coordinates���,ֻ��������չ������ά

	enum Flags{
		Enclosing,//�Ƿ���������SceneNodeBoxy�Ƴ������SceneNodeBoxy��ʱ���Զ�����
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

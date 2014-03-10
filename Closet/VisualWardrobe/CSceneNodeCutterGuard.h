#pragma once

#include "3d.h"
#include "ISceneNodeBoxy.h"
#include <memory>
#include "CSceneNodeBoard.h"
class CSceneNodeCutterGuard: public ISceneNodeBoxy{//用于显示柜被切角后新增的板材的类
	typedef CSceneNodeBoard board_type;
	typedef CSceneNodeCutterGuard self_type;
public:
	virtual void Render() const;
	virtual void SetAABB(const AABB &aabb);
	virtual const AABB GetAABB() const;
	virtual void UpdateTransform(const matrix44 &parentTransform);
	//CSceneNodeCutterGuard(const AABB &aabb, board_type *pBoardMinX, board_type *pBoardMaxX, board_type *pBoardMinY, board_type *pBoardMaxY);
	CSceneNodeCutterGuard(const AABB &aabb, board_type *pBoardX, board_type *pBoardY);//, board_type *pBoardMinY, board_type *pBoardMaxY);
	void SetCutter(class CCutter *pCutter);
private:
	//std::auto_ptr<board_type> m_pBoards[2][2];//[x,y][min,max]
	//bool m_bEnabled[2][2];
	//std::auto_ptr<board_type> m_pBoards[2];//[x,y]
	std::auto_ptr<board_type> m_pBoardX,m_pBoardY;
	AABB m_aabb;
	matrix44 m_worldTransform;
	matrix44 m_worldInvTransform;
	class CCutter *m_pCutter;

	void OnSpatialChange();
};

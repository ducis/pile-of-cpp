#pragma once

#include "ISceneNode.h"
#include <vector>
#include <string>
#include "3d.h"
#include "ptree.h"


class CSceneNodeInteriorDef:public ISceneNode{//旧的柜内空间的定义,不使用
public:
	void Render() const;
	void CatchVerts();
	void SetAxis(int iAxis);
	int GetAxis() const;
	int GetBoardNum() const { return m_nBoards; }
	void SetBoardNum(int d) { m_nBoards = d; }
	int GetBoardThickness() const { return m_boardThickness; }
	void SetBoardThickness(int d) { m_boardThickness = d; }
	void AddSubNode(ISceneNode *pNode);
	int GetSubNodeNum() { return m_pSubNodes.size(); }
	void RepositionSubNodes(unsigned int directionMask = 0xFFFFFFFF);
	void ClearSubNodes();
	void RemoveNthSubNode(unsigned int n);
	CSceneNodeInteriorDef(const boost::property_tree::wptree &tr);
	void SetOffsetBegin(int offset){ m_offsetBegin = offset; }
	void SetOffsetEnd(int offset){ m_offsetEnd = offset; }
private:
	std::wstring m_polyName;
	std::vector<int> m_verts;
	std::vector<const vector3 *> m_pPositions;
	int m_iAxis;
	int m_nBoards;
	int m_boardThickness;
	int m_offsetBegin,m_offsetEnd;
	class CSceneNodePolygonModel *m_pPoly;
	std::vector<ISceneNode *> m_pSubNodes;

	const std::pair<vector3,vector3> GetAABB() const;
};

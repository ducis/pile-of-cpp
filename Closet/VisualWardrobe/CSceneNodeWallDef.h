#pragma once

#include "ISceneNode.h"
#include <vector>
#include <string>
#include "3d.h"
#include "ptree.h"
#include <iterator>

class CSceneNodeWallDef:public ISceneNode{//旧的可选取墙壁，不建议使用
public:
	void Render() const{}
	void CatchVerts();
	int GetAxis() const{ return m_iAxis; }
	CSceneNodeWallDef(const boost::property_tree::wptree &tr);
	typedef std::vector<int>::const_iterator SORTED_RANGE_CONST_ITERATOR;
	SORTED_RANGE_CONST_ITERATOR GetSortedRangeBegin() const { return m_verts.begin(); }
	SORTED_RANGE_CONST_ITERATOR GetSortedRangeEnd() const { return m_verts.end(); }
private:
	std::vector<vector3*> m_pPositions;
	std::wstring m_polyName;
	std::vector<int> m_verts;
	int m_iAxis;
	class CSceneNodePolygonModel *m_pPoly;
};

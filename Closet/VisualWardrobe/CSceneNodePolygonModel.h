#pragma once 
#include "3d.h"
#include "ISceneNode.h"
#include "IControlPlane.h"
#include <string>
#include <vector>
#include "ptree.h"
#include "ITransformBaked.h"
#include "CSceneNodeWallDef.decl.h"
#include "stdext.h"
class CSceneNodePolygonModel:public ISceneNode{//,public ITransformBaked{
	//用于显示普通的面片模型(抽屉,领带盒等)的类,从xml中生成,移动面片位置的功能不再使用,被注释掉了
public:
	void Render() const;
	void OnInsertion();
	//void BakeTransform(int x_mm,int y_mm,int z_mm,int rotZ_90deg);
	CSceneNodePolygonModel( const boost::property_tree::wptree &tr);
	~CSceneNodePolygonModel();
	//void RegisterWallDef(CSceneNodeWallDef &wallDef);
	const vector3 *GetVertPositionConstPtr(int id);

	//template<class OutputIterator,class InputIterator> 
	//void GetControlPlanesByFaces(int axis,InputIterator _FirstSortedFace, InputIterator _LastSortedFace, OutputIterator _DestCPs){
	//	std::remove_copy_if(
	//		m_pControlPlanesAxisAligned[axis].begin(),
	//		m_pControlPlanesAxisAligned[axis].end(),
	//		_DestCPs,
	//		std::not1(std::bind2nd(std::mem_fun(&CControlPlane::HasSortedFaces<InputIterator>),std::make_pair(_FirstSortedFace,_LastSortedFace)))
	//		);
	//}

	void EnlargeAABB(AABB &aabb)const;
private:
	//std::wstring m_name;
	class CTexture *m_pDiffuseTex;
	struct FACE{
	public:
		int degree;
		std::vector<int> verts;
		std::vector<int> edges;
		//std::vector<int> mapVerts;
		vector3 normal;
	};
	struct EDGE{
	public:
		int verts[2];
		int faces[2];
	};
	struct VERTEX{
	public:
		vector3 pos;
	};
	struct MAPVERT{
	public:
		vector2 uv;
	};
	std::vector<FACE> m_faces;
	std::vector<EDGE> m_edges;
	std::vector<VERTEX> m_verts;
	//std::vector<MAPVERT> m_mapVerts;
	//vector3 m_minAABB;
	//vector3 m_maxAABB;
	//class CControlPlane: public IControlPlane{
	//public:
	//	CControlPlane(CSceneNodePolygonModel &poly,const boost::property_tree::wptree &tr);
	//	virtual void Render() const;
	//	virtual float Pick(const class vector2 &posHS,const class matrix44 &viewProj) const;
	//	virtual void RegisterToSystem(class CControlPlaneSystem *p);
	//	virtual int Move(int delta);
	//	virtual int MoveTo(int p);
	//	virtual std::pair<float,float> CastRange( const class vector3 &dir ) const;
	//	void RegisterWallDef(CSceneNodeWallDef *pWallDef);
	//	template<class InputIterator>
	//	bool HasSortedFaces(std::pair<InputIterator,InputIterator> sortedRange){
	//		return stdext::set_intersect(m_faces.begin(),m_faces.end(),sortedRange.first,sortedRange.second);
	//	}
	//private:
	//	int GetAxis() const;
	//	CSceneNodePolygonModel &m_polyModel;
	//	std::vector<int> m_faces;
	//	std::vector<int> m_verts;
	//	std::vector<CSceneNodeWallDef *> m_pWallDefs;
	//};
	//std::vector<CControlPlane*> m_pControlPlanesAxisAligned[3];
	class DrawFace: public std::unary_function<const FACE &,void>{
	protected:
		const std::vector<VERTEX> &m_verts;
		const std::vector<MAPVERT> *m_pMapVerts;
	public:
		DrawFace(const std::vector<VERTEX> &verts,const std::vector<MAPVERT> *pMapVerts = NULL)
			:m_verts(verts),m_pMapVerts(pMapVerts){}
		void operator()(const FACE &) const;
	};
	//class PickFace: public std::unary_function<const FACE &,float>{
	//protected:
	//	const std::vector<VERTEX> &m_verts;
	//	const matrix44 &m_viewProj;
	//	const vector2 &m_hpos;
	//public:
	//	PickFace(const std::vector<VERTEX> &verts, const matrix44 &viewProj, const vector2 &hpos):m_verts(verts),m_viewProj(viewProj),m_hpos(hpos){}
	//	float operator()(const FACE &) const;
	//};
	//class PickFaceEdges: private PickFace{
	//protected:
	//	const std::vector<EDGE> &m_edges;
	//public:
	//	PickFaceEdges(const std::vector<EDGE> &edges, const std::vector<VERTEX> &verts,const matrix44 &viewProj, const vector2 &hpos)
	//		:PickFace(verts,viewProj,hpos),m_edges(edges){}
	//	float operator()(const FACE &) const;
	//};
};

#include "ISceneNodeFactory.h"

class CSceneNodePolygonModelFactory:public ISceneNodeFactory{
public:
	virtual ISceneNode* Create( boost::property_tree::wptree &tr );
};

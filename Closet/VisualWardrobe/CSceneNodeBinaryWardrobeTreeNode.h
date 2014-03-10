#pragma once

#include "ISceneNodeBoxy.h"
#include "CCoordSysAxis.h"
#include "ptree.h"
#include "IDraggable.h"
#include "IWardrobeCell.h"
#include "IAnnotated.h"
#include "IRemovable.h"
//#include <bitset>

class CSceneNodeBinaryWardrobeTreeNode: 
	public ISceneNodeBoxy, 
	public IPickable, 
	public IDraggable, 
	public IWardrobeCell, 
	public IAnnotated, 
	public IRemovable
{
public:
	//每个柜体都是一棵二叉树,将一个长方体的空间不断二分为更多长方体空间
	virtual void Render() const;
	CSceneNodeBinaryWardrobeTreeNode( 
		CSceneNodeBinaryWardrobeTreeNode *pParent,
		const AABB &aabb);
	~CSceneNodeBinaryWardrobeTreeNode();
	void Split(const CCoordSysAxis &axis,ISceneNodeBoxy *pSeparator,float splitPos,float separatorThickness,bool draggable);//分割
	float DefaultSplitPos(const CCoordSysAxis &axis);
	float DefaultSplitThickness();
	virtual void SetAABB(const AABB &aabb);
	virtual const AABB GetAABB() const;
	static CSceneNodeBinaryWardrobeTreeNode *BuildBWTFromBoxes( const boost::property_tree::wptree &tr);//不用
	virtual const vector2 Drag(const vector2 &displacementHS, const matrix44 &viewProj);
	virtual float Pick(const vector2 &posHS, const matrix44 &viewProj) const;
	virtual void VisualizePick() const;
	virtual void OnInsertion();
	virtual void VisualizeCell();
	virtual float PickCell(const vector2 &posHS,const matrix44 &viewProj) const;
	virtual void SplitCell(int iAxis);//分割
	virtual void InsertContent(class ISceneNodeBoxy *p);
	virtual void ShowCellDimensions();
	virtual void ShowAnnotation() const;
	virtual bool Removable() const;
	virtual void Remove();
	//void SetSeparation();
	void RecursivelySetAliveIfOnlyLeafContainingMarkers(std::vector<vector3>::const_iterator first, std::vector<vector3>::const_iterator last);
	const CCoordSysAxis GetSplitAxis() const{ return m_axis; }
	void ConsistencyCheck() const;
	CSceneNodeBinaryWardrobeTreeNode *Root();
	virtual void UpdateTransform(const matrix44 &parentTransform);

	void ClaimContentsFrom(CSceneNodeBinaryWardrobeTreeNode &theOther);

	template < typename T, typename _OItr > _OItr CollectOrthogonalNeighborsPositiveSide( _OItr dest ){
		return m_pChildren.second->CollectOrthogonalNeighborsHi<T>(dest,m_axis);
	}
	template < typename T, typename _OItr > _OItr CollectOrthogonalNeighborsNegativeSide( _OItr dest ){
		return m_pChildren.first->CollectOrthogonalNeighborsLo<T>(dest,m_axis);
	}
	//template < typename T > T *GetSeparator(){
	//	if(IsLeaf()) return NULL;
	//	return dynamic_cast<T*>(m_pContentOrSeparator);
	//}
	static const std::pair< CSceneNodeBinaryWardrobeTreeNode * , CSceneNodeBinaryWardrobeTreeNode * > BuildBWT(const boost::property_tree::wptree &generator,const boost::property_tree::wptree &params);
	//bool BothChildrenAreLeaves(){ return (!IsLeaf()) && m_pChildren.first->IsLeaf() && m_pChildren.second->IsLeaf(); }
	//bool NoOrthogonalSeparators(){ return NoOrthogonalSeparators_impl(m_axis); }
	bool NoOrthogonalSeparatorsAttached();
	bool Fixed(){ return m_fix!=0; }
	void SetOrthogonalSeparatorsEnabled(bool enabled);
	template < typename T, typename _OItr > _OItr CollectSeparatorsOfType(_OItr dest){
		if(IsLeaf())	return dest;
		T *p( dynamic_cast<T*>(m_pContentOrSeparator) );
		if(p)	*dest++ = p;
		dest = m_pChildren.first->CollectSeparatorsOfType<T>(dest);
		dest = m_pChildren.second->CollectSeparatorsOfType<T>(dest);
		return dest;
	}
	template < typename T, typename _Func > _Func ForEachSeparatorOfType( _Func func ){
		if(IsLeaf())	return func;
		T *p( dynamic_cast<T*>(m_pContentOrSeparator) );
		if(p)	func(p);
		func = m_pChildren.first->ForEachSeparatorOfType<T>(func);
		func = m_pChildren.second->ForEachSeparatorOfType<T>(func);
		return func;
	}
private:
	std::pair<bool,bool> m_splitAxisFixed;
	int m_fix;
	void SetAlive(bool alive);
	bool m_bAlive;
	float m_splitPos,m_separatorThickness;
	AABB m_aabb,m_aabbMod;
	CCoordSysAxis m_axis;
	CSceneNodeBinaryWardrobeTreeNode * m_pParent;
	std::pair<CSceneNodeBinaryWardrobeTreeNode *, CSceneNodeBinaryWardrobeTreeNode *> m_pChildren;
	CSceneNodeBinaryWardrobeTreeNode *m_pDimensionShow;
	ISceneNodeBoxy *m_pContentOrSeparator;
	matrix44 m_worldTransform;

	const AABB GetFirstChildAABB()const;
	const AABB GetSecondChildAABB()const;
	const AABB GetSeparatorAABB()const;
	bool IsLeaf() const;
	void BuildBWTFromAABBs(
		std::vector<AABB>::iterator first,
		std::vector<AABB>::iterator last);
	//void ConstrainSeparator();
	void UpdatePtrDimensionShow();
	void UndeletedReplace(CSceneNodeBinaryWardrobeTreeNode *pReplacedBy);
	CSceneNodeBinaryWardrobeTreeNode *AnotherChild(CSceneNodeBinaryWardrobeTreeNode *p);
	/*static */ISceneNodeBoxy *CreateDefaultSeparator(const CCoordSysAxis &axis);
	CSceneNodeBinaryWardrobeTreeNode *BuildBWT_impl(
		const boost::property_tree::wptree &generator,
		const boost::property_tree::wptree &params);
	
	template < typename T, typename _OItr > _OItr CollectOrthogonalNeighborsLo( _OItr dest , const CCoordSysAxis &axis ){
		if(IsLeaf()){
			return dest;
		}else if(m_axis == axis){
			return m_pChildren.second->CollectOrthogonalNeighborsLo<T>( dest, axis );
		}else{
			T *p( dynamic_cast<T*>(m_pContentOrSeparator) );
			if(p){
				*dest++ = p;
			}
			dest = m_pChildren.first->CollectOrthogonalNeighborsLo<T>( dest, axis );
			dest = m_pChildren.second->CollectOrthogonalNeighborsLo<T>( dest, axis );
			return dest;
		}
	}

	template < typename T, typename _OItr > _OItr CollectOrthogonalNeighborsHi( _OItr dest , const CCoordSysAxis &axis ){
		if(IsLeaf()){
			return dest;
		}else if(m_axis == axis){
			return m_pChildren.first->CollectOrthogonalNeighborsHi<T>( dest, axis );
		}else{
			T *p( dynamic_cast<T*>(m_pContentOrSeparator) );
			if(p){
				*dest++ = p;
			}
			dest = m_pChildren.first->CollectOrthogonalNeighborsHi<T>( dest, axis );
			dest = m_pChildren.second->CollectOrthogonalNeighborsHi<T>( dest, axis );
			return dest;
		}
	}
//	bool NoOrthogonalSeparators_impl(const CCoordSysAxis &axis){
//		return IsLeaf() || ( axis==m_axis && m_pChildren.first->NoOrthogonalSeparators_impl(axis) && m_pChildren.second->NoOrthogonalSeparators_impl(axis) );
//	}
//	template < typename T > static const T Evaluate( const boost::property_tree::wptree &generator,const boost::property_tree::wptree &params, const std::wstring &prop );
//	template < typename T > static const T Evaluate( const boost::property_tree::wptree &tr, const boost::property_tree::wptree &params );
};

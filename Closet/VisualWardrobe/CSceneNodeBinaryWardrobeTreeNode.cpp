#include "CSceneNodeBinaryWardrobeTreeNode.h"
#include "CSceneNodeAABB.h"
#include "MyUtilities.h"
#include "singleton.h"
#include "CProgram.h"
#include "CSceneNodeBoxyAutoTex.h"
#include "CAnnotationStyle.h"
#include "CSceneNodeBoard.h"
#include "ptree.h"
#include <memory>
#include "CSceneNodeFactoryRegistry.h"
#include "ISceneNodeFactory.h"
#include "CSceneNodeFactoryRegistry.h"
#include <algorithm>

const AABB CSceneNodeBinaryWardrobeTreeNode::GetFirstChildAABB() const{
	AABB aabb(m_aabb);
	aabb.second[m_axis.Id()] = m_splitPos-0.5*m_separatorThickness;
	return aabb;
}



const AABB CSceneNodeBinaryWardrobeTreeNode::GetSecondChildAABB() const{
	AABB aabb(m_aabb);
	aabb.first[m_axis.Id()] = m_splitPos+0.5*m_separatorThickness;
	return aabb;
}



const AABB CSceneNodeBinaryWardrobeTreeNode::GetSeparatorAABB() const{
	AABB aabb(m_aabb);
	aabb.first[m_axis.Id()] = m_splitPos - 0.5*m_separatorThickness;
	aabb.second[m_axis.Id()] = m_splitPos + 0.5*m_separatorThickness;
	return aabb;
}

//
//void CSceneNodeBinaryWardrobeTreeNode::ConstrainSeparator(){
//	MyUtilities::ClampR(m_separatorThickness,0.5f*GetDim(m_aabb,m_axis.Id()));
//	MyUtilities::Clamp(
//		m_splitPos,
//		m_aabb.first[m_axis.Id()]+0.5f*m_separatorThickness,
//		m_aabb.second[m_axis.Id()]-0.5f*m_separatorThickness
//		);
//}


float CSceneNodeBinaryWardrobeTreeNode::DefaultSplitPos(const CCoordSysAxis &axis){
	return 0.5*(m_aabb.first[axis.Id()]+m_aabb.second[axis.Id()]);
}


float CSceneNodeBinaryWardrobeTreeNode::DefaultSplitThickness(){
	//return 0.5*(m_aabb.second[m_axis.Id()]-m_aabb.first[m_axis.Id()]);
	return 18.0f;
}


void CSceneNodeBinaryWardrobeTreeNode::Split(const CCoordSysAxis &axis,ISceneNodeBoxy *pSeparator,float splitPos,float separatorThickness,bool draggable){
	assert(IsLeaf());
	assert(!m_pContentOrSeparator);
	
//	assert(m_bAlive);
	m_axis=axis;
	m_splitPos = splitPos;
	m_separatorThickness = separatorThickness;
	assert(m_separatorThickness>=0);
	//ConstrainSeparator();
	m_pChildren.first = new CSceneNodeBinaryWardrobeTreeNode(this,GetFirstChildAABB());
	m_pChildren.second = new CSceneNodeBinaryWardrobeTreeNode(this,GetSecondChildAABB());
	m_pContentOrSeparator = pSeparator;
	if(m_pContentOrSeparator) m_pContentOrSeparator->SetAABB(GetSeparatorAABB());
	
	if(draggable)	GetSingleton<CProgram>()->RegisterPickable(1,this);

	GetSingleton<CProgram>()->UnregisterWardrobeCell(this);
	for(CSceneNodeBinaryWardrobeTreeNode *p(this); p/*&&p->GetSplitAxis().Id()==GetSplitAxis().Id()*/; p=p->m_pParent){
		p->UpdatePtrDimensionShow();
	}
}


void CSceneNodeBinaryWardrobeTreeNode::UpdatePtrDimensionShow(){
	assert(!IsLeaf());
	if(m_pChildren.first->IsLeaf()||m_pChildren.first->GetSplitAxis().Id()==GetSplitAxis().Id() && m_pChildren.first->m_pDimensionShow){
		m_pDimensionShow = m_pChildren.first->m_pDimensionShow;
	}else if(m_pChildren.second->IsLeaf()||m_pChildren.second->GetSplitAxis().Id()==GetSplitAxis().Id() && m_pChildren.second->m_pDimensionShow){
		m_pDimensionShow = m_pChildren.second->m_pDimensionShow;
	}else{
		m_pDimensionShow = NULL;
	}
}


bool CSceneNodeBinaryWardrobeTreeNode::IsLeaf() const{
	assert( !m_pChildren.first == !m_pChildren.second );
	if(m_pChildren.first){
		assert(m_pChildren.first->m_pParent == this);
		assert(m_pChildren.second->m_pParent == this);
	}
	if(m_pParent){
		assert( this == m_pParent->m_pChildren.first || this == m_pParent->m_pChildren.second);
	}
	return NULL==m_pChildren.first;
}



void CSceneNodeBinaryWardrobeTreeNode::Render() const{
	if(!IsLeaf()){
		m_pChildren.first->Render();
		m_pChildren.second->Render();
	}
	if(m_pContentOrSeparator){
		m_pContentOrSeparator->Render();
	}
}



CSceneNodeBinaryWardrobeTreeNode::CSceneNodeBinaryWardrobeTreeNode(
	CSceneNodeBinaryWardrobeTreeNode *pParent,const AABB &aabb):
m_pParent(pParent),
m_pContentOrSeparator(NULL),
m_pChildren(NULL,NULL),
m_axis(0),
m_aabb(aabb),
m_bAlive(false),
m_pDimensionShow(NULL),
m_fix(0),
m_aabbMod(vector3(0,0,0),vector3(0,0,0)),
m_splitAxisFixed(false,false)
{
	m_pDimensionShow = this;
	//GetSingleton<CProgram>()->RegisterWardrobeCell(this);
}



void CSceneNodeBinaryWardrobeTreeNode::SetAABB(const AABB &aabb){
	static int counter(0);
	++counter;
	AABB modifiedAABB ( aabb+m_aabbMod );
	float splitAlignment(0);
	if( 1 == m_fix ){
		splitAlignment = m_splitPos - m_aabb.second[m_axis.Id()];
	}else if( -1 == m_fix ){
		splitAlignment = m_splitPos - m_aabb.first[m_axis.Id()];
	}
	bool bModifiedAlongSplitAxis (
		( m_aabb.first[m_axis.Id()] != modifiedAABB.first[m_axis.Id()] || m_aabb.second[m_axis.Id()] != modifiedAABB.second[m_axis.Id()] )
		&&
		(( 1==m_fix && m_aabb.second[m_axis.Id()] != modifiedAABB.second[m_axis.Id()]) || ( -1==m_fix && m_aabb.first[m_axis.Id()] != modifiedAABB.first[m_axis.Id()] ))
		);
	if(m_bAlive) for(int iAxis(0);iAxis<3;++iAxis){
		//static const boost::property_tree::wptree &limPropNode(GetSingleton<CProgram>()->GetConfig().find(L"MinCellSize")->second);
		//float lim( limPropNode.get_value<float>() );
		//float lim(10.0f);
		float lim(GetSingleton<CProgram>()->GetMinCellSize());
		if(modifiedAABB.first[iAxis]>m_aabb.first[iAxis]){
			MyUtilities::ClampR(modifiedAABB.first[iAxis], max(m_aabb.first[iAxis],m_aabb.second[iAxis]-lim));
		}else if(modifiedAABB.second[iAxis]<m_aabb.second[iAxis]){
			MyUtilities::ClampL(modifiedAABB.second[iAxis], min(m_aabb.second[iAxis],m_aabb.first[iAxis]+lim));
		}
	}
	m_aabb = modifiedAABB;
	//ConstrainSeparator();
	if(IsLeaf()){
		if(m_pContentOrSeparator){
			m_pContentOrSeparator->SetAABB(m_aabb);
			m_aabb = Union(m_aabb, m_pContentOrSeparator->GetAABB());
		}
	}else if(m_bAlive || 0 == m_fix || !bModifiedAlongSplitAxis){
		MyUtilities::ClampR(m_aabb.first[m_axis.Id()], m_splitPos-0.5f*m_separatorThickness);
		MyUtilities::ClampL(m_aabb.second[m_axis.Id()], m_splitPos+0.5f*m_separatorThickness);

		AABB aabb(m_aabb);
		m_pChildren.first->SetAABB(GetFirstChildAABB());
		m_aabb = Union(m_aabb, m_pChildren.first->GetAABB());

		m_pChildren.second->SetAABB(GetSecondChildAABB());
		m_aabb = Union(m_aabb, m_pChildren.second->GetAABB());

		if(m_aabb != aabb){
			m_pChildren.second->SetAABB(GetSecondChildAABB());
			m_pChildren.first->SetAABB(GetFirstChildAABB());
		}

		if(m_pContentOrSeparator){
			m_pContentOrSeparator->SetAABB(GetSeparatorAABB());
			//m_aabb = Union(m_aabb, m_pContentOrSeparator->GetAABB());
		}
	}else{
		AABB aabb(m_aabb);
		if( 1 == m_fix ){
			m_splitPos = max(m_aabb.second[m_axis.Id()] + splitAlignment , m_aabb.first[m_axis.Id()] + 0.5f*m_separatorThickness);
			m_pChildren.first->SetAABB(GetFirstChildAABB());
			MyUtilities::ClampL(m_splitPos , m_pChildren.first->GetAABB().second[m_axis.Id()] + 0.5f*m_separatorThickness);
			m_aabb.second[m_axis.Id()] = m_splitPos - splitAlignment;
			m_aabb = Union(m_aabb, m_pChildren.first->GetAABB());
			m_pChildren.second->SetAABB(GetSecondChildAABB());
		}else if( -1 == m_fix ){
			m_splitPos = min(m_aabb.first[m_axis.Id()] + splitAlignment , m_aabb.second[m_axis.Id()] - 0.5f*m_separatorThickness);
			m_pChildren.second->SetAABB(GetSecondChildAABB());
			MyUtilities::ClampR(m_splitPos , m_pChildren.second->GetAABB().first[m_axis.Id()] - 0.5f*m_separatorThickness);
			m_aabb.first[m_axis.Id()] = m_splitPos - splitAlignment;
			m_aabb = Union(m_aabb, m_pChildren.second->GetAABB());
			m_pChildren.first->SetAABB(GetFirstChildAABB());
		}else{
			assert(0);
		}
		if(m_aabb != aabb){
			m_pChildren.second->SetAABB(GetSecondChildAABB());
			m_pChildren.first->SetAABB(GetFirstChildAABB());
		}

		if(m_pContentOrSeparator){
			m_pContentOrSeparator->SetAABB(GetSeparatorAABB());
		}
	}
}



const AABB CSceneNodeBinaryWardrobeTreeNode::GetAABB() const{
	return m_aabb;
}



CSceneNodeBinaryWardrobeTreeNode *CSceneNodeBinaryWardrobeTreeNode::BuildBWTFromBoxes(const boost::property_tree::wptree &tr){
	using namespace std;
	using namespace boost::property_tree;
	std::vector<AABB> aabbs;
	std::vector<vector3> cellMarkers;
	for(wptree::const_iterator itr = tr.begin(); itr!=tr.end(); ++itr){
		if(itr->first == L"AABB"){
			AABB aabb;
			wistringstream(itr->second.data())>>aabb;
			aabbs.push_back(aabb);
		}else if(itr->first == L"CellMarker"){
			vector3 cellMarker;
			wistringstream(itr->second.data())>>cellMarker;
			cellMarkers.push_back(cellMarker);
		}else{
			assert(0);
		}
	}
	assert(aabbs.size());
	AABB biggest(aabbs.front());
	for(std::vector<AABB>::const_iterator itr = aabbs.begin(); itr!= aabbs.end(); ++itr){
		Enlarge(biggest,*itr);
	}
	CSceneNodeBinaryWardrobeTreeNode *p(new CSceneNodeBinaryWardrobeTreeNode(NULL,biggest));
	p->BuildBWTFromAABBs(aabbs.begin(),aabbs.end());
	p->RecursivelySetAliveIfOnlyLeafContainingMarkers(cellMarkers.begin(),cellMarkers.end());
	return p;
}



void CSceneNodeBinaryWardrobeTreeNode::SetAlive(bool alive){
	if(alive && !m_bAlive){
		GetSingleton<CProgram>()->RegisterAnnotated(this);
		GetSingleton<CProgram>()->RegisterWardrobeCell(this);
	}else if(m_bAlive && !alive){
		GetSingleton<CProgram>()->UnregisterAnnotated(this);
		GetSingleton<CProgram>()->UnregisterWardrobeCell(this);
	}
	m_bAlive = alive;
}


void CSceneNodeBinaryWardrobeTreeNode::RecursivelySetAliveIfOnlyLeafContainingMarkers(std::vector<vector3>::const_iterator first, std::vector<vector3>::const_iterator last){
	if(!IsLeaf()){
		m_pChildren.first->RecursivelySetAliveIfOnlyLeafContainingMarkers(first,last);
		m_pChildren.second->RecursivelySetAliveIfOnlyLeafContainingMarkers(first,last);
		SetAlive(false);
		if( m_splitPos<GetCenter(m_aabb, m_axis.Id()) ){
			m_fix = -1;
		}else{
			m_fix = 1;
		}
	}else{
		for(std::vector<vector3>::const_iterator i(first); i!=last; ++i){
			if(Contains(m_aabb,*i)){
				SetAlive(true);
				return;
			}
		}
		SetAlive(false);
	}
}




void CSceneNodeBinaryWardrobeTreeNode::BuildBWTFromAABBs(
		std::vector<AABB>::iterator first,
		std::vector<AABB>::iterator last)
{
	assert(first<=last);
	if(first==last){
		return;
	}
	for(std::vector<AABB>::iterator i(first); i<last; ++i){
		for(int iAxis = 0; iAxis<3; ++iAxis){
			bool separable(Equal(m_aabb,*i,(iAxis+1)%3) && Equal(m_aabb,*i,(iAxis+2)%3));
			for(std::vector<AABB>::iterator j(first); separable && j<last; ++j){
				if(j!=i){
					const float epsilon(0.01f);
					if(j->second[iAxis]-i->first[iAxis]>epsilon && i->second[iAxis]-j->first[iAxis]>epsilon){
						separable = false;
						break;
					}
				}
			}
			if(separable){
				class smallerThanX:public std::binary_function<const AABB &, const AABB &, bool>{
				private:
					int m_iAxis;
					float m_x;
				public:
					smallerThanX(int iAxis,float x):m_iAxis(iAxis),m_x(x){}
					bool operator()(const AABB &lhs){
						return (lhs.first[m_iAxis]+lhs.second[m_iAxis]) < m_x;
					}
				};
				--last;
				std::swap(*i,*last);
				std::vector<AABB>::iterator m = std::partition(first,last,smallerThanX(iAxis,last->first[iAxis]+last->second[iAxis]));
				Split(CCoordSysAxis(iAxis),CreateDefaultSeparator(CCoordSysAxis(iAxis)),0.5*(last->first[iAxis]+last->second[iAxis]),last->second[iAxis]-last->first[iAxis],false);
				m_pChildren.first->BuildBWTFromAABBs(first,m);
				m_pChildren.second->BuildBWTFromAABBs(m,last);
				return;
			}
		}
	}
	assert(0);
}


//void CSceneNodeBinaryWardrobeTreeNode::SetSeparation(){
//}




void CSceneNodeBinaryWardrobeTreeNode::OnInsertion(){
}



const vector2 CSceneNodeBinaryWardrobeTreeNode::Drag(const vector2 &displacementHS, const matrix44 &viewProj){
	//if(m_pContained.empty()) return;
	matrix44 fullTransform( viewProj*m_worldTransform );
	matrix44 fullTransformInv( fullTransform );
	fullTransformInv.invert();
	vector3 delta = (fullTransformInv*vector4(displacementHS.x,displacementHS.y,0,0)).xyz();
	float prev(m_splitPos);
	//{
	//	float lim( GetSingleton<CProgram>()->GetConfigVar<float>(L"MinCellSize") );
	//	if(delta[m_axis.Id()]>0){
	//		MyUtilities::ClampR(delta[m_axis.Id()],max(0.0f,GetDim(GetSecondChildAABB(),m_axis.Id())-lim));
	//	}else if(delta[m_axis.Id()]<0){
	//		MyUtilities::ClampL(delta[m_axis.Id()],min(0.0f,lim-GetDim(GetFirstChildAABB(),m_axis.Id())));
	//	}
	//	if(0.0f==delta[m_axis.Id()])	return vector2(0,0);
	//}
	//
	m_splitPos+=delta[m_axis.Id()];
	delta[m_axis.Next().Id()] = delta[m_axis.Prev().Id()] = 0.0f;
	MyUtilities::Clamp(
		m_splitPos,
		m_aabb.first[m_axis.Id()]+0.5f*m_separatorThickness,
		m_aabb.second[m_axis.Id()]-0.5f*m_separatorThickness
		);
	{
		float d(m_splitPos-0.5f*m_separatorThickness-m_aabb.first[m_axis.Id()]);
		d-=floor(d);
		m_splitPos-=d;
	}
	m_pChildren.first->SetAABB(GetFirstChildAABB());
	MyUtilities::ClampL(m_splitPos,m_pChildren.first->GetAABB().second[m_axis.Id()]+0.5f*m_separatorThickness);

	m_pChildren.second->SetAABB(GetSecondChildAABB());
	MyUtilities::ClampR(m_splitPos,m_pChildren.second->GetAABB().first[m_axis.Id()]-0.5f*m_separatorThickness);

	m_pChildren.first->SetAABB(GetFirstChildAABB());
	MyUtilities::ClampL(m_splitPos,m_pChildren.first->GetAABB().second[m_axis.Id()]+0.5f*m_separatorThickness);

	m_pChildren.second->SetAABB(GetSecondChildAABB());
	MyUtilities::ClampR(m_splitPos,m_pChildren.second->GetAABB().first[m_axis.Id()]-0.5f*m_separatorThickness);

	if(m_pContentOrSeparator){
		m_pContentOrSeparator->SetAABB(GetSeparatorAABB());
		//m_aabb = Union(m_aabb, m_pContentOrSeparator->GetAABB());
	}
	vector3 r3(0,0,0);
	r3[m_axis.Id()] = m_splitPos-prev;
	vector2 r((fullTransform*vector4(r3,0.0f)).xy());
//	CheckForInvalidSeparator();
	return r;
}



float CSceneNodeBinaryWardrobeTreeNode::Pick(const vector2 &posHS, const matrix44 &viewProj) const{
//	if(m_pContained.empty()) return 2000000.0f;
	matrix44 fullTransform = viewProj*m_worldTransform;
	for(int iAxis(0); iAxis<3; ++iAxis){
		CCoordSysAxis axis(iAxis);
		if( abs((fullTransform*axis.Vector4()).xyz().normalized().z)>0.99f ){
			AABB aabb(GetSeparatorAABB());
			AABB aabbHS(AABBFromTwoPoints( (fullTransform*vector4(aabb.first,1.0f)).xyz() , (fullTransform*vector4(aabb.second,1.0f)).xyz() ));
			if(1+aabbHS.first.z>0 && MyUtilities::IsInClosedRect(posHS,aabbHS.first.xy(),aabbHS.second.xy())){
				return 1+aabbHS.first.z;
			}else{
				return 2000000.0f;
			}
		}
	}
	return 2000000.0f;
}




void CSceneNodeBinaryWardrobeTreeNode::VisualizeCell(){
	glPushMatrix();
	glMultMatrixf((GLfloat*)&m_worldTransform);
	glPushAttrib(GL_CURRENT_BIT);
	glColor3f(1,0,0);
	GetSingleton<CAnnotationStyle>()->DrawAABBSelection(GetAABB());
	glPopAttrib();
	glPopMatrix();
}


float CSceneNodeBinaryWardrobeTreeNode::PickCell(const vector2 &posHS,const matrix44 &viewProj) const{
	matrix44 fullTransform(viewProj*m_worldTransform);
	for(int iAxis(0); iAxis<3; ++iAxis){
		CCoordSysAxis axis(iAxis);
		if( abs((fullTransform*axis.Vector4()).xyz().normalized().z)>0.99f ){
			AABB aabb(GetAABB());
			AABB aabbHS(AABBFromTwoPoints( (fullTransform*vector4(aabb.first,1.0f)).xyz() , (fullTransform*vector4(aabb.second,1.0f)).xyz() ));
			if(1+aabbHS.first.z>0 && MyUtilities::IsInClosedRect(posHS,aabbHS.first.xy(),aabbHS.second.xy())){
				return 1+aabbHS.first.z;
			}else{
				return 2000000.0f;
			}
		}
	}
	return 2000000.0f;
}


void CSceneNodeBinaryWardrobeTreeNode::SplitCell(int iAxis){
	assert(m_bAlive);
	float thickness( DefaultSplitThickness() );
	if( GetSingleton<CProgram>()->GetConfigVar<float>(L"MinCellSize")>(GetDim(GetAABB(),iAxis)-thickness)*0.5f )	return;
	if( (m_splitAxisFixed.first||m_splitAxisFixed.second) && m_axis.Id()!=iAxis ) return;
	Split( CCoordSysAxis(iAxis),CreateDefaultSeparator(CCoordSysAxis(iAxis)),DefaultSplitPos(CCoordSysAxis(iAxis)),thickness,true );
	m_pChildren.first->SetAlive(true);
	m_pChildren.second->SetAlive(true);
	if( m_splitAxisFixed.first ){
		m_pChildren.first->m_splitAxisFixed.first = true;
		m_pChildren.first->m_axis = m_axis;
	}
	if( m_splitAxisFixed.second ){
		m_pChildren.second->m_splitAxisFixed.second = true;
		m_pChildren.second->m_axis = m_axis;
	}
	{
		if(m_pContentOrSeparator) m_pContentOrSeparator->UpdateTransform(m_worldTransform);
		m_pChildren.first->UpdateTransform(m_worldTransform);
		m_pChildren.second->UpdateTransform(m_worldTransform);
	}
}

void CSceneNodeBinaryWardrobeTreeNode::InsertContent(class ISceneNodeBoxy *p){
	assert(m_bAlive);
	if(m_pContentOrSeparator){
		delete m_pContentOrSeparator;
	}
	m_pContentOrSeparator = p;
	SetAABB(m_aabb);
	UpdateTransform(m_worldTransform);
}

void CSceneNodeBinaryWardrobeTreeNode::ShowCellDimensions(){
	vector3 dim(GetDim(m_aabb));
	if(abs(dim.x)<0.01f || abs(dim.y)<0.01f || abs(dim.z)<0.01f) return;
	if(!m_bAlive) return;
	assert(IsLeaf());
	glPushMatrix();
	glMultMatrixf((float*)&m_worldTransform);
	glBegin(GL_LINES);
	glVertex3f(m_aabb.first.x,m_aabb.first.y,m_aabb.first.z);
	glVertex3f(m_aabb.first.x,m_aabb.second.y,m_aabb.first.z);
	glVertex3f(m_aabb.first.x,m_aabb.first.y,m_aabb.first.z);
	glVertex3f(m_aabb.first.x,m_aabb.first.y,m_aabb.second.z);
	glEnd();
	vector3 mid(m_aabb.first.x,GetCenter(m_aabb).y,m_aabb.first.z);
	glRasterPos3fv(&mid.x);
	char s[32];
	_itoa_s(int(GetDim(m_aabb,1)+0.5f),s,30,10);
	for(char *pc = s; *pc; ++pc){
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12,*pc);
	}
	mid = vector3(m_aabb.first.x,m_aabb.second.y,GetCenter(m_aabb).z);
	glRasterPos3fv(&mid.x);
	_itoa_s(int(GetDim(m_aabb,2)+0.5f),s,30,10);
	if(m_pParent){
		assert( this == m_pParent->m_pChildren.first || this == m_pParent->m_pChildren.second);
	}
	for(char *pc = s; *pc; ++pc){
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12,*pc);
	}
	glPopMatrix();
}





void CSceneNodeBinaryWardrobeTreeNode::ShowAnnotation() const{
	vector3 dim(GetDim(m_aabb));
	if(abs(dim.x)<0.01f || abs(dim.y)<0.01f || abs(dim.z)<0.01f) return;
	if(!m_bAlive) return;
	bool bH(true), bV(true);
	float offset(30.0f);
	if(m_pParent){
		if(IsLeaf()){
			const CSceneNodeBinaryWardrobeTreeNode *p(this);
			while(p->m_pParent && p->m_pParent->m_bAlive && (p->IsLeaf()||p->m_pParent->GetSplitAxis().Id()==p->GetSplitAxis().Id())){
				p = p->m_pParent;
			}
			if(this!=p->m_pDimensionShow){
				assert(p->m_pDimensionShow && p->m_pDimensionShow->IsLeaf());
				if(m_pParent->GetSplitAxis().Id() == 2)	bH = false;
				if(m_pParent->GetSplitAxis().Id() == 1)	bV = false;
			}
		}else{
			return;
			//offset = 100.0f;
			bH = bV =false;
			if(m_pParent->GetSplitAxis().Id()!=GetSplitAxis().Id()||!m_pParent->m_bAlive){
				if(GetSplitAxis().Id()==2){
					bH = true;
				}else if(GetSplitAxis().Id()==1){
					bV = true;
				}
			}
		}
	}else{
		bH=bV=false;
	}
	glPushMatrix();
	glMultMatrixf((float*)&m_worldTransform);
	if(bH){
		vector3 a(m_aabb.second.x,m_aabb.first.y,m_aabb.first.z+offset);
		vector3 b(m_aabb.second.x,m_aabb.second.y,m_aabb.first.z+offset);
		GetSingleton<CAnnotationStyle>()->DrawAnnotationEnd(a);
		GetSingleton<CAnnotationStyle>()->DrawAnnotationEnd(b);
		glBegin(GL_LINES);
		glVertex3f(a.x,a.y,a.z);
		glVertex3f(b.x,b.y,b.z);
		glEnd();
	}
	if(bV && !bH){
		vector3 a(m_aabb.second.x,m_aabb.first.y+offset,m_aabb.first.z);
		vector3 b(m_aabb.second.x,m_aabb.first.y+offset,m_aabb.second.z);
		GetSingleton<CAnnotationStyle>()->DrawAnnotationEnd(a);
		GetSingleton<CAnnotationStyle>()->DrawAnnotationEnd(b);
		glBegin(GL_LINES);
		glVertex3f(a.x,a.y,a.z);
		glVertex3f(b.x,b.y,b.z);
		glEnd();
	}
	//float offset(IsLeaf()?0:100);
	vector3 mid(m_aabb.second.x,GetCenter(m_aabb).y-40.0f,m_aabb.first.z+offset+12.0f);
	glRasterPos3fv(&mid.x);
	char s[32];
	if(bH){
		_itoa_s(int(GetDim(m_aabb,1)+0.5f),s,30,10);
		for(char *pc = s; *pc; ++pc){
			glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12,*pc);
		}
	}
	mid = vector3(m_aabb.second.x,m_aabb.first.y+offset+10.0f,GetCenter(m_aabb).z);
	glRasterPos3fv(&mid.x);
	if(bV){
		if(bH){
			glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12,'H');
			glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12,'=');
		}
		_itoa_s(int(GetDim(m_aabb,2)+0.5f),s,30,10);
		for(char *pc = s; *pc; ++pc){
			glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12,*pc);
		}
	}
	glPopMatrix();
}


bool CSceneNodeBinaryWardrobeTreeNode::Removable() const{
	if(IsLeaf()) return false;
	for(CSceneNodeBinaryWardrobeTreeNode *p(m_pChildren.first); !p->IsLeaf(); p=p->m_pChildren.second){
		if(p->GetSplitAxis().Id()!=GetSplitAxis().Id()) return false;
	}
	for(CSceneNodeBinaryWardrobeTreeNode *p(m_pChildren.second); !p->IsLeaf(); p=p->m_pChildren.first){
		if(p->GetSplitAxis().Id()!=GetSplitAxis().Id()) return false;
	}
	return true;
}


void CSceneNodeBinaryWardrobeTreeNode::Remove(){
	assert(m_pParent);
	CSceneNodeBinaryWardrobeTreeNode *pToReset(m_pParent);
	CSceneNodeBinaryWardrobeTreeNode *p(m_pChildren.second);
	while(!p->IsLeaf())		p = p->m_pChildren.first;
	m_splitPos = p->m_pParent->m_splitPos;
	p->m_pParent->UndeletedReplace(p->m_pParent->AnotherChild(p));
	p->m_pParent->m_pChildren.first = p->m_pParent->m_pChildren.second = NULL;
	delete p->m_pParent;
	delete p;
	pToReset->SetAABB(pToReset->GetAABB());
}



CSceneNodeBinaryWardrobeTreeNode::~CSceneNodeBinaryWardrobeTreeNode(){
	GetSingleton<CProgram>()->UnregisterPickable(1,this);
	GetSingleton<CProgram>()->UnregisterAnnotated(this);
	GetSingleton<CProgram>()->UnregisterWardrobeCell(this);
	if(m_pChildren.first){
		delete m_pChildren.first;
		delete m_pChildren.second;
	}
	if(m_pContentOrSeparator){
		delete m_pContentOrSeparator;
	}
}


void CSceneNodeBinaryWardrobeTreeNode::UndeletedReplace(CSceneNodeBinaryWardrobeTreeNode *pReplacedBy){
	assert(m_pParent);
	if(this==m_pParent->m_pChildren.first){
		m_pParent->m_pChildren.first = pReplacedBy;
	}else if(this==m_pParent->m_pChildren.second){
		m_pParent->m_pChildren.second = pReplacedBy;
	}else{
		assert(0);
	}
	pReplacedBy->m_pParent=m_pParent;
	for(CSceneNodeBinaryWardrobeTreeNode *p(m_pParent); p; p=p->m_pParent){
		p->UpdatePtrDimensionShow();
	}
	m_pParent->SetAABB(m_pParent->GetAABB());
}



CSceneNodeBinaryWardrobeTreeNode *CSceneNodeBinaryWardrobeTreeNode::AnotherChild(CSceneNodeBinaryWardrobeTreeNode *p){
	if(p==m_pChildren.first){
		return m_pChildren.second;
	}else{
		assert(p==m_pChildren.second);
		return m_pChildren.first;
	}
}




void CSceneNodeBinaryWardrobeTreeNode::ConsistencyCheck() const{
	if(!IsLeaf()){
		assert(this == m_pChildren.first->m_pParent);
		assert(this == m_pChildren.second->m_pParent);
		m_pChildren.first->ConsistencyCheck();
		m_pChildren.second->ConsistencyCheck();
	}
}

CSceneNodeBinaryWardrobeTreeNode *CSceneNodeBinaryWardrobeTreeNode::Root(){
	CSceneNodeBinaryWardrobeTreeNode *p(this);
	while(p->m_pParent) {
		assert(p->m_pParent!=p);
		p=p->m_pParent;
	}
	return p;
}

void CSceneNodeBinaryWardrobeTreeNode::UpdateTransform(const matrix44 &parentTransform){
	m_worldTransform = parentTransform;
	if(m_pContentOrSeparator)	m_pContentOrSeparator->UpdateTransform(parentTransform);
	if(!IsLeaf()){
		m_pChildren.first->UpdateTransform(m_worldTransform);
		m_pChildren.second->UpdateTransform(m_worldTransform);
	}
}


ISceneNodeBoxy *CSceneNodeBinaryWardrobeTreeNode::CreateDefaultSeparator(const CCoordSysAxis &axis){

	//vector4 uCoeff(1,1,1,0),vCoeff(0,0,0,0);
	//uCoeff[axis.Prev().Id()] = 0;
	//vCoeff[axis.Prev().Id()] = 1;

//	vector4 uCoeff(0,0,0,0),vCoeff(0,0,0,0);
//	uCoeff[axis.Id()] = vCoeff[axis.Id()] = 0.001;
//	uCoeff[axis.Prev().Id()] = vCoeff[axis.Next().Id()] = 0.001;

//	return new CSceneNodeBoxyAutoTex(GetSingleton<CProgram>()->GetTexture(L"testTexture.bmp"),uCoeff,vCoeff,new CSceneNodeAABB(AABB(vector3(0,0,0),vector3(1,1,1))));
	return NSceneNodeBoard::New( GetSingleton<CProgram>()->GetCurrentMaterial(),AABB(vector3(0,0,0),vector3(10,10,10)),axis.Id(),this );
}


void CSceneNodeBinaryWardrobeTreeNode::VisualizePick() const{
	GetSingleton<CAnnotationStyle>()->DrawAABBSelection(m_worldTransform*GetSeparatorAABB());
}


#include "parsing.h"
using namespace Parsing;


const std::pair<CSceneNodeBinaryWardrobeTreeNode * , CSceneNodeBinaryWardrobeTreeNode *> CSceneNodeBinaryWardrobeTreeNode::BuildBWT(const boost::property_tree::wptree &g,const boost::property_tree::wptree &p)
{
	float x( Evaluate<float>(g,p,L"SizeX") );
	float y( Evaluate<float>(g,p,L"SizeY") );
	float z( Evaluate<float>(g,p,L"SizeZ") );
	std::auto_ptr<CSceneNodeBinaryWardrobeTreeNode> pBWT( new CSceneNodeBinaryWardrobeTreeNode( NULL, 
		AABB( vector3(0, -0.5*y, 0), vector3( x, 0.5*y, z ) ) ) );
	CSceneNodeBinaryWardrobeTreeNode *pOutNode( pBWT->BuildBWT_impl( g, p ) );
	//assert(pOutNode);
	return std::make_pair( pBWT.release(), pOutNode );
}


CSceneNodeBinaryWardrobeTreeNode *CSceneNodeBinaryWardrobeTreeNode::BuildBWT_impl(const boost::property_tree::wptree &g, const boost::property_tree::wptree &p){
	using namespace boost::property_tree;
	using namespace boost;
	SetAlive(g.get<bool>(L"Alive",false));

	m_aabbMod.first.x = Evaluate(g,p,L"MinXMod",0.0f);
	m_aabbMod.first.y = Evaluate(g,p,L"MinYMod",0.0f);
	m_aabbMod.first.z = Evaluate(g,p,L"MinZMod",0.0f);
	m_aabbMod.second.x = Evaluate(g,p,L"MaxXMod",0.0f);
	m_aabbMod.second.y = Evaluate(g,p,L"MaxYMod",0.0f);
	m_aabbMod.second.z = Evaluate(g,p,L"MaxZMod",0.0f);
	SetAABB(m_aabb);

	optional< const wptree & > sep(g.get_child_optional(L"Separator"));
	CSceneNodeBinaryWardrobeTreeNode *pOutNode( m_bAlive?this:NULL );
	if(!pOutNode && sep.is_initialized()){
		CCoordSysAxis axis(sep->get<int>(L"Axis"));
		float thickness( Evaluate<float>(*sep,p,L"Thickness") );
		float splitPos( GetCenter(m_aabb,axis.Id()) );
		optional< const wptree & > r(sep->get_child_optional(L"StartSpacing"));
		if(r.is_initialized()){
			splitPos = m_aabb.first[axis.Id()]+Evaluate<float>(*r,p)+0.5*thickness;
			m_fix = -1;
		}else{
			r = sep->get_child_optional(L"EndSpacing");
			if(r.is_initialized()){
				splitPos = m_aabb.second[axis.Id()]-Evaluate<float>(*r,p)-0.5*thickness;
				m_fix = 1;
			}
		}
		std::auto_ptr<ISceneNodeBoxy> pSep;
		{
			optional< const wptree & > r( sep->get_child_optional(L"SpecialSeparator") );
			if(r.is_initialized()&&!r->empty()){
				ISceneNodeFactory *pFac = GetSingleton<CSceneNodeFactoryRegistry>()->GetFactory(r->begin()->first);
				if(pFac){
					ISceneNode *pSN( pFac->Create(r->begin()->second) );
					pSep.reset( dynamic_cast<ISceneNodeBoxy*>( pSN ) );
					if(!pSep.get()){
						delete pSN;
					}
				}
			}
		}
		if(!pSep.get()){
			pSep.reset(CreateDefaultSeparator(axis));
			CSceneNodeBoard *pBoard( dynamic_cast<CSceneNodeBoard*>(pSep.get()) );
			if( pBoard ){
				optional< const wptree & > r( sep->get_child_optional(L"BoardMod") );
				if(r.is_initialized()){
					pBoard->ReadProps(*r,p);
				}
				r = sep->get_child_optional(L"BoardVisual");
				if(r.is_initialized()&&r->size()==1){
					std::auto_ptr<ISceneNode> pSN( GetSingleton<CSceneNodeFactoryRegistry>()->GetFactory(r->begin()->first)->Create(r->begin()->second) );
					if(dynamic_cast< ISceneNodeBoxy* >(pSN.get())){
						std::auto_ptr<ISceneNodeBoxy> pSNB( dynamic_cast< ISceneNodeBoxy* >(pSN.release()) );
						pBoard->GiveVisual( pSNB.release() );
					}
				}
			}
		}
		Split(axis,pSep.release(),splitPos,thickness,false);
		optional< const wptree & > firstChild(g.get_child_optional(L"FirstChild"));
		if(firstChild.is_initialized())	{
			CSceneNodeBinaryWardrobeTreeNode *p( m_pChildren.first->BuildBWT_impl(*firstChild,p) );
			if(p){
				assert(!pOutNode);
				pOutNode = p;
			}
		}
		optional< const wptree & > secondChild(g.get_child_optional(L"SecondChild"));
		if(secondChild.is_initialized()) {
			CSceneNodeBinaryWardrobeTreeNode *p( m_pChildren.second->BuildBWT_impl(*secondChild,p) );
			if(p){
				assert(!pOutNode);
				pOutNode = p;
			}
		}
	}
	return pOutNode;
}



void CSceneNodeBinaryWardrobeTreeNode::ClaimContentsFrom(CSceneNodeBinaryWardrobeTreeNode &theOther){
	assert(m_bAlive);
	assert(theOther.m_bAlive);
	m_splitPos = theOther.m_splitPos;
	m_separatorThickness = theOther.m_separatorThickness;
	m_axis = theOther.m_axis;
	if(m_pChildren.first){
		delete m_pChildren.first;
		delete m_pChildren.second;
	}
	m_pChildren = theOther.m_pChildren;
	theOther.m_pChildren.first = theOther.m_pChildren.second = NULL;
	if(m_pChildren.first){
		m_pChildren.first->m_pParent = this;
		m_pChildren.second->m_pParent = this;
	}
	if(&theOther == theOther.m_pDimensionShow){
		m_pDimensionShow = this;
	}else{
		m_pDimensionShow = theOther.m_pDimensionShow;
		theOther.m_pDimensionShow = &theOther;
	}
	if(m_pContentOrSeparator){
		delete m_pContentOrSeparator;
	}
	m_pContentOrSeparator = theOther.m_pContentOrSeparator;
	theOther.m_pContentOrSeparator = NULL;
	UpdateTransform(m_worldTransform);
	SetAABB(m_aabb);
	if(!IsLeaf())	GetSingleton<CProgram>()->RegisterPickable(1,this);
}




bool CSceneNodeBinaryWardrobeTreeNode::NoOrthogonalSeparatorsAttached(){
	if(IsLeaf()) return true;
	for(const CSceneNodeBinaryWardrobeTreeNode *p( m_pChildren.first ); !p->IsLeaf(); p=p->m_pChildren.second){
		if(p->m_axis != m_axis)  return false;
	}
	for(const CSceneNodeBinaryWardrobeTreeNode *p( m_pChildren.second ); !p->IsLeaf(); p=p->m_pChildren.first){
		if(p->m_axis != m_axis) return false;
	}
	return true;
}




void CSceneNodeBinaryWardrobeTreeNode::SetOrthogonalSeparatorsEnabled(bool enabled){
	{
		CSceneNodeBinaryWardrobeTreeNode *p( m_pChildren.first );
		while(!p->IsLeaf()) p=p->m_pChildren.second;
		p->m_axis = m_axis;
		p->m_splitAxisFixed.second = !enabled;
	}
	{
		CSceneNodeBinaryWardrobeTreeNode *p( m_pChildren.second);
		while(!p->IsLeaf()) p=p->m_pChildren.first;
		p->m_axis = m_axis;
		p->m_splitAxisFixed.first = !enabled;
	}
}
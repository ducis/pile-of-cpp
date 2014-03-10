#include "CSceneNodeBoard.h"
#include "3d.h"
#include "IMaterial.h"
#include "singleton.h"
#include "CProgram.h"
#include "CAnnotationStyle.h"
#include "CCoordSysAxis.h"
#include "CSceneNodeBinaryWardrobeTreeNode.h"
#include "IHoleStyle.h"
#include "ptree.h"
#include "IScene.h"
#include <set>
#include "CCutter.h"
#include "OpenGLUtil.h"

namespace CSceneNodeBoard_impl{};
using namespace CSceneNodeBoard_impl;
void CSceneNodeBoard::Render() const 
{
	glPushAttrib(GL_ALL_ATTRIB_BITS);
	m_board.GetMaterial()->Apply();
	m_texCoordGen.Apply();
	if(m_pVisual.get()) m_pVisual->Render();
	glPopAttrib();
}



const AABB CSceneNodeBoard::GetModifiedAABB() const{
	AABB modified(m_aabb);
	modified.first[(m_up+1)%3]+=m_boardSpatialMod.first.x;
	modified.first[(m_up+2)%3]+=m_boardSpatialMod.first.y;
	modified.first[m_up]+=m_boardSpatialMod.first.z;
	modified.second[(m_up+1)%3]+=m_boardSpatialMod.second.x;
	modified.second[(m_up+2)%3]+=m_boardSpatialMod.second.y;
	modified.second[m_up]+=m_boardSpatialMod.second.z;
	return modified;
}



void CSceneNodeBoard::SetAABB(const AABB &aabb)
{
	m_aabb = aabb;
	AABB modified(GetModifiedAABB());
	if(m_pVisual.get()) m_pVisual->SetAABB(modified);
	vector3 dim(GetDim(modified));
	m_board.SetSize( vector2(dim[(m_up+1)%3],dim[(m_up+2)%3]) );
	m_board.SetThickness( dim[m_up] );
}



const AABB CSceneNodeBoard::GetAABB() const
{
	return m_aabb;
}



void CSceneNodeBoard::SetBoardMaterial (class IMaterial *pMaterial)
{
	m_board.SetMaterial(pMaterial);
}



CSceneNodeBoard::CSceneNodeBoard(class IMaterial *pMaterial,const AABB &aabb,unsigned up, CSceneNodeBinaryWardrobeTreeNode * pOwner, ISceneNodeBoxy *pVisual):
m_pVisual(pVisual),
m_up(up%3),
m_board(vector2(GetDim(aabb,TransformAxis(0,up%3)), GetDim(aabb,TransformAxis(1,up%3))) , GetDim(aabb,TransformAxis(2,up%3)) , pMaterial , up>=3),
m_texCoordGen(UCoeff(up%3),VCoeff(up%3)),
m_pOwner(pOwner),
m_boardSpatialMod(vector3(0,0,0),vector3(0,0,0)),
m_bRemovable(false),
m_pWardrobe(NULL)
{
	if(pVisual)		pVisual->SetAABB(aabb);
	for(int axis=0; axis<2; ++axis){
		for(int positive=0; positive<2; ++positive){
			m_pDrillingRuleOverride[axis][positive] = NULL;
			m_overrideDrillingRule[axis][positive] = false;
		}
	}
	{			
		for(unsigned iUp(0); iUp<6; ++iUp){
			for(unsigned iDir(0); iDir<6; ++iDir){
				assert(InverseTransformDirection(TransformDirection(iDir,iUp),iUp)==iDir);
			}
		}
	}
	m_board.SetName(L"¸ô°å");
	Enable();
}



CSceneNodeBoard::~CSceneNodeBoard(){
	Disable();
}



void CSceneNodeBoard::Enable(){
	GetSingleton<CProgram>()->RegisterPickable(3,this);	
	GetSingleton<CProgram>()->RegisterAnnotated(this);
	m_board.SetEnabled(true);
}
void CSceneNodeBoard::Disable(){
	GetSingleton<CProgram>()->UnregisterAnnotated(this);
	GetSingleton<CProgram>()->UnregisterPickable(3,this);
	m_board.SetEnabled(false);
}


const vector4 CSceneNodeBoard::UCoeff(int iAxis){
	vector4 uCoeff(0,0,0,0);
	uCoeff[iAxis] = 0.002;
	uCoeff[(iAxis+2)%3] = 0.002;
	return uCoeff;
}



const vector4 CSceneNodeBoard::VCoeff(int iAxis){
	vector4 vCoeff(0,0,0,0);
	vCoeff[iAxis] = 0.002;
	vCoeff[(iAxis+1)%3] = 0.002;
	return vCoeff;
}



float CSceneNodeBoard::Pick(const vector2 & posHS, const matrix44 &viewProj) const{
	matrix44 fullTransform = viewProj*m_worldTransform;
	for(int iAxis(0); iAxis<3; ++iAxis){
		CCoordSysAxis axis(iAxis);
		if( abs((fullTransform*axis.Vector4()).xyz().normalized().z)>0.99f ){
			AABB aabb(GetModifiedAABB());
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



void CSceneNodeBoard::VisualizePick() const{
	GetSingleton<CAnnotationStyle>()->DrawAABBSelection(m_worldTransform*GetModifiedAABB());
}



void CSceneNodeBoard::UpdateTransform(const matrix44 &parentTransform){
	m_worldTransform = parentTransform;
	if(m_pVisual.get()) m_pVisual->UpdateTransform(parentTransform);
}



const CBoard &CSceneNodeBoard::GetBoard() const { 
	return m_board; 
}



void CSceneNodeBoard::PunchHoles(){
	m_board.ClearHoles();
	if(m_pOwner){
		std::vector<CSceneNodeBoard *> m_snbs[2];
		m_pOwner->CollectOrthogonalNeighborsNegativeSide<CSceneNodeBoard>(std::back_inserter(m_snbs[0]));
		m_pOwner->CollectOrthogonalNeighborsPositiveSide<CSceneNodeBoard>(std::back_inserter(m_snbs[1]));
		for(int iSide(0); iSide<2; ++iSide){
			for(std::vector<CSceneNodeBoard *>::iterator i(m_snbs[iSide].begin()); i!= m_snbs[iSide].end(); ++i){
				CSceneNodeBoard *p(*i);
				int negNormalThisSpace(iSide?5:2);
				int negNormalThatSpace(p->GetLocalDirection(GetDirection(negNormalThisSpace)));
				assert(negNormalThatSpace!=2);
				assert(negNormalThatSpace!=5);
				int positiveXThatSpace(p->GetLocalDirection(GetDirection(0)));
				int positiveYThatSpace(p->GetLocalDirection(GetDirection(1)));
				vector3 thatPosCommonSpace(GetCenter(p->GetModifiedAABB()));
				vector3 thisPosCommonSpace(GetCenter(GetModifiedAABB()));
				vector3 dispCommonSpace(thatPosCommonSpace-thisPosCommonSpace);
				vector2 dispThisSpace(dispCommonSpace[GetDirection(0)],dispCommonSpace[GetDirection(1)]);
				p->CheckDrillingRules();
				if(positiveXThatSpace%3==2){
					assert(positiveYThatSpace%3!=2);
					//thatPX
					int n(p->GetBoard().GetXYHoleCount(negNormalThatSpace));
					for(int i(0); i<n; ++i){
						float t( p->GetBoard().GetXYHolePos(negNormalThatSpace,i) );
						IHoleStyle *s( p->GetBoard().GetXYHole(negNormalThatSpace,i) );
						vector2 pos( 0, positiveYThatSpace<3 ? t : -t );
						pos+=dispThisSpace;
						m_board.PunchZHole(pos,CBoard::Hole(s->GetDiameter(),s->GetExtrude()),iSide);
					}
				}else{
					assert(positiveYThatSpace%3==2);
					int n(p->GetBoard().GetXYHoleCount(negNormalThatSpace));
					for(int i(0); i<n; ++i){
						float t( p->GetBoard().GetXYHolePos(negNormalThatSpace,i) );
						IHoleStyle *s( p->GetBoard().GetXYHole(negNormalThatSpace,i) );
						vector2 pos( positiveXThatSpace<3 ? t : -t, 0 );
						pos+=dispThisSpace;
						m_board.PunchZHole(pos,CBoard::Hole(s->GetDiameter(),s->GetExtrude()),iSide);
					}
				}
			}
		}
	}
	CheckDrillingRules();
	if(m_bRemovable){
		m_board.PunchXYHoles();
	}
	const std::set<CCutter*> &cutters( GetSingleton<CProgram>()->GetScene()->GetCutterSet() );
	matrix44 invT(m_worldTransform);
	invT.invert();
	m_board.ClearCuts();
	const AABB modified(GetModifiedAABB());
	const vector3 center(GetCenter(modified));
	for(std::set<CCutter*>::const_iterator i( cutters.begin() ) ; i!=cutters.end() ; ++i){
		AABB cutterAABB_WS( AABB( vector3((*i)->Min(),-1000), vector3((*i)->Max(),5000) ) );

		for(int stat(0); stat<5; ++stat){
			const CCutter::GuardData &guard( (*i)->GetGuard(0!=(stat&1),0!=(stat&2)) );
			if( RoughlyIn( modified, invT * guard.volumeOfEffect, 0.01f ) ){
				cutterAABB_WS.first.x-=guard.thickness.x;
				cutterAABB_WS.first.y-=guard.thickness.y;
				cutterAABB_WS.second.x+=guard.thickness.x;
				cutterAABB_WS.second.y+=guard.thickness.y;
			}
		}
		const AABB cutterAABB( invT * cutterAABB_WS );
		const AABB translated(cutterAABB.first-center,cutterAABB.second-center);
		if(Intersect(modified,cutterAABB)){
			m_board.Cut(
				vector2(translated.first[(m_up+1)%3], translated.first[(m_up+2)%3]), 
				vector2(translated.second[(m_up+1)%3], translated.second[(m_up+2)%3])
				);
		}
	}
}


int CSceneNodeBoard::GetDirection(int localDirection){

	return TransformDirection(localDirection,m_up);
}



int CSceneNodeBoard::GetLocalDirection(int direction){
	return InverseTransformDirection(direction,m_up);
}


unsigned CSceneNodeBoard::TransformDirection(unsigned direction, unsigned up){
	assert(up<6);
	assert(direction<6);
	static const unsigned transform[6][3] = {
		{1,2,0},//,4,5,3},
		{2,0,1},//,0,5,4},
		{0,1,2},//,0,1,5},
		{5,4,3},//,2,2,0},
		{3,5,4},
		{4,3,5}
	};
	return (transform[up][direction%3]+(direction/3)*3)%6;
}



unsigned CSceneNodeBoard::InverseTransformDirection(const unsigned direction, const unsigned up){
	assert(direction<6);
	assert(up<6);
	static unsigned transform[6][6] = {
		{-1,-1,-1,-1,-1,-1},
		{-1,-1,-1,-1,-1,-1},
		{-1,-1,-1,-1,-1,-1},
		{-1,-1,-1,-1,-1,-1},
		{-1,-1,-1,-1,-1,-1},
		{-1,-1,-1,-1,-1,-1}
	};
	bool initialized(false);
	if(!initialized){
		initialized = true;
		for(unsigned iUp(0); iUp<6; ++iUp){
			for(unsigned iDir(0); iDir<6; ++iDir){
				transform[iUp][TransformDirection(iDir,iUp)] = iDir;
			}
		}
		{
			int c[6]={0,0,0,0,0,0};
			for(int i=0; i<6; ++i){
				for(int j=0; j<6; ++j){
					assert(transform[i][j]<6);
					++c[transform[i][j]];
				}
			}
			for(int i=0; i<6; ++i){
				assert(c[i] == 6);
			}
		}
	}
	return transform[up][direction];
}



unsigned CSceneNodeBoard::TransformAxis(unsigned int iAxis, unsigned int up){
	assert(up<6);
	assert(iAxis<3);
	assert(TransformDirection(iAxis,up)%3 == TransformDirection(iAxis+3,up)%3);
	assert(abs(int(TransformDirection(iAxis,up))-int(TransformDirection(iAxis+3,up)))==3);
	return TransformDirection(iAxis,up)%3;
}



void CSceneNodeBoard::CheckDrillingRules(){
	for(int axis(0); axis<2; ++axis){
		for(int positive(0); positive<2; ++positive){
			m_board.ApplyDrillingRule(
				m_overrideDrillingRule[axis][positive] ? 
				m_pDrillingRuleOverride[axis][positive] : 
				GetSingleton<CProgram>()->GetCurrentDrillingRule(GetDirection(positive*3+axis)),
				axis,positive);
		}
	}
}


#include "Parsing.h"
using namespace Parsing;

void CSceneNodeBoard::ReadProps(const boost::property_tree::wptree &g,const boost::property_tree::wptree &p){
	//for(boost::property_tree::wptree::const_iterator i(tr.begin()); i!=tr.end(); ++i){
	//	std::wstring ws( i->first );
	//}
	//m_boardSpatialMod.first.x = tr,
	m_boardSpatialMod.first.x = Evaluate(g,p,L"MinXMod",0.0f);
	m_boardSpatialMod.first.y = Evaluate(g,p,L"MinYMod",0.0f);
	m_boardSpatialMod.first.z = Evaluate(g,p,L"MinZMod",0.0f);
	m_boardSpatialMod.second.x = Evaluate(g,p,L"MaxXMod",0.0f);
	m_boardSpatialMod.second.y = Evaluate(g,p,L"MaxYMod",0.0f);
	m_boardSpatialMod.second.z = Evaluate(g,p,L"MaxZMod",0.0f);
	m_board.SetName(g.get<std::wstring>(L"Name",L"¸ô°å"));
	unsigned capping[2][2] = {
		{ g.get<unsigned>(L"MinXCap",0) , g.get<unsigned>(L"MaxXCap",0) },
		{ g.get<unsigned>(L"MinYCap",0) , g.get<unsigned>(L"MaxYCap",0) }
	};
	m_board.SetCapping(false,false,g.get<unsigned>(L"MinXCap",0));
	m_board.SetCapping(false,true ,g.get<unsigned>(L"MaxXCap",0));
	m_board.SetCapping(true ,false,g.get<unsigned>(L"MinYCap",0));
	m_board.SetCapping(true ,true ,g.get<unsigned>(L"MaxYCap",0));
	//SetAABB(m_aabb);
}

ISceneNodeBoxy *CSceneNodeBoard::CreateDefaultVisual(const AABB &aabb){
	return new CSceneNodeAABB(aabb);
}


void CSceneNodeBoard::ShowAnnotation() const{
	if(!GetSingleton<CProgram>()->GetConfigVar<bool>(L"ShowBoardNames")) return;
	glPushAttrib(GL_CURRENT_BIT);
	glPushMatrix();
	glMultMatrixf(&m_worldTransform.col[0][0]);
	const vector3 center(GetCenter(GetAABB()));
	if(GetBoardRemovable()){
		glColor3f(0,0.8,0.8);
	}else{
		glColor3f(0,0.8,0.0);
	}
	glRasterPos3fv(&center.x);
	OpenGLUtil::DrawText(m_board.GetName());
	if(GetBoardRemovable()){
		OpenGLUtil::DrawText(L"(»î¶¯)");
	}
	glPopMatrix();
	glPopAttrib();
}

bool CSceneNodeBoard::CanBeRemovable(){
	return 2==m_up && m_pOwner && !m_pOwner->Fixed() && m_pOwner->NoOrthogonalSeparatorsAttached();
}

void CSceneNodeBoard::GiveVisual(ISceneNodeBoxy *pVisual){
	m_pVisual.reset(pVisual);
}

void CSceneNodeBoard::SetBoardRemovable(bool removable){ 
	m_bRemovable = removable && CanBeRemovable(); 
	if(m_pOwner){
		m_pOwner->SetOrthogonalSeparatorsEnabled(!m_bRemovable);
	}
}
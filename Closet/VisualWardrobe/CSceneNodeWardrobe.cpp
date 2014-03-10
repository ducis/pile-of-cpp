#include "CSceneNodeWardrobe.h"
#include "CSceneNodeBinaryWardrobeTreeNode.h"
#include "CCutter.h"
#include "singleton.h"
#include "CProgram.h"
#include "IScene.h"
#include <algorithm>
#include <functional>
#include <utility>
#include "MyUtilities.h"
#include "CSceneNodeBoard.h"
#include "OpenGLUtil.h"


namespace CSceneNodeWardrobe_impl{
}
using namespace CSceneNodeWardrobe_impl;
using namespace std;
using namespace MyUtilities;

//CSceneNodeWardrobe::CSceneNodeWardrobe(CSceneNodeWardrobeSelector &selector, CSceneNodeCutterGuard &cutterGuardA, CSceneNodeCutterGuard &cutterGuardB)
//:m_pSelector(&selector),m_pCutterGuardMinY(&cutterGuardA),m_pCutterGuardMaxY(&cutterGuardB),m_aabb(m_pSelector->GetAABB())
CSceneNodeWardrobe::CSceneNodeWardrobe(CSceneNodeWardrobeSelector &selector)
:m_pSelector(&selector),
m_aabb(selector.GetAABB()),
m_pCutterGuardMinY( new CSceneNodeCutterGuard(m_aabb,
				   NSceneNodeBoard::New( GetSingleton<CProgram>()->GetCurrentMaterial(),m_pSelector->GetAABB(),0,NULL ),
				   NSceneNodeBoard::New( GetSingleton<CProgram>()->GetCurrentMaterial(),m_pSelector->GetAABB(),1,NULL ))
				   ),
m_pCutterGuardMaxY( new CSceneNodeCutterGuard(m_aabb,
				   NSceneNodeBoard::New( GetSingleton<CProgram>()->GetCurrentMaterial(),m_pSelector->GetAABB(),0,NULL ),
				   NSceneNodeBoard::New( GetSingleton<CProgram>()->GetCurrentMaterial(),m_pSelector->GetAABB(),1,NULL ))
				   )
{
	m_pSelector->SetWardrobe(this);
	GetSingleton<CProgram>()->RegisterAnnotated(this);
	GetSingleton<CProgram>()->RegisterOrderedObject(*this);
}



CSceneNodeWardrobe::~CSceneNodeWardrobe(){
	GetSingleton<CProgram>()->UnregisterOrderedObject(*this);
	GetSingleton<CProgram>()->UnregisterAnnotated(this);
}


void CSceneNodeWardrobe::Render() const{
	m_pSelector->Render(); 
	m_pCutterGuardMinY->Render(); 
	m_pCutterGuardMaxY->Render();
}

//namespace CSceneNodeWardrobe_impl{
//}
void CSceneNodeWardrobe::CheckCutterIntersection(){
	if(!m_pSelector->GetOutNode()) return;
	const std::set<CCutter*> &cutters(GetSingleton<CProgram>()->GetScene()->GetCutterSet());
//	std::for_each(cutters.begin(),cutters.end(),std::bind1st(std::mem_fun(&CSceneNodeWardrobe::CheckCutterIntersection_impl),this));
	CCutter *pMinYCutter(NULL), *pMaxYCutter(NULL);
	//const AABB aabb(m_aabb);
	const AABB aabb(m_pSelector->GetOutNode()->GetAABB());
	matrix44 worldInv(m_worldTransform);
	worldInv.invert();
	for(std::set<CCutter*>::const_iterator i(cutters.begin()); i!=cutters.end() && (NULL==pMinYCutter||NULL==pMaxYCutter); ++i){
		CCutter *pCutter(*i);
		const AABB aabbOS(worldInv*AABB(vector3(pCutter->Min(),0.0f),vector3(pCutter->Max(),0.0f)));
		if( Intersect(make_pair(aabbOS.first.x,aabbOS.second.x),make_pair(aabb.first.x,aabb.second.x)) ){
			if(IsInOpenRange(aabbOS.second.y,aabb.first.y,aabb.second.y)){
				assert(!pMinYCutter);
				pMinYCutter = pCutter;
			}else if(IsInOpenRange(aabbOS.first.y,aabb.first.y,aabb.second.y)){
				assert(!pMaxYCutter);
				pMaxYCutter = pCutter;
			}
		}
	}
	m_pCutterGuardMinY->SetCutter(pMinYCutter);
	m_pCutterGuardMaxY->SetCutter(pMaxYCutter);
}




void CSceneNodeWardrobe::UpdateTransform(const matrix44 &parentTransform){ 
	m_worldTransform = parentTransform;
	m_pSelector->UpdateTransform(m_worldTransform); 
	m_aabb = m_pSelector->GetAABB();

	//AABB newAABB(m_pSelector->GetAABB());
	//if(m_pSelector->GetOutNode()){
	//	const AABB innerAABB(m_pSelector->GetOutNode()->GetAABB());
	//	newAABB.first.z = innerAABB.first.z;
	//	newAABB.second.z = innerAABB.second.z;
	//}
	//m_pCutterGuardMinY->SetAABB(newAABB);
	//m_pCutterGuardMaxY->SetAABB(newAABB);
	SetCutterGuardsAABB();
	CheckCutterIntersection();
	m_pCutterGuardMinY->UpdateTransform(m_worldTransform);
	m_pCutterGuardMaxY->UpdateTransform(m_worldTransform);
}


void CSceneNodeWardrobe::SetAABB(const AABB &aabb){ 
	m_pSelector->SetAABB(aabb); 
	m_aabb = m_pSelector->GetAABB();
	CheckCutterIntersection();
	//AABB newAABB(m_pSelector->GetAABB());
	//if(m_pSelector->GetOutNode()){
	//	const AABB innerAABB(m_pSelector->GetOutNode()->GetAABB());
	//	newAABB.first.z = innerAABB.first.z;
	//	newAABB.second.x = innerAABB.second.z;
	//}
	//m_pCutterGuardMinY->SetAABB(newAABB);
	//m_pCutterGuardMaxY->SetAABB(newAABB);
	SetCutterGuardsAABB();
}



void CSceneNodeWardrobe::SetCutterGuardsAABB(){
	if(m_pSelector->GetOutNode()){
		const AABB aabb(m_pSelector->GetOutNode()->GetAABB());
		m_pCutterGuardMinY->SetAABB(aabb);
		m_pCutterGuardMaxY->SetAABB(aabb);
	}
}


void CSceneNodeWardrobe::ShowAnnotation()const{
	glPushAttrib(GL_CURRENT_BIT);
	glPushMatrix();
	glMultMatrixf(&m_worldTransform.col[0][0]);
	const AABB aabb(GetAABB());
	const vector3 center(0.5f*(aabb.first.xy()+aabb.second.xy()), aabb.first.z*0.3f+aabb.second.z*0.7f);
	glColor3f(0,0,1.0f);
	glRasterPos3fv(&center.x);
	//OpenGLUtil::DrawText(m_board.GetName());
	std::wstringstream buf;
	buf<<L"¹ñÌå "<<GetSingleton<CProgram>()->GetObjectId(*this)+1;
	OpenGLUtil::DrawText(buf.str());
	glPopMatrix();
	glPopAttrib();
}
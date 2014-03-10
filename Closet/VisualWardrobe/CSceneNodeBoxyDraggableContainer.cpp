#include "CSceneNodeBoxyDraggableContainer.h"
#include "CCoordSysAxis.h"
#include "myutilities.h"
#include <algorithm>
#include <functional>
#include "CSceneNodeObjectSet.h"
#include "singleton.h"
#include "CProgram.h"
#include "CConstraintOutOfAABB.h"
#include "IScene.h"
#include "CBoxyDraggableContainerAxialAnnotator.h"
#include "CAnnotationStyle.h"

CSceneNodeBoxyDraggableContainer::CSceneNodeBoxyDraggableContainer(float rotZInDeg,const vector3 &translation,bool bCreateConstraint):
m_rotZInDeg(fmod(rotZInDeg,360.0f)),m_translation(translation),m_pConstraint(bCreateConstraint?new CConstraintOutOfAABB(GetAABB()):NULL),
m_pConnectedOnLeft(NULL),m_pConnectedOnRight(NULL)
{
	for(int i=0; i<6; ++i){
		m_draggableSurfaces[DraggableSurfaceType::IntToType(i)].reset( 
			new DraggableSurfaceType( this , DraggableSurfaceType::IntToType(i) , CObjectConstRef<matrix44>(m_worldTransform) , m_pConstraint.get()) 
			);
	}
}


void CSceneNodeBoxyDraggableContainer::Render() const{
	glPushMatrix();
	glTranslatef(m_translation.x,m_translation.y,m_translation.z);
	glRotatef(m_rotZInDeg,0,0,1);
	std::for_each(m_pContained.begin(),m_pContained.end(),std::mem_fun(&ISceneNodeBoxy::Render));
	glPopMatrix();
}


void CSceneNodeBoxyDraggableContainer::SetAABB_impl(const AABB &aabb,int from){
	const AABB original(GetAABB());
	SetAABBThis(aabb);
	AABB modified(GetAABB());
	if(from>=0 && m_pConnectedOnLeft){
		AABB thatAabb(m_pConnectedOnLeft->GetAABB());
		const AABB thatOriginal(thatAabb);
		thatAabb.second.x+=modified.second.x - original.second.x;
		thatAabb.second.z+=modified.second.z - original.second.z;
		thatAabb.second.y+=modified.first.y - original.first.y;
		m_pConnectedOnLeft->SetAABB_impl(thatAabb,1);
		thatAabb = m_pConnectedOnLeft->GetAABB();
		modified.second.x = original.second.x + thatAabb.second.x - thatOriginal.second.x;
		modified.second.z = original.second.z + thatAabb.second.z - thatOriginal.second.z;
		modified.first.y = original.first.y + thatAabb.second.y - thatOriginal.second.y;
	}
	if(from<=0 && m_pConnectedOnRight){
		AABB thatAabb(m_pConnectedOnRight->GetAABB());
		const AABB thatOriginal(thatAabb);
		thatAabb.second.x+=modified.second.x - original.second.x;
		thatAabb.second.z+=modified.second.z - original.second.z;
		thatAabb.first.y+=modified.second.y - original.second.y;
		m_pConnectedOnRight->SetAABB_impl(thatAabb,-1);
		thatAabb = m_pConnectedOnRight->GetAABB();
		modified.second.x = original.second.x + thatAabb.second.x - thatOriginal.second.x;
		modified.second.z = original.second.z + thatAabb.second.z - thatOriginal.second.z;
		modified.second.y = original.second.y + thatAabb.first.y - thatOriginal.first.y;
	}
	SetAABBThis(modified);
}


void CSceneNodeBoxyDraggableContainer::SetAABBThis(const AABB &aabb){
	for(std::vector<ISceneNodeBoxy*>::iterator i(m_pContained.begin()) ; i<m_pContained.end() ; ++i){
		(*i)->SetAABB(aabb);
	}
	UpdateTransform(m_parentTransform);
}


void CSceneNodeBoxyDraggableContainer::SetAABB(const AABB &aabb){
	SetAABB_impl(aabb,0);
}


const AABB CSceneNodeBoxyDraggableContainer::GetAABB() const{
	return m_pContained.empty()?MinimumAABB():m_pContained.front()->GetAABB();
}


const vector2 CSceneNodeBoxyDraggableContainer::Drag(const vector2 &displacementHS, const matrix44 &viewProj){
	if(m_pContained.empty()) return displacementHS;
	if(m_pConnectedOnLeft) {
		m_pConnectedOnLeft->DisconnectFrom(*this);
		DisconnectFrom(*m_pConnectedOnLeft);
	}
	if(m_pConnectedOnRight) {
		m_pConnectedOnRight->DisconnectFrom(*this);
		DisconnectFrom(*m_pConnectedOnRight);
	}
	matrix44 fullTransform(viewProj*m_parentTransform);
	matrix44 fullTransformInv(fullTransform);
	fullTransformInv.invert();
	vector3 delta( (fullTransformInv*vector4(displacementHS.x,displacementHS.y,0,0)).xyz() );
	//delta.x = floor(delta.x+0.5f);
	//delta.y = floor(delta.y+0.5f);
	delta.z = 0;
	vector2 prevPosHS( (fullTransform*vector4(m_translation,1.0f)).xy() );
	matrix44 world(GetTransform());
	matrix44 parentInv(m_parentTransform);
	parentInv.invert();
	if(m_pConstraint.get()) GetSingleton<CProgram>()->GetScene()->GetConstraintComposite()->RemoveConstraint(m_pConstraint.get());
	vector3 newTranslation(m_translation+delta);
	newTranslation.x = floor(newTranslation.x+0.5f);
	newTranslation.y = floor(newTranslation.y+0.5f);
	m_translation = parentInv*GetSingleton<CProgram>()->GetScene()->GetConstraintComposite()->Constrain(
		(m_parentTransform*vector4(m_translation,1.0f)).xyz(), 
		(m_parentTransform*vector4(newTranslation,1.0f)).xyz(), 
		TransformWithoutTranslation(world,GetAABB())
		);
	for(int i=0;i<3;++i){
		m_translation[i]=floor(m_translation[i]+0.5f);
	}
	vector2 curPosHS( (fullTransform*vector4(m_translation,1.0f)).xy() );
	if(m_pConstraint.get())  GetSingleton<CProgram>()->GetScene()->GetConstraintComposite()->AddConstraint(m_pConstraint.get());
	UpdateTransform(m_parentTransform);
	return curPosHS-prevPosHS;
}


float CSceneNodeBoxyDraggableContainer::Pick(const vector2 &posHS, const matrix44 &viewProj) const{
	if(m_pContained.empty()) return 2000000.0f;
	matrix44 fullTransform = viewProj*GetTransform();
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



const matrix44 &CSceneNodeBoxyDraggableContainer::GetTransform() const{
	m_worldTransform = m_parentTransform*TranslateMatrix44(m_translation.x,m_translation.y,m_translation.z)*RotateRadMatrix44('z',m_rotZInDeg*PI/180.0f);
	return m_worldTransform;
}



void CSceneNodeBoxyDraggableContainer::RipObjectSet(CSceneNodeObjectSet *pObjectSet){
	pObjectSet->ClaimChildrenByType<ISceneNodeBoxy>(std::back_inserter(m_pContained));
	if(m_pConnectedOnRight){
		ConnectTo(*m_pConnectedOnRight);
	}
	if(m_pConnectedOnLeft){
		ConnectTo(*m_pConnectedOnLeft);
	}
}



void CSceneNodeBoxyDraggableContainer::OnInsertion(){
	GetSingleton<CProgram>()->RegisterPickable(0,this);
	std::for_each( m_pContained.begin(), m_pContained.end(), std::mem_fun( &ISceneNode::OnInsertion ) );
	//SetAABB(m_aabb);
	UpdateTransform(m_parentTransform);
	if(m_pConstraint.get()) {
		GetSingleton<CProgram>()->GetScene()->GetConstraintComposite()->AddConstraint(m_pConstraint.get());
	}
}



void CSceneNodeBoxyDraggableContainer::UpdateTransform(const matrix44 &parentTransform){
	m_parentTransform = parentTransform;
	matrix44 fullTransform(GetTransform());
	if(m_pConstraint.get()) m_pConstraint->SetAABB(fullTransform*GetAABB());
	for(std::vector<ISceneNodeBoxy*>::iterator i(m_pContained.begin()); i!=m_pContained.end(); ++i){
		(*i)->UpdateTransform(fullTransform);
	}
	if(m_axialAnnotator.get()){
		m_axialAnnotator->OnSpatialChange();
	}
}



void CSceneNodeBoxyDraggableContainer::Rotate90Deg(){
	matrix44 world(GetTransform());
	if(m_pConstraint.get()) GetSingleton<CProgram>()->GetScene()->GetConstraintComposite()->RemoveConstraint(m_pConstraint.get());
	const AABB aabb(GetAABB());
	vector3 center( GetCenter(aabb) );
	float newRot( m_rotZInDeg+90.0f );
	vector3 translationCentered( m_translation+(RotateRadMatrix44('z',m_rotZInDeg*(PI/180.0))*vector4(center,1)).xyz() );
	vector3 newTranslation( translationCentered-(RotateRadMatrix44('z',newRot*(PI/180.0))*vector4(center,1)).xyz() );
	//if(!GetSingleton<CProgram>()->GetScene()->GetConstraintComposite()->Violated(
	//	m_parentTransform*(m_translation), 
	//	TransformWithoutTranslation(world*RotateRadMatrix44('z',0.5*PI),GetAABB())
	//	))
	//{
	//	m_rotZInDeg+=90.0f;
	//	while(m_rotZInDeg>=360.0f){
	//		m_rotZInDeg-=360.0f;
	//	}
	//	UpdateTransform(m_parentTransform);
	//}
	if(!GetSingleton<CProgram>()->GetScene()->GetConstraintComposite()->Violated(
		vector3(0,0,0) , m_parentTransform*TranslateMatrix44(newTranslation.x,newTranslation.y,newTranslation.z)*RotateRadMatrix44('z',newRot*PI/180.0f)*aabb))
	{
		m_rotZInDeg = newRot;
		m_translation = newTranslation;
		UpdateTransform(m_parentTransform);
	}
	if(m_pConstraint.get())  GetSingleton<CProgram>()->GetScene()->GetConstraintComposite()->AddConstraint(m_pConstraint.get());
}




void CSceneNodeBoxyDraggableContainer::SetSurfaceDraggable(int mask){
	for(int i=0; i<6; ++i){
		if( mask & (1<<DraggableSurfaceType::IntToType(i)) ){
			GetSingleton<CProgram>()->RegisterPickable( 1 , m_draggableSurfaces[i].get() );
		}else{
			GetSingleton<CProgram>()->UnregisterPickable( 1 , m_draggableSurfaces[i].get() );
		}
	}
}




void CSceneNodeBoxyDraggableContainer::EnableAutoAnnotation(){
	if(!m_axialAnnotator.get()){
		m_axialAnnotator.reset(new CAxialAnnotator(*this));
	}
}



void CSceneNodeBoxyDraggableContainer::VisualizePick() const{
	GetSingleton<CAnnotationStyle>()->DrawAABBSelection(m_worldTransform*GetAABB());
}



CSceneNodeBoxyDraggableContainer::~CSceneNodeBoxyDraggableContainer(){
	SetSurfaceDraggable(0);
	Clear();
	GetSingleton<CProgram>()->UnregisterPickable(0,this);
	if(m_pConstraint.get()) GetSingleton<CProgram>()->GetScene()->GetConstraintComposite()->RemoveConstraint(m_pConstraint.get());
}


void CSceneNodeBoxyDraggableContainer::Insert(ISceneNodeBoxy *p) { 
	m_pContained.push_back(p); 
	IConnectible *pC( dynamic_cast<IConnectible*>(p) );
	if(pC) ConnectChild(pC);
}
void CSceneNodeBoxyDraggableContainer::Clear(){ 
	if(m_pConnectedOnRight){
		DisconnectFrom(*m_pConnectedOnRight);
	}
	if(m_pConnectedOnLeft){
		DisconnectFrom(*m_pConnectedOnLeft);
	}
	for(std::vector<ISceneNodeBoxy*>::iterator i(m_pContained.begin()); i!=m_pContained.end(); ++i) delete *i;
	m_pContained.clear(); 
}


bool CSceneNodeBoxyDraggableContainer::ConnectTo(const IConnectible &theOther){
	const CSceneNodeBoxyDraggableContainer *pOther( dynamic_cast<const CSceneNodeBoxyDraggableContainer *>(&theOther) );
	if( !pOther ) return false;
	if( abs(pOther->m_rotZInDeg-m_rotZInDeg)>0.001 ) return false;
	matrix44 invTransform( GetTransform() );
	invTransform.invert();
	const AABB theOthersAabb( invTransform * pOther->GetTransform() * pOther->GetAABB() );//( pOther->GetAABB() + pOther->m_translation - m_translation );
	const AABB aabb(GetAABB());
	bool newConnection(false);
	if( Intersect( 
		std::make_pair( vector2(aabb.first.x,aabb.first.z) , vector2(aabb.second.x,aabb.second.z) ),
		std::make_pair( vector2(theOthersAabb.first.x,theOthersAabb.first.z) , vector2(theOthersAabb.second.x,theOthersAabb.second.z) )
		) )
	{
		if(abs(theOthersAabb.first.y-aabb.second.y)<0.01 && !m_pConnectedOnRight){
			m_pConnectedOnRight = const_cast<CSceneNodeBoxyDraggableContainer*>(pOther);
			newConnection = true;
		}else if(abs(theOthersAabb.second.y-aabb.first.y)<0.01 && !m_pConnectedOnLeft){
			m_pConnectedOnLeft = const_cast<CSceneNodeBoxyDraggableContainer*>(pOther);
			newConnection = true;
		}
	}
	if(newConnection){
		for(int i=0; i<6; ++i){
			m_draggableSurfaces[i]->AddConstraintToDisable(pOther->m_pConstraint.get());
		}
		for(std::vector<ISceneNodeBoxy*>::iterator i( m_pContained.begin() ); i!=m_pContained.end(); ++i){
			IConnectible *p( dynamic_cast<IConnectible*>(*i) );
			if(p) ConnectChild(p,*pOther);
		}
		return true;
	}else{
		return false;
	}
}



bool CSceneNodeBoxyDraggableContainer::DisconnectFrom(const IConnectible &theOther){
	const CSceneNodeBoxyDraggableContainer *pOther( dynamic_cast<const CSceneNodeBoxyDraggableContainer *>(&theOther) );
	if( !pOther ) return false;
	if(m_pConnectedOnLeft==pOther){
		m_pConnectedOnLeft = NULL;
	}else if(m_pConnectedOnRight==pOther){
		m_pConnectedOnRight = NULL;
	}else{
		return false;
	}
	for(int i=0; i<6; ++i){
		m_draggableSurfaces[i]->RemoveConstraintToDisable(pOther->m_pConstraint.get());
	}
	for(std::vector<ISceneNodeBoxy*>::iterator i( m_pContained.begin() ); i!=m_pContained.end(); ++i){
		IConnectible *p( dynamic_cast<IConnectible*>(*i) );
		if(p) DisconnectChild(p,*pOther);
	}
	return true;
}



void CSceneNodeBoxyDraggableContainer::ConnectChild(IConnectible *p){
	if(m_pConnectedOnLeft) ConnectChild(p,*m_pConnectedOnLeft);
	if(m_pConnectedOnRight) ConnectChild(p,*m_pConnectedOnRight);
}


void CSceneNodeBoxyDraggableContainer::DisconnectChild(IConnectible *p){
	if(m_pConnectedOnLeft) DisconnectChild(p,*m_pConnectedOnLeft);
	if(m_pConnectedOnRight) DisconnectChild(p,*m_pConnectedOnRight);
}




void CSceneNodeBoxyDraggableContainer::ConnectChild(IConnectible *pChild, const CSceneNodeBoxyDraggableContainer &target){
	for(std::vector<ISceneNodeBoxy*>::const_iterator i( target.m_pContained.begin() ); i!=target.m_pContained.end(); ++i){
		IConnectible *p( dynamic_cast<IConnectible*>(*i) );
		if(p) pChild->ConnectTo(*p);
	}
}



void CSceneNodeBoxyDraggableContainer::DisconnectChild(IConnectible *pChild, const CSceneNodeBoxyDraggableContainer &target){
	for(std::vector<ISceneNodeBoxy*>::const_iterator i( target.m_pContained.begin() ); i!=target.m_pContained.end(); ++i){
		IConnectible *p( dynamic_cast<IConnectible*>(*i) );
		if(p) pChild->DisconnectFrom(*p);
	}
}


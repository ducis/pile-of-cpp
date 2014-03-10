#include "CSceneNodeInteriorDef.h"
#include "CRegistryStack.h"
#include "CSceneNodePolygonModel.h"
#include "singleton.h"
#include "srl.h"
#include "CProgram.h"
#include "CTypedSceneNodeManager.h"
#include "CSceneNodeObjectSet.h"
#include "CSceneNodeInterfaceDef.h"
#include "IControlPlane.h"
#include "CConfigVarMod.h"

using namespace std;

CSceneNodeInteriorDef::CSceneNodeInteriorDef(const boost::property_tree::wptree &tr):
m_pPoly(NULL),
m_iAxis(2),
m_nBoards(2),
m_boardThickness(100),
m_offsetBegin(0),
m_offsetEnd(0),
m_polyName(tr.get<wstring>(L"PolyName"))
{
	wistringstream(tr.get_child(L"Verts").data())>>m_verts;
	GetSingleton<std::stack<std::vector<CSceneNodeInteriorDef *>>>()->top().push_back(this);
	GetSingleton<CTypedSceneNodeManager<CSceneNodeInteriorDef>>()->Register(this);
}


void CSceneNodeInteriorDef::CatchVerts()
{
	m_pPoly = GetSingleton<CRegistryStack<CSceneNodePolygonModel *>>()->Find(m_polyName);
	assert(m_pPoly);
	transform(
		m_verts.begin(),
		m_verts.end(),
		back_inserter(m_pPositions),
		bind1st(mem_fun(&CSceneNodePolygonModel::GetVertPositionConstPtr),m_pPoly)
		);
}


const std::pair<vector3,vector3> CSceneNodeInteriorDef::GetAABB() const{
	std::pair<vector3,vector3> ret(*m_pPositions[0],*m_pPositions[0]);
	for(std::vector<const vector3 *>::const_iterator itr = m_pPositions.begin()+1; itr!= m_pPositions.end(); ++itr){
		for( int i=0; i<3; ++i){
			if((**itr)[i]<ret.first[i]){
				ret.first[i] = (**itr)[i];
			}else if((**itr)[i]>ret.second[i]){
				ret.second[i] = (**itr)[i];
			}
		}
	}
	return ret;
}


void CSceneNodeInteriorDef::Render() const
{
	//vector3 bbMin(*m_pPositions[0]);
	//vector3 bbMax(bbMin);
	//for(std::vector<const vector3 *>::const_iterator itr = m_pPositions.begin(); itr!= m_pPositions.end(); ++itr){
	//	for( int i=0; i<3; ++i){
	//		if((**itr)[i]<bbMin[i]){
	//			bbMin[i] = (**itr)[i];
	//		}else if((**itr)[i]>bbMax[i]){
	//			bbMax[i] = (**itr)[i];
	//		}
	//	}
	//}
	std::pair<vector3,vector3> aabb(GetAABB());
	const vector3 &bbMin(aabb.first);
	const vector3 &bbMax(aabb.second);
	vector3 bbCenter(0.5*(bbMin+bbMax));
	vector3 bbDim(bbMax - bbMin);
	if(GetSingleton<CProgram>()->GetConfigVar<bool>(L"ShowInteriorDefBoundingBox")){
		glPushMatrix();
		glTranslatef(bbCenter.x,bbCenter.y,bbCenter.z);
		glScalef(bbDim.x,bbDim.y,bbDim.z);
		glPushAttrib(GL_ALL_ATTRIB_BITS);
		glDisable(GL_LIGHTING);
		glDepthRange(0,0);
		glutWireCube(1.0);
		glPopAttrib();
		glPopMatrix();
	}
	if(m_pSubNodes.empty()){
		glPushAttrib(GL_ALL_ATTRIB_BITS);
		//float c[4] = {1.0f,1.0f,1.0f,1.0f};
		//glMaterialfv(GL_FRONT_AND_BACK,GL_DIFFUSE,c);
		//glMaterialfv(GL_FRONT_AND_BACK,GL_AMBIENT,c);
		//glMaterialfv(GL_FRONT_AND_BACK,GL_EMISSION ,c);
		//glColor3f(1.0f,1.0f,1.0f);
		//glDisable(GL_TEXTURE_2D);
		//glDisable(GL_COLOR_MATERIAL);
		//glEnable(GL_LIGHTING);
		//glEnable(GL_LIGHT0);
		glEnable(GL_NORMALIZE);
		//glShadeModel(GL_FLAT);
		//glDisable(GL_CULL_FACE);
		//glLightModeli(GL_LIGHT_MODEL_TWO_SIDE,1);
		float step(bbDim[m_iAxis]/float(m_nBoards+1));
		vector3 boardCenter(bbCenter);
		boardCenter[m_iAxis] = bbMin[m_iAxis];
		vector3 boardDim(bbDim);
		boardDim[m_iAxis] = m_boardThickness;
		for(int i=0; i<m_nBoards;++i){
			boardCenter[m_iAxis] += step;
			glPushMatrix();
			glTranslatef(boardCenter.x,boardCenter.y,boardCenter.z);
			glScalef(boardDim.x,boardDim.y,boardDim.z);
			glutSolidCube(1.0);
			glPopMatrix();
		}
		glPopAttrib();
	}else{
		for_each(m_pSubNodes.begin(),m_pSubNodes.end(),mem_fun(&ISceneNode::Render));
	}
	//CAabb bb(m_pPoly->Get
}



void CSceneNodeInteriorDef::SetAxis(int iAxis){
	assert(iAxis>=0 && iAxis<3);
	m_iAxis = iAxis;
	RepositionSubNodes();
}


int CSceneNodeInteriorDef::GetAxis() const{
	return m_iAxis;
}


void CSceneNodeInteriorDef::AddSubNode(ISceneNode *pNode){
	m_pSubNodes.push_back(pNode);
	GetSingleton<CProgram>()->DiscardTransformBakingBatch();
	pNode->OnInsertion();
	std::pair<vector3,vector3> aabb(GetAABB());
	vector3 bbCenter( 0.5*(aabb.first+aabb.second) );
	GetSingleton<CProgram>()->BakeBatch(int(bbCenter.x),int(bbCenter.y),int(bbCenter.z),0);
}


void CSceneNodeInteriorDef::ClearSubNodes(){
	for(std::vector<ISceneNode *>::iterator itr = m_pSubNodes.begin(); itr!= m_pSubNodes.end(); ++itr){
		delete *itr;
	}
	m_pSubNodes.clear();
}


void CSceneNodeInteriorDef::RemoveNthSubNode(unsigned int n){
	assert(n<m_pSubNodes.size());
	if(n>=m_pSubNodes.size()) return;
	delete m_pSubNodes[n];
	m_pSubNodes.erase(m_pSubNodes.begin()+n);
	SetBoardNum(GetSubNodeNum());
}


void CSceneNodeInteriorDef::RepositionSubNodes( unsigned int directionMask ){
	if(m_pSubNodes.empty())
		return;
	std::pair<vector3,vector3> aabb(GetAABB());
	vector3 &bbMin(aabb.first);
	vector3 &bbMax(aabb.second);
	bbMin[m_iAxis]+=m_offsetBegin;
	bbMax[m_iAxis]-=m_offsetEnd;
	//vector3 bbCenter(0.5*(bbMin+bbMax));
	vector3 bbDim(bbMax - bbMin);
	float pAxis(bbMin[m_iAxis]);
	float step(bbDim[m_iAxis]/float(m_nBoards+1));

	int moveTo[6] = { int(bbMax.x), int(bbMax.y), int(bbMax.z), int(bbMin.x), int(bbMin.y), int(bbMin.z) };
	std::vector<CSceneNodeInterfaceDef*> interfaceDefs;
	CConfigVarMod<bool> wtMod(L"FixWallThickness",false);
	for( std::vector<ISceneNode *>::iterator itr = m_pSubNodes.begin(); itr!= m_pSubNodes.end(); ++itr )
	{
		CSceneNodeObjectSet *pObjSet = dynamic_cast<CSceneNodeObjectSet*>(*itr);
		assert(pObjSet);
		if(!pObjSet) continue;
		interfaceDefs.clear();
		pObjSet->GetChildrenByType<CSceneNodeInterfaceDef>( back_inserter(interfaceDefs) );
		assert( interfaceDefs.size() == 6 );
		
		pAxis += step;
		moveTo[m_iAxis] = int(pAxis + 0.5*m_boardThickness);
		moveTo[m_iAxis+3] = moveTo[m_iAxis] - m_boardThickness;
		for(std::vector<CSceneNodeInterfaceDef*>::iterator itr = interfaceDefs.begin(); itr!= interfaceDefs.end(); ++itr){
			if( directionMask & ( 1<<(*itr)->GetDirection() ) ){
				for_each( (*itr)->Begin(), (*itr)->End(), bind2nd( mem_fun1( &IControlPlane::MoveTo ), moveTo[(*itr)->GetDirection()] ) );
			}
		}
	}
}

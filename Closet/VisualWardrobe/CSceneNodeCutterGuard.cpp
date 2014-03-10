#include "CSceneNodeCutterGuard.h"
#include "CCutter.h"
#include "MyUtilities.h"

//CSceneNodeCutterGuard::CSceneNodeCutterGuard(const AABB &aabb,board_type *pBoardMinX, board_type *pBoardMaxX, board_type *pBoardMinY, board_type *pBoardMaxY)
CSceneNodeCutterGuard::CSceneNodeCutterGuard(const AABB &aabb, board_type *pBoardX, board_type *pBoardY)
:m_aabb(aabb),m_pCutter(NULL),m_pBoardX(pBoardX),m_pBoardY(pBoardY)
{
	m_pBoardX->SetBoardName(L"ÇÐ½Ç°å");
	m_pBoardY->SetBoardName(L"ÇÐ½Ç°å");
	//m_pBoards[0][0].reset(pBoardMinX);
	//m_pBoards[0][1].reset(pBoardMaxX);
	//m_pBoards[1][0].reset(pBoardMinY);
	//m_pBoards[1][1].reset(pBoardMaxY);
}




void CSceneNodeCutterGuard::Render() const{
	if(!m_pCutter)	return;
	m_pBoardX->Render();
	m_pBoardY->Render();
}


void CSceneNodeCutterGuard::SetAABB(const AABB &aabb){
	m_aabb = aabb;
	OnSpatialChange();
}


const AABB CSceneNodeCutterGuard::GetAABB() const{
	return m_aabb;
}


void CSceneNodeCutterGuard::UpdateTransform(const matrix44 &parentTransform){
	m_pBoardX->UpdateTransform(parentTransform);
	m_pBoardY->UpdateTransform(parentTransform);
	m_worldInvTransform = m_worldTransform = parentTransform;
	m_worldInvTransform.invert();
	OnSpatialChange();
}


void CSceneNodeCutterGuard::OnSpatialChange(){
	if(!m_pCutter)	return;
	const AABB cutterAABB( m_worldInvTransform*AABB(vector3(m_pCutter->Min(),0),vector3(m_pCutter->Max(),0)) );
	const AABB &aabb(m_aabb);
	//float tx( GetDim(m_pBoardX->GetAABB(), 0) );
	//float ty( GetDim(m_pBoardY->GetAABB(), 1) );
	float tx(18.0f);
	float ty(18.0f);
	AABB boardAABB[2] = { m_pBoardX->GetAABB() , m_pBoardY->GetAABB() };
	boardAABB[0].first.z = boardAABB[1].first.z = aabb.first.z;
	boardAABB[0].second.z = boardAABB[1].second.z = aabb.second.z;
	float t[2] = {tx,ty};
	bool positive[2] = {false,false};
	for(int iAxis(0); iAxis<2; ++iAxis){
		int jAxis((iAxis+1)%2);
		if(cutterAABB.second[iAxis]<aabb.second[iAxis]){
			positive[iAxis] = true;
			assert(cutterAABB.second[iAxis]>aabb.first[iAxis]);
			boardAABB[iAxis].first[iAxis] = cutterAABB.second[iAxis];
			boardAABB[iAxis].second[iAxis] = cutterAABB.second[iAxis] + t[iAxis];
			boardAABB[jAxis].first[iAxis] = aabb.first[iAxis];
			boardAABB[jAxis].second[iAxis] = cutterAABB.second[iAxis];
		}else if(cutterAABB.first[iAxis]>aabb.first[iAxis]){
			positive[iAxis] = false;
			assert(cutterAABB.first[iAxis]<aabb.second[iAxis]);
			boardAABB[iAxis].first[iAxis] = cutterAABB.first[iAxis] - t[iAxis];
			boardAABB[iAxis].second[iAxis] = cutterAABB.first[iAxis];
			boardAABB[jAxis].first[iAxis] = cutterAABB.first[iAxis];
			boardAABB[jAxis].second[iAxis] = aabb.second[iAxis];
		}else{
			assert(0);
		}
	}
	using namespace MyUtilities;
	ClampL(boardAABB[0].second.y,boardAABB[1].second.y);
	ClampR(boardAABB[0].first.y,boardAABB[1].first.y);
	m_pBoardX->SetAABB(boardAABB[0]);
	m_pBoardY->SetAABB(boardAABB[1]);
	vector3 xB(m_worldTransform.col[0].xyz());
	vector3 yB(m_worldTransform.col[1].xyz());
	if(!positive[0]) xB*=-1;
	if(!positive[1]) yB*=-1;
	//m_pCutter->SetGuard( positive[0], positive[1], aabb.first.z, aabb.second.z, vector2(t[0],t[1]), m_worldTransform*aabb );
	if(abs(xB.x)<abs(yB.x)){
		std::swap(xB,yB);
		std::swap(t[0],t[1]);
	}
	m_pCutter->SetGuard( 
		//positive[0], 
		xB.x>0,
		xB.y>0,
		aabb.first.z, aabb.second.z, vector2(t[0],t[1]), m_worldTransform*aabb );
}


void CSceneNodeCutterGuard::SetCutter(CCutter *pCutter){
	m_pCutter = pCutter;
	if(m_pCutter){
		//OnSpatialChange();
		m_pBoardX->Enable();
		m_pBoardY->Enable();
	}else{
		m_pBoardX->Disable();
		m_pBoardY->Disable();
	}
}

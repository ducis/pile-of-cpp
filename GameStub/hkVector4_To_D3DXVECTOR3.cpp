#include "DXUT.h"
#include <Common/Base/hkBase.h>
	
#include <D3DX9.h>
void hkVector4_to_D3DXVECTOR3(hkVector4 *pIn,D3DXVECTOR3 *pOut){
	hkQuadReal &q =GetQuad();
	pOut->x=q.x;
	pOut->y=q.y;
	pOut->z=q.z;
}
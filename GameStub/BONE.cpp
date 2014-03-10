#include "DXUT.h"
#include "index.h"
BONE	*BONE::cur=NULL;
LPD3DXMATRIXSTACK BONE::MatStack=NULL;
LPD3DXCONSTANTTABLE BONE::ShaderConstantTable=NULL;
float BONE::FinalMats[1024][16];
long BONE::MaxBoneID;
//BONE::Update(BONE *parent){
//}
void BONE::MakeReady(BONE *parent){
	D3DXQuaternionNormalize(&Rotation,&Rotation);
	D3DXMATRIX MatTranslation,MatRotation,MatScale;
	D3DXMatrixTranslation(&MatTranslation,Position.x,Position.y,Position.z);
	D3DXMatrixRotationQuaternion(&MatRotation,&Rotation);
	D3DXMatrixScaling(&MatScale,Scale.x,Scale.y,Scale.z);
	D3DXMATRIX myMat=MatScale*MatRotation*MatTranslation;
	D3DXMatrixInverse(&Inversion,NULL,&myMat);
	++cur;
	for(long i=0;i<nC;++i){
		cur->MakeReady(this);
	}
	if(parent){
		D3DXVec3TransformCoord(&Position,&Position,&parent->Inversion);
		//D3DXVec3TransformCoord((D3DXVECTOR3*)&Rotation,(D3DXVECTOR3*)&Rotation,pMat);
		D3DXQUATERNION PntRotInv;
		D3DXQuaternionInverse(&PntRotInv,&parent->Rotation);
		D3DXQuaternionMultiply(&Rotation,&Rotation,&PntRotInv);
		//D3DXVec3TransformCoord(&Scale,&Scale,pMat);
		Scale.x/=parent->Scale.x;
		Scale.y/=parent->Scale.y;
		Scale.z/=parent->Scale.z;
	}
}
void BONE::FillMatrices(){
	D3DXMATRIX MatTranslation,MatRotation,MatScale;
	D3DXMatrixTranslation(&MatTranslation,Position.x,Position.y,Position.z);
	D3DXMatrixRotationQuaternion(&MatRotation,&Rotation);
	D3DXMatrixScaling(&MatScale,Scale.x,Scale.y,Scale.z);
	MatStack->Push();
	*(MatStack->GetTop())=MatScale*MatRotation*MatTranslation*(*(MatStack->GetTop()));


	D3DXMATRIX Final=Inversion*(*MatStack->GetTop());
	//ShaderConstantTable->SetMatrix(DXUTGetD3D9Device(),ShaderConstantTable->GetConstantElement("BoneTrans",id),&Final);
	float *p=FinalMats[id];
	p[0]=Final._11;
	p[1]=Final._21;
	p[2]=Final._31;
	p[3]=Final._41;
	p[4]=Final._12;
	p[5]=Final._22;
	p[6]=Final._32;
	p[7]=Final._42;
	p[8]=Final._13;
	p[9]=Final._23;
	p[10]=Final._33;
	p[11]=Final._43;
	p[12]=Final._14;
	p[13]=Final._24;
	p[14]=Final._34;
	p[15]=Final._44;
	//MaxBoneID>?=id;
	if(id>MaxBoneID){
		MaxBoneID=id;
	}

	++cur;
	for(long i=0;i<nC;++i){
		cur->FillMatrices();
	}
	MatStack->Pop();
}

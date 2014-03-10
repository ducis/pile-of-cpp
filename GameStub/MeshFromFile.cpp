#include <DXUT.h>
#include <stdio.h>
#include <D3DX9Mesh.h>
#include "index.h"
void MeshFromFile(LPD3DXMESH *ppMesh,FILE *file/*,LPD3DXSKININFO * ppSkinInfo*/){
	*ppMesh=NULL;
	if(!file){
		return;
	}
	DWORD NumF,NumV,NumB,Options;
	fread(&NumF,sizeof(NumF),1,file);
	fread(&NumV,sizeof(NumV),1,file);
	fread(&NumB,sizeof(NumB),1,file);
	fread(&Options,sizeof(Options),1,file);
	Options|=D3DXMESH_MANAGED;
	long type;
	fread(&type,sizeof(type),1,file);
//	HRESULT result=-1l;
	long VertSize=0;
	if(type<0){
	}else if(type==0){
		D3DVERTEXELEMENT9 Vert[32]= 
		{
			{0,0,D3DDECLTYPE_FLOAT3,D3DDECLMETHOD_DEFAULT,D3DDECLUSAGE_POSITION, 0},
			{0,12,D3DDECLTYPE_FLOAT3,D3DDECLMETHOD_DEFAULT,D3DDECLUSAGE_NORMAL, 0},
			{0,24,D3DDECLTYPE_FLOAT2,D3DDECLMETHOD_DEFAULT,D3DDECLUSAGE_TEXCOORD, 0},
			{0,32,D3DDECLTYPE_FLOAT4,D3DDECLMETHOD_DEFAULT,D3DDECLUSAGE_BLENDINDICES, 0},
			{0,48,D3DDECLTYPE_FLOAT4,D3DDECLMETHOD_DEFAULT,D3DDECLUSAGE_BLENDWEIGHT, 0},
			D3DDECL_END()
		};
		if(NumB==0){
			D3DVERTEXELEMENT9 t=D3DDECL_END();
			Vert[3]=t;
			VertSize=32;
		}else if(NumB==1){
			Vert[3].Type=Vert[4].Type=D3DDECLTYPE_FLOAT1;
			Vert[4].Offset=32+4*1;
			VertSize=32+8*1;
		}else if(NumB==2){
			Vert[3].Type=Vert[4].Type=D3DDECLTYPE_FLOAT2;
			Vert[4].Offset=32+4*2;
			VertSize=32+8*2;
		}else if(NumB==3){
			Vert[3].Type=Vert[4].Type=D3DDECLTYPE_FLOAT3;
			Vert[4].Offset=32+4*3;
			VertSize=32+8*3;
		}else if(NumB==4){
			Vert[3].Type=Vert[4].Type=D3DDECLTYPE_FLOAT4;
			Vert[4].Offset=32+4*4;
			VertSize=32+8*4;
		}
		//VertSize=8*sizeof(float);
		D3DXCreateMesh(NumF,NumV,Options,Vert,DXUTGetD3D9Device(),ppMesh);
	}
	if(!(*ppMesh)){
		return;
	}
	float* pData;
	(*ppMesh)->LockIndexBuffer(0,(void **)&pData);
	if(fread(pData,3*sizeof(WORD),NumF,file)!=NumF){
		MessageBoxA(NULL,"Corrupted mesh file:F data","Error",0);
		exit(0);
	}
	(*ppMesh)->UnlockIndexBuffer();
	(*ppMesh)->LockVertexBuffer(0,(void **)&pData);
	{
		DWORD i;
		char *p;
		for(i=0,p=(char*)pData;i<NumV;++i,p+=VertSize){
			if(!fread(p,32,1,file)){
				MessageBoxA(NULL,"Corrupted mesh file:V data","Error",0);
				exit(0);
			}
		}
		if(NumB){
			for(i=0,p=(char*)pData+32;i<NumV;++i,p+=VertSize){
				if(!fread(p,VertSize-32,1,file)){
					MessageBoxA(NULL,"Corrupted mesh file:B data","Error",0);
					exit(0);
				}
			}
		}
	}
	(*ppMesh)->UnlockVertexBuffer();
/*	if(ppSkinInfo){
		D3DVERTEXELEMENT9 decl[128];
		(*ppMesh)->GetDeclaration(decl);
		D3DXCreateSkinInfo((*ppMesh)->GetNumVertices(),decl,
	}*/
}
void MeshFromFile(LPD3DXMESH *ppMesh,char *path/*,LPD3DXSKININFO * ppSkinInfo*/){
	FILE *f=fopen(path,"rb");
	MeshFromFile(ppMesh,f/*,ppSkinInfo*/);
	fclose(f);
}

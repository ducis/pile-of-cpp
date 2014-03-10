#include "CMyMesh.h"
#include "error.h"
CMyMesh::CMyMesh(){
	ZeroMemory(this,sizeof(CMyMesh));
}
CMyMesh::CMyMesh(FILE *file){
	CMyMesh();
	if(!file){
		exit(ERR_MYMESH_NULL_FILE_HANDLE);
	}
	fread(&nV,sizeof(nV),1,file);
	fread(&nI,sizeof(nI),1,file);
	nI*=3;
	fread(&Options,sizeof(Options),1,file);
	//Options|=D3DXMESH_MANAGED;
	fread(&Type,sizeof(Type),1,file);
	HRESULT result=-1l;
//	long VertSize=0;
	{
		nIB=1;
		DXUTGetD3D9Device()->CreateIndexBuffer(nI*sizeof(WORD),D3DUSAGE_WRITEONLY,D3DFMT_INDEX16,D3DPOOL_MANAGED,iBuf,NULL);
		LPVOID pData;
		iBuf[0]->Lock(0,nI*sizeof(WORD),&pData,D3DLOCK_NOSYSLOCK);
		if(fread(pData,sizeof(WORD),nI,file)!=nI){
			MessageBoxA(NULL,"Corrupted mesh file\:I data","Error",0);
			exit(ERR_MYMESH_FILE_CORRUPTED_I);
		}
		iBuf[0]->Unlock();
	}
	++nVB;
	if(Type<0){
	}else if(Type==0){
		D3DVERTEXELEMENT9 Vert[]= 
		{
			{0,0,D3DDECLTYPE_FLOAT3,D3DDECLMETHOD_DEFAULT,D3DDECLUSAGE_POSITION, 0},
			{0,12,D3DDECLTYPE_FLOAT3,D3DDECLMETHOD_DEFAULT,D3DDECLUSAGE_NORMAL, 0},
			{0,24,D3DDECLTYPE_FLOAT2,D3DDECLMETHOD_DEFAULT,D3DDECLUSAGE_TEXCOORD, 0},
			D3DDECL_END()
		};
		DXUTGetD3D9Device()->CreateVertexDeclaration(Vert,pVertDecl);
		//long VertSize=8*sizeof(float);
		vSize[0]=32;
		//(*ppMesh)->LockVertexBuffer(0,&pData);
		LPVOID pData;
		vBuf[0]->Lock(0,vSize[0]*nV,&pData,D3DLOCK_NOSYSLOCK)
		if(fread(pData,vSize[0],nV,file)!=nV){
			MessageBoxA(NULL,"Corrupted mesh file\:V data","Error",0);
			exit(ERR_MYMESH_FILE_CORRUPTED_V);
		}
		vBuf[0]->Unlock();
	}
	long MaxBoneCount=0;
	fread(MaxBoneCount,sizeof(long),1,file);
	if(BoneCount){
		D3DVERTEXELEMENT9 Vert[1024];
		for(i=0;i<MaxBoneCount;++i){

		}
		Vert[MaxBoneCount]=D3DDECL_END();
	}
}
CMyMesh::~CMyMesh(){
	for(long i=0;i<nVB;++i){
		vBuf[i]->Release();
	}
	for(long i=0;i<nIB;++i){
		iBuf[i]->Release();
	}
	ZeroMemory(this,sizeof(CMyMesh));
}
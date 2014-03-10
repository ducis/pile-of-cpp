#include <DXUT.h>
#include <stdio.h>
#include "index.h"
void SkeletonFromFile(BONE **ppBoneArray,long *pNumBones,char *path){
	FILE *file=fopen(path,"rb");
	fseek(file,0,SEEK_END);
	long c=(ftell(file)+1)/(sizeof(BONE)-sizeof(D3DXMATRIX));
	fseek(file,0,SEEK_SET);
	(*ppBoneArray)=new BONE[c];
	for(long i=0;i<c;++i){
		fread((*ppBoneArray)+i,sizeof(BONE)-sizeof(D3DXMATRIX),1,file);
	}
	//fread(*ppBoneArray,sizeof(BONE),c,file);
	fclose(file);
	*pNumBones=c;
	BONE::cur=*ppBoneArray;
	BONE::cur->MakeReady(NULL);
}

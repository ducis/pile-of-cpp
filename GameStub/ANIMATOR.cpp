#include "DXUT.h"
#include "index.h"
#define ANIMATOR_KEYS_LOOP 1
/*struct ANIMATOR_KEYS{
	void *pKeys,*pOut;
	DWORD *pTimes;
	void (*pInterpolater)(void *,void *,void *,float);
	WORD flag,stride,nKeys,cur;
	DWORD length;
	void Update(DWORD t);
	void QuickSet(DWORD t);
};*/

void ANIMATOR_KEYS::Update(DWORD t){
	if(t>=length){
		if(flag&ANIMATOR_KEYS_LOOP){
			t%=length;
			cur=0;
		}else{
			t=length;
		}
	}
	while(t>=pTimes[cur]){
		++cur;
		if(cur==nKeys){
			cur=0;
			break;
		}
	}
	if(cur){
		long prev=cur-1;
		if(t==pTimes[prev]){
			memcpy(pOut,(char*)pKeys+prev*stride,stride);
		}else{
			(*pInterpolater)(pOut,(char*)pKeys+prev*stride,(char*)pKeys+cur*stride,((float)(t-pTimes[prev]))/((float)(pTimes[cur]-pTimes[prev])));
		}
	}else{
		long prev=nKeys-1;
		if(t==pTimes[prev]){
			memcpy(pOut,(char*)pKeys+prev*stride,stride);
		}else{
			(*pInterpolater)(pOut,(char*)pKeys+prev*stride,(char*)pKeys+cur*stride,((float)(t-pTimes[prev]))/((float)(pTimes[cur]+length-pTimes[prev])));
		}
	}
}
void ANIMATOR_QUICKSET(DWORD t){
}

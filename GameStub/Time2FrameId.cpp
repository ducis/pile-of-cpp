#include "DXUT.h"
long Time2FrameId(float time,float *timeseq,long num){
	long lo=0,hi=num-1;
	while(lo<hi){
		if(time<=timeseq[lo]){
			hi=lo;
		}else if(time>=timeseq[hi]){
			lo=hi;
		}else if(lo+1==hi){
			hi=lo;
		}else{
			long m=(lo+hi)>>1;
			if(time<timeseq[m]){
				hi=m;
			}else{
				lo=m;
			}
		}
	}
	return lo;
}

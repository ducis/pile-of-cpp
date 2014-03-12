#ifdef _DEBUG
//#include <afxext.h>
//#define new DEBUG_NEW
#endif

#define START_VAL_NUM 16

// We have to define operator= for arr[2],arr[3],arr[4] in order 
// to enable template use.

void assign(VRSFVec2f& to,float from[]){
	to[0]=from[0];
	to[1]=from[1];
}

void assign(VRSFVec3f& to,float from[]){
	to[0]=from[0];
	to[1]=from[1];
	to[2]=from[2];
}

void assign(VRSFRotation& to,float from[]){
	to[0]=from[0];
	to[1]=from[1];
	to[2]=from[2];
	to[3]=from[3];
}

int is_pow2(int n){
	if(n<=0) return 0;
	int t=n,acc=0;
	while(t>0) {
		if(t & 1) acc++;
		t>>=1;
	}
	return acc==1;
};

template <class M,class S>
void add_scalar(M m,S s){
	if(m->values==NULL && m->numValues<=0) {
		m->values=new S[START_VAL_NUM];
		m->numValues=0;
	} else if(m->numValues>=START_VAL_NUM && is_pow2(m->numValues)){
		S* tmp=m->values;
		m->values=new S[m->numValues*2];
		for(unsigned int i=0;i<m->numValues;i++) m->values[i]=tmp[i];
		delete tmp;
	}
	m->values[m->numValues]=s;
	m->numValues++;
};

template <class M,class S>
void add_array(M m,S s){
	if(m->values==NULL && m->numValues<=0) {
		m->values=new S[START_VAL_NUM];
		m->numValues=0;
	} else if(m->numValues>=START_VAL_NUM && is_pow2(m->numValues)){
		S* tmp=m->values;
		m->values=new S[m->numValues*2];
		for(unsigned int i=0;i<m->numValues;i++) assign(m->values[i],tmp[i]);
		delete[] tmp;
	}
	assign(m->values[m->numValues],s);
	m->numValues++;
};
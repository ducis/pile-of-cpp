#ifndef GNAMEDVARMANAGER_H
#define GNAMEDVARMANAGER_H
template <typename T,T Invalid,void (*Init)()=NULL,char **InitNameList=NULL,T* InitVarList=NULL> class GNamedVarManager{
private:
	struct ITEM{
		T var;
		ITEM *children[4];
		ITEM():var(Invalid){
			memset(children,0,sizeof(children));
		}
	}m_root;
public:
	T GetVar(const char *name);
	T *GetPtr(const char *name);
	T *GetPtrReady(const char *name);
	void Insert(const char *name,T var);
	GNamedVarManager(){
		if(Init){
			Init();
		}
		if(InitNameList&&InitVarList){
			char **name=InitNameList;
			T *var=InitVarList;
			while(*name){
				Insert(*name,*var);
				++name;
				++var;
			}
		}
	}
};

template <typename T,T Invalid,void (*Init)(),char **InitNameList,T* InitVarList>
T GNamedVarManager<T,Invalid,Init,InitNameList,InitVarList>::GetVar(const char* name){
	T* p=GetPtr(name);
	if(p)
		return *p;
	else
		return Invalid;
}
template <typename T,T Invalid,void (*Init)(),char **InitNameList,T* InitVarList>
void GNamedVarManager< T, Invalid,Init,InitNameList,InitVarList>::Insert(const char *name,T var){
	*GetPtrReady(name)=var;
}
template <typename T,T Invalid,void (*Init)(),char **InitNameList,T* InitVarList>
T *GNamedVarManager< T, Invalid,Init,InitNameList,InitVarList>::GetPtr(const char *name){
	ITEM *p=&m_root;
	while(*name){
		for(long i=0;i<8;i+=2){
			p=p->children[((*name)>>i)&3];
			if(!p){
				return NULL;
			}
		}
		++name;
	}
	return &p->var;
}
template <typename T,T Invalid,void (*Init)(),char **InitNameList,T* InitVarList>
T *GNamedVarManager< T, Invalid,Init,InitNameList,InitVarList>::GetPtrReady(const char *name){
	ITEM *p=&m_root;
	while(*name){
		for(long i=0;i<8;i+=2){
			long next=((*name)>>i)&3;
			if(!p->children[next])
				p->children[next]=new ITEM;
			p=p->children[next];
		}
		++name;
	}
	return &p->var;
}
#endif
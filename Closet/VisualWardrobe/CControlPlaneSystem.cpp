#include "CControlPlaneSystem.h"
#include "CAnnotation.h"
#include "3d.h"
#include <cassert>
#include <algorithm>
#include "singleton.h"
#include "CProgram.h"
using namespace std;
void CControlPlaneSystem::Insert(IControlPlane *p){
	assert(p);
	assert( !Exists(p) );
	m_controlPlanes.insert(p);
}


void CControlPlaneSystem::Erase(IControlPlane *p){
	assert(p);
	assert( Exists(p) );
	class HasPlane{
	public:
		bool operator()(const PAIR &p){
			return (p.first == m_pCP) || (p.second == m_pCP);
		}
		bool operator()(ANNOTATION_PTR_CONTAINER::const_reference a){
			return (*this)(*a.first);
		}
		HasPlane(IControlPlane *p):m_pCP(p){}
	private:
		IControlPlane *m_pCP;
	};
	for(int iBase = 0; iBase<2; ++iBase){
		for(ANNOTATION_PTR_CONTAINER::iterator itr = m_annotations[iBase].begin(); itr!= m_annotations[iBase].end(); ){
			if( HasPlane(p)(*itr) ){
				itr = m_annotations[iBase].erase(itr);
			}else{
				++itr;
			}
		}
	}
	m_annotatedPairs.erase( remove_if(m_annotatedPairs.begin(), m_annotatedPairs.end(), HasPlane(p)), m_annotatedPairs.end() );
	m_controlPlanes.erase(p);
	GetSingleton<CProgram>()->ClearPicks();
	GetSingleton<CProgram>()->ClearTheOtherCP();
}


void CControlPlaneSystem::AddAnnotation(IControlPlane *a,IControlPlane *b){
	assert( Exists(a) && Exists(b) );
	assert( a != b );
	pair<PAIR_CONTAINER::iterator,bool> ret = m_annotatedPairs.insert(MakePair(a,b));
	if(ret.second){
		float orthoCoord[2] = {0,0};
		for(int iBase=0; iBase<2; ++iBase){
			orthoCoord[iBase] = min(
				ret.first->first->CastRange(m_orthoBase[iBase]).first,
				ret.first->second->CastRange(m_orthoBase[iBase]).first );
		}
		for(int iBase=0; iBase<2; ++iBase){
			m_annotations[iBase].insert(make_pair(&*ret.first,new CAnnotation(*ret.first,orthoCoord[iBase],orthoCoord[(iBase+1)%2])));
		}
	}
}


void CControlPlaneSystem::RemoveAnnotation(IControlPlane *a, IControlPlane *b){
	assert( Exists(a) && Exists(b) );
	PAIR_CONTAINER::iterator itrPair = m_annotatedPairs.find(MakePair(a,b));
	assert(itrPair != m_annotatedPairs.end());
	for(int iBase=0; iBase<2; ++iBase){
		m_annotations[iBase].erase(&*itrPair);
	}
}


bool CControlPlaneSystem::Exists(IControlPlane *p){
	return m_controlPlanes.find(p)!=m_controlPlanes.end();
}


CControlPlaneSystem::PAIR CControlPlaneSystem::MakePair(IControlPlane *a,IControlPlane *b){
	return (a<b)?PAIR(a,b):PAIR(b,a);
}


void CControlPlaneSystem::RenderAnnotations(){
	int iBase = 0;
	{
		matrix44 view;//,proj;
		glGetFloatv(GL_MODELVIEW_MATRIX,(GLfloat*)&view);
		//glGetFloatv(GL_PROJECTION_MATRIX,(GLfloat*)&proj);
		//matrix44 viewProj = proj*view;
		vector4 dirCS = view*vector4(m_dir,0.0f);
		if(vector2(dirCS.x,dirCS.y).lengthSqr()<0.01f){
			return;
		}
		float orthoBaseCSLenSq[2];
		for(int i=0;i<2;++i){
			vector4 cs = view*vector4(m_orthoBase[i],0.0f);
			orthoBaseCSLenSq[i] = vector2(cs.x,cs.y).lengthSqr();
		}
		if(orthoBaseCSLenSq[1]>orthoBaseCSLenSq[0]){
			iBase = 1;
		}
	}
	//if(iBase)
	//	return;
	class renderAnnotation:public std::unary_function<CAnnotation *,void>{
	private:
		const vector3 &m_mainVec, &m_uBase, &m_vBase;
		const float m_uOffset;
	public:
		renderAnnotation(const vector3 &mainVec,const vector3 &uBase,const vector3 &vBase)
			:m_mainVec(mainVec),m_uBase(uBase),m_vBase(vBase),m_uOffset(GetSingleton<CProgram>()->GetConfigVar<float>(L"AnnotationOffset")){}
		void operator()(const pair<PAIR *,class CAnnotation *> &pr){
			pr.second->Render(m_mainVec, m_uBase, m_vBase, m_uOffset);
		}
		void operator()(CAnnotation *p){
			p->Render(m_mainVec, m_uBase, m_vBase, m_uOffset);
		}
	};
	//std::for_each(m_annotations[iBase].begin(),m_annotations[iBase].end(),renderAnnotation(m_dir,m_orthoBase[iBase],m_orthoBase[(iBase+1)%2]));
	static std::vector<CAnnotation *> seq;
	seq.clear();
	for(ANNOTATION_PTR_CONTAINER::const_iterator itr(m_annotations[iBase].begin()); itr!=m_annotations[iBase].end(); ++itr){
		itr->second->Update(m_dir);
		seq.push_back(itr->second);
	}
	//sort(seq.begin(),seq.end(),&NAnnotation::Shorter);
	//stable_sort(seq.begin(),seq.end(),&NAnnotation::SeparateAndLess);
	class Less:public std::binary_function<const CAnnotation *,const CAnnotation *,bool>{
	public:
		bool operator()(const CAnnotation* lhs,const CAnnotation* rhs){
			//return lhs->GetDRange().second<rhs->GetDRange().second;
			return lhs->GetLength()<rhs->GetLength();
		}
	};
	stable_sort(seq.begin(),seq.end(),Less());
	const float spacing = GetSingleton<CProgram>()->GetConfigVar<float>(L"AnnotationSpacing");
	for(std::vector<CAnnotation*>::iterator i(seq.begin()); i!=seq.end(); ++i){
		(*i)->ResetUPos();
		for(std::vector<CAnnotation*>::const_iterator j(seq.begin()); j!=i; ++j){
			if((*i)->GetDRange().first>=(*j)->GetDRange().second)	continue;
			if((*i)->GetDRange().second<=(*j)->GetDRange().first)	continue;
			if(abs((*i)->GetUPos()-(*i)->GetUPos())<spacing){
				(*i)->SetUPos((*j)->GetUPos()-spacing);
			}
		}
	}
	for_each(seq.begin(),seq.end(),renderAnnotation(m_dir,m_orthoBase[iBase],m_orthoBase[(iBase+1)%2]));
}

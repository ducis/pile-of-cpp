#pragma once
#include "ISceneNode.h"
#include "ptree.h"
#include <vector>

class CSceneNodeObjectSet:public ISceneNode{
public:
	//将场景中间的多个节点结合成一个节点的类,没有特殊功能
	CSceneNodeObjectSet(const boost::property_tree::wptree &tr);
	void Render() const;
	void OnInsertion();
	template<class Required, class OutputIterator> 
	void GetChildrenByType(OutputIterator out) const{
		for(std::vector<ISceneNode *>::const_iterator itr = m_children.begin(); itr != m_children.end(); ++itr){
			Required *p = dynamic_cast<Required *>(*itr);
			if(p){
				*out++ = p;
			}
		}
	}
	template<class Required, class OutputIterator>
	void ClaimChildrenByType(OutputIterator out){
		for(std::vector<ISceneNode *>::iterator itr = m_children.begin(); itr != m_children.end(); ++itr){
			Required *p = dynamic_cast<Required *>(*itr);
			if(p){
				*out++ = p;
				if(itr+1 == m_children.end()){
					m_children.pop_back();
					break;
				}else{
					*itr = m_children.back();
					m_children.pop_back();
				}
			}
		}
	}
	~CSceneNodeObjectSet();
	virtual void EnlargeAABB(AABB &aabb)const;
	virtual void UpdateTransform(const matrix44 &parentTransform);
private:
	std::vector<ISceneNode *> m_children;
};

#pragma once

#include "mtxlib.h"
#include "CGenericComponent.h"
#include "CSceneNodeBoxyDecorator.h"
#include <boost/property_tree/ptree_fwd.hpp>
#include "IAnnotated.h"

class CSceneNodeBoxyGenericComponent: public CSceneNodeBoxyDecorator, public IAnnotated
{//����е�һ�㲿�����ڳ����еı�ʾ����
public:
	virtual void ShowAnnotation() const;
	//virtual void Render() const;
	CSceneNodeBoxyGenericComponent(const boost::property_tree::wptree &);
	~CSceneNodeBoxyGenericComponent();
	virtual void UpdateTransform(const matrix44 &parentTransform);
private:
	CGenericComponent m_component;
	matrix44 m_parentTransform;
};

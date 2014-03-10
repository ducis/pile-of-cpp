#pragma once

#include "IMaterial.h"
#include "CIdentifier.h"
#include <boost/property_tree/ptree_fwd.hpp> 

class CSimpleMaterial: public IMaterial{
public:
	//最简单的材质,就是一张贴图,对应Materials文件夹下xml文件中的SimpleMaterial结点
	virtual void Apply() const;//设定OpenGL状态准备渲染
	virtual const CIdentifier &GetIdentifier() const;//返回标识符
	CSimpleMaterial(boost::property_tree::wptree &tr);//通过xml文件中的<SimpleMaterial></SimpleMaterial>节点进行创建
private:
	class CTexture *m_pTex;
	CIdentifier m_identifier;
};

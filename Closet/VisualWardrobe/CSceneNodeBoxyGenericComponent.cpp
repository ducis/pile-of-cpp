#include "CSceneNodeBoxyGenericComponent.h"
#include "ptree.h"
#include "singleton.h"
#include "CProgram.h"
#include "ISceneNodeFactory.h"
#include "CSceneNodeFactoryRegistry.h"
#include <fstream>
#include "CSceneNodeBoxyScalar.h"
#include "OpenGLUtil.h"
using namespace std;
using namespace boost;
using namespace boost::property_tree;

namespace CSceneNodeBoxyComponent_impl{
	ISceneNodeBoxy *GetDecorated(const wptree &tr){
		wptree ftr;
		read_xml(wifstream(tr.data().c_str()),ftr);
		return new CSceneNodeBoxyScalar( GetSingleton<CSceneNodeFactoryRegistry>()->GetFactory(ftr.begin()->first)->Create(ftr.begin()->second) , UnitAABB() );
	}
}
using namespace CSceneNodeBoxyComponent_impl;


//void CSceneNodeBoxyGenericComponent::Render() const{
//	CSceneNodeBoxyDecorator::Render();
//}

void CSceneNodeBoxyGenericComponent::ShowAnnotation() const{
	glPushAttrib(GL_CURRENT_BIT);
	glPushMatrix();
	glMultMatrixf(&m_parentTransform.col[0][0]);
	const vector3 center(GetCenter(GetAABB()));
	glColor3f(0,0.5,0.5);
	glRasterPos3fv(&center.x);
	OpenGLUtil::DrawText(m_component.GetName());
	glPopMatrix();
	glPopAttrib();
}


CSceneNodeBoxyGenericComponent::CSceneNodeBoxyGenericComponent(const boost::property_tree::wptree &tr):
CSceneNodeBoxyDecorator(GetDecorated(tr.get_child(L"Visual"))),
m_component(tr.get_child(L"Data"))
{
	GetSingleton<CProgram>()->RegisterAnnotated(this);
}

CSceneNodeBoxyGenericComponent::~CSceneNodeBoxyGenericComponent(){
	GetSingleton<CProgram>()->UnregisterAnnotated(this);
}

void CSceneNodeBoxyGenericComponent::UpdateTransform(const matrix44 &parentTransform){
	m_parentTransform = parentTransform;
	CSceneNodeBoxyDecorator::UpdateTransform(parentTransform);
}

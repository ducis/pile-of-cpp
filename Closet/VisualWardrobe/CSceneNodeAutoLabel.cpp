#include "CSceneNodeAutoLabel.h"
#include "3d.h"
#include <algorithm>
#include <functional>
#include "CProgram.h"
#include "singleton.h"
using namespace std;

CSceneNodeAutoLabel::COUNTERS CSceneNodeAutoLabel::s_counters;
const unsigned g_numDefs = 2;
const pair<wstring,wstring> g_defs[g_numDefs]={
	make_pair(wstring(L"$GT"),wstring(L"GuiTi")),
	make_pair(wstring(L"$GB"),wstring(L"GeBan"))
};
CSceneNodeAutoLabel::DEFS CSceneNodeAutoLabel::s_defs(g_defs,g_defs+g_numDefs);

void CSceneNodeAutoLabel::RenderSymbol(){
	glPushAttrib(GL_ALL_ATTRIB_BITS);
	glDisable(GL_DEPTH_TEST);
	glRasterPos3fv(&m_pos.x);
	for_each(m_labelStr.begin(),m_labelStr.end(),bind1st(ptr_fun(&glutBitmapCharacter),GLUT_BITMAP_HELVETICA_12));
	glPopAttrib();
}


void CSceneNodeAutoLabel::OnInsertion(){
	GetSingleton<CProgram>()->RequestTransformBaking(this);
	GetSingleton<CProgram>()->RegisterFloatingSymbol(this);
}


void CSceneNodeAutoLabel::BakeTransform(int x_mm,int y_mm,int z_mm,int rotZ_90deg){
	m_pos.x+=x_mm;
	m_pos.y+=y_mm;
	m_pos.z+=z_mm;
	//m_labelStr.reserve(32);
	m_labelStr.assign(m_labelCategory.begin(),m_labelCategory.end());
	ostringstream os;
	os<<' '<<(++s_counters.insert(make_pair(m_labelCategory,0)).first->second);
	m_labelStr += os.str();
}


CSceneNodeAutoLabel::CSceneNodeAutoLabel(const boost::property_tree::wptree &tr){
	const wstring &s = tr.data();
	wistringstream(s)>>m_labelCategory>>m_pos;
	DEFS::iterator iDef = s_defs.find(m_labelCategory);
	if(iDef!=s_defs.end()){
		m_labelCategory = iDef->second;
	}
}
#include "CProgram.h"
#include "ICommand.h"
#include "windowsnominmax.h"
#include "3d.h"
//#include <gl/glut.h>
#include <string>
#include <cassert>
#include <sstream>
#include "IScene.h"
#include "CControlPlaneSystem.h"
#include "ITransformBaked.h"
#include "IFloatingSymbol.h"
#include "CTexture.h"
#include "singleton.impl.h"
#include "CTypedSceneNodeManager.h"
#include "CSceneNodeInteriorDef.h"
#include "IDraggable.h"
#include "CSceneNodeBoxyDraggableContainer.h"
#include "IWardrobeCell.h"
#include "IAnnotated.h"
#include "IRemovable.h"
#include "CSceneNodeBinaryWardrobeTreeNode.h"
#include "CAxialAABBAnnotation.h"
#include "CAnnotationStyle.h"
#include "CSimpleMaterial.h"
#include "CDxfHeader.h"
#include "CSceneNodeBoard.h"
#include "CNullDrillingRule.h"
#include "CClassicDrillingRule.h"
#include "CHoleStyle.h"
#include <io.h>
#include <locale>
#include <boost/archive/detail/utf8_codecvt_facet.hpp>
#include "StupidlyReadXmlToCreateWPTree.h"
#include "IConnectible.h"
#include "CTargetCamera.h"
#include "CSceneNodeBoxyGenericComponentVariant.h"

namespace CProgram_impl{}

using namespace CProgram_impl;

template CProgram *GetSingleton();
using namespace std;

CProgram::CProgram():
m_trieCommands(0,NULL),
m_hDC(NULL),
m_pScene(new CScene),
m_cursorHS(-100.0f,-100.0f),
m_selectedCP(-1),
m_pTheOtherCP(NULL),
m_pCurrentlyPicked(NULL),
m_pCurrentlyDragged(NULL),
m_pCurrentWardrobeCell(NULL),
m_dragDelta(0,0),
m_renderMode(SHADEDMODE),
m_pCurrentMaterial(NULL),
m_pCurrentTablesSecDxfHeader(NULL)
{
	{
		locale loc( "chs" , locale::ctype );
		//loc.combine<num_put<wchar_t> >( locale("C") );
		locale::global(loc);
		//locale::global(locale( "C" , locale::numeric ));
//		setlocale(LC_ALL,"chs");
		//locale::global.combine<codecvt
	}
	m_axisAlignedCPSys[0] = new CControlPlaneSystem(vector3(1,0,0),vector3(0,1,0),vector3(0,0,1));
	m_axisAlignedCPSys[1] = new CControlPlaneSystem(vector3(0,1,0),vector3(0,0,1),vector3(1,0,0));
	m_axisAlignedCPSys[2] = new CControlPlaneSystem(vector3(0,0,1),vector3(1,0,0),vector3(0,1,0));
#define CVAR( a , b ) m_config.push_back(make_pair(a,b))
	CVAR(L"ShowRoom",L"true");
	CVAR(L"ShowAnnotations",L"true");
	CVAR(L"FixWallThickness",L"true");
	CVAR(L"ShowInteriorDefBoundingBox",L"true");
	CVAR(L"AnnotationOffset",L"-50");
	CVAR(L"AnnotationSpacing",L"50");
	CVAR(L"SpecularEnabled",L"true");
	CVAR(L"DragSpeedModifier",L"1.0");
	CVAR(L"DoorStyle",L"2");
	CVAR(L"ShowBoardNames",L"true");
	CVAR(L"MinCellSize",L"10");
#undef CVAR
	m_propNodeMinCellSize = m_config.find(L"MinCellSize");
	m_minCellSize = m_propNodeMinCellSize->second.get_value<float>();
	RegisterPickable(2,&m_viewPanner);

	LoadMaterials();
	LoadDxfHeaders();
	LoadHoleStyles();
	LoadManufactureProperties();
	LoadGenerators();
	LoadGenericComponentSchemes();
	LoadGenericComponentVariantTables();
}

int CProgram::AcceptCommand(const wstring &cmd, const wstring &arg, wostream &out){
	ICommand *pCommand = m_trieCommands.retrieve(cmd);
	//assert(pCommand);
	if(!pCommand){
		out<<L"No such command.\n";
		return -1;
	}
	int r = pCommand->Invoke(this, wistringstream(arg), out);
	if(r)
		return r;
	if(m_hDC)	Redraw();
	return 0;
}

void CProgram::InsertCommand(const std::wstring &cmd,ICommand *pCommand){
	m_trieCommands.insert(cmd,pCommand);
}

void CProgram::Redraw(){/*
	int w = GetDeviceCaps(m_hDC,HORZRES);
	int h = GetDeviceCaps(m_hDC,VERTRES);
	glViewport(0,0,w,h);*/
	if(WIREFRAMEMODE == m_renderMode){
		glClearColor(1,1,1,1);
	}else{
		glClearColor(0,0,0,1);
	}
	Clear();
	glEnable(GL_NORMALIZE);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	glEnable(GL_LIGHTING);
	glPushAttrib(GL_ALL_ATTRIB_BITS);
	if(GetConfigVar<bool>(L"SpecularEnabled")){
		float c[4] = {1.0,1.0,1.0,1.0};
		glMaterialfv(GL_FRONT_AND_BACK , GL_SPECULAR , (GLfloat*)c);
		glMaterialf(GL_FRONT_AND_BACK , GL_SHININESS , 10.0f);
		glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER,1);
	}
	{
		float c[4] = {0.5f,0.5f,0.5f,1.0f};
		glLightfv(GL_LIGHT1,GL_DIFFUSE,c);
		glLightfv(GL_LIGHT1,GL_SPECULAR,c);
	}
	if(WIREFRAMEMODE == m_renderMode){
		glColor3f(0,0,0);
		glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
		glDisable(GL_LIGHTING);
	}
	m_pScene->Render();
	glPopAttrib();
	RenderCPSelection();
	RenderAnnotations();
	RenderFloatingSymbols();
	RenderCursor();
	Present();
}
void CProgram::Clear(){
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
}
void CProgram::Present(){
	SwapBuffers(m_hDC);
}
int CProgram::BindDC(HDC hDC){

	PIXELFORMATDESCRIPTOR pfd;
	memset(&pfd,0,sizeof(pfd));
	pfd.nSize = sizeof(pfd);
	pfd.nVersion = 1;
	pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_GENERIC_ACCELERATED | PFD_DOUBLEBUFFER | PFD_SWAP_EXCHANGE | PFD_TYPE_RGBA;
	pfd.cColorBits = 24;
	pfd.cDepthBits = 32;
	pfd.cAlphaBits = 8;
	pfd.iLayerType = PFD_MAIN_PLANE;
	int iPF = ChoosePixelFormat(hDC, &pfd);
	BOOL bR = SetPixelFormat(hDC,iPF,&pfd);

	HGLRC hGLRC = wglCreateContext(hDC);
	assert(hGLRC);
	BOOL b = wglMakeCurrent(hDC,hGLRC);
	assert(b);
	//int w = GetDeviceCaps(hDC,HORZRES);
	//int h = GetDeviceCaps(hDC,VERTRES);
	//glViewport( 0,0,1024,768);
	m_hDC = hDC;

	return 0;
}
void CProgram::UseCurrentDC(){
	m_hDC = wglGetCurrentDC();
}

IScene *CProgram::GetScene(){
	assert(m_pScene);
	return m_pScene;
}


void CProgram::PushNewBorn(class ISceneNode* p ){
	assert(p);
	if(p){
		m_newBorn.push(p);
	}
}


class ISceneNode *CProgram::PopNewBorn(){
	class ISceneNode *p = m_newBorn.front();
	m_newBorn.pop();
	return p;
}


class ISceneNode *CProgram::PeekNewBorn(){
	return m_newBorn.front();
}


void CProgram::SetCursorHS(float x,float y){
	if(m_cursorHS.x==x && m_cursorHS.y==y) return;
	if(m_pCurrentlyDragged){
		m_dragDelta += (vector2(x,y)-m_cursorHS)*GetConfigVar<float>(L"DragSpeedModifier");
	}
	if(!m_dragDelta.isZero()){
		matrix44 view,proj;
		glGetFloatv(GL_MODELVIEW_MATRIX,(GLfloat*)&view);
		glGetFloatv(GL_PROJECTION_MATRIX,(GLfloat*)&proj);
		matrix44 viewProj = proj*view;

		for(int i(0); i<20 && !m_dragDelta.isZero() ;++i ){
			m_dragDelta -= m_pCurrentlyDragged->Drag(
				vector2( MyUtilities::Clamped(m_dragDelta.x,-0.01f,0.01f) , MyUtilities::Clamped(m_dragDelta.y,-0.01f,0.01f) ),
				viewProj);
		}
		//m_dragDelta -= m_pCurrentlyDragged->Drag(
		//		m_dragDelta,
		//		viewProj);
	}
	m_cursorHS.set(x,y);
}


void CProgram::HideCursor(){
	SetCursorHS(-100.0f,-100.0f);
}


void CProgram::RenderCursor(){
	glPushAttrib(GL_ALL_ATTRIB_BITS);
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	glDisable(GL_LIGHTING);
	glDisable(GL_DEPTH_TEST);
	
	glBegin(GL_LINES);
	glColor3f(0.0,1.0,1.0);
	const static float r = 0.1f;
	glVertex2f(m_cursorHS.x-r,m_cursorHS.y);
	glVertex2f(m_cursorHS.x+r,m_cursorHS.y);
	glVertex2f(m_cursorHS.x,m_cursorHS.y-r);
	glVertex2f(m_cursorHS.x,m_cursorHS.y+r);
	glEnd();

	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();
	//glRasterPos3f(10000,0,2000);
	//glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18,'0');
	glPopAttrib();
}



int CProgram::PickCP(float x, float y){
	ClearPicks();
	matrix44 view,proj;
	glGetFloatv(GL_MODELVIEW_MATRIX,(GLfloat*)&view);
	glGetFloatv(GL_PROJECTION_MATRIX,(GLfloat*)&proj);
	//view.transpose();
	//proj.transpose();
	matrix44 viewProj = proj*view;
	for(int iAxis=0; iAxis<3; ++iAxis){
		m_axisAlignedCPSys[iAxis]->Pick(vector2(x,y),viewProj,back_inserter(m_pickedCPs));
	}
	if(!m_pickedCPs.empty()){
		m_selectedCP = 0;
	}
	return m_pickedCPs.size();
}


int CProgram::PickWithCursor(){
	return PickCP(m_cursorHS.x,m_cursorHS.y);
}


void CProgram::ClearPicks(){
	m_selectedCP = -1;
	m_pickedCPs.clear();
}


void CProgram::RequestTransformBaking(class ITransformBaked *p){
	m_transformBakingRequested.push_back(p);
}


void CProgram::BakeBatch(int x_mm,int y_mm,int z_mm,int rotZ_90deg){
	for(std::vector<class ITransformBaked*>::iterator itr = m_transformBakingRequested.begin(); itr != m_transformBakingRequested.end(); ++itr){
		(*itr)->BakeTransform(x_mm,y_mm,z_mm,rotZ_90deg);
	}
	m_transformBakingRequested.clear();
}


void CProgram::DiscardTransformBakingBatch(){
	assert(m_transformBakingRequested.empty());
	m_transformBakingRequested.clear();
}


class CControlPlaneSystem *CProgram::GetAxisAlignedCPSystem(int iAxis){
	assert(iAxis>=0);
	assert(iAxis<3);
	return m_axisAlignedCPSys[iAxis];
}



void CProgram::RenderCPSelection(){
	glPushAttrib(GL_ALL_ATTRIB_BITS);
	glColor3f(1,0,0);
	glDisable(GL_LIGHTING);
	glMatrixMode(GL_PROJECTION);
	{
		matrix44 m( 1 , 0 , 0 , 0 ,
					0 , 1 , 0 , 0 ,
					0 , 0 ,0.999999f,0 ,
					0 , 0 , 0 , 1 );
		glMultMatrixf((float*)&m);
	}
	//glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
	if(IsInOrthoView()){
		glDisable(GL_DEPTH_TEST);
	}
	for_each(m_pickedCPs.begin(),m_pickedCPs.end(),mem_fun(&IControlPlane::Render));
	//for(std::vector<IControlPlane*>::iterator itr = m_pickedCPs.begin(); itr!=m_pickedCPs.end(); ++itr){
		//(*itr)->Render();
	//}
	glDisable(GL_DEPTH_TEST);
	glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
	if(!m_pickedCPs.empty()){
		glColor3f(0,1,0);
		m_pickedCPs[m_selectedCP]->Render();
	}

	if(m_pTheOtherCP){
		glColor3f(1,1,0);
		m_pTheOtherCP->Render();
	}
	glPopAttrib();
}


void CProgram::NextCP(){
	if(m_pickedCPs.empty())
		return;
	m_selectedCP = (m_selectedCP+1)%m_pickedCPs.size();
}


void CProgram::PrevCP(){
	if(m_pickedCPs.empty())
		return;
	m_selectedCP = (m_selectedCP-1+m_pickedCPs.size())%m_pickedCPs.size();
}


void CProgram::MoveCP(int delta){
	if(m_pickedCPs.empty())
		return;
	if(!delta)
		return;
	m_pickedCPs[m_selectedCP]->Move(delta);
	unsigned int directionMask = 0;
	for(int iAxis=0; iAxis<3; ++iAxis){
		if(m_axisAlignedCPSys[iAxis]->Exists(m_pickedCPs[m_selectedCP])){
			directionMask = (1<<iAxis) | (1<<(iAxis+3));
			break;
		}
	}
	assert(directionMask);
	//for_each( GetSingleton<CTypedSceneNodeManager<CSceneNodeInteriorDef>>()->Begin(), GetSingleton<CTypedSceneNodeManager<CSceneNodeInteriorDef>>()->End(), 
	//	bind2nd( mem_fun1(&CSceneNodeInteriorDef::RepositionSubNodes), directionMask ) );
}



void CProgram::RememberCurCP(){
	if(m_pickedCPs.empty())
		return;
	m_pTheOtherCP = m_pickedCPs[m_selectedCP];
	ClearPicks();
}


void CProgram::ClearTheOtherCP(){
	m_pTheOtherCP = NULL;
}


void CProgram::AnnotateCP(){
	if(m_pickedCPs.empty() || !m_pTheOtherCP)
		return;
	if( m_pickedCPs[m_selectedCP] == m_pTheOtherCP ){
		return;
	}
	for(int iAxis=0; iAxis<3; ++iAxis){
		if( m_axisAlignedCPSys[iAxis]->Exists(m_pickedCPs[m_selectedCP]) && m_axisAlignedCPSys[iAxis]->Exists(m_pTheOtherCP) ){
			m_axisAlignedCPSys[iAxis]->AddAnnotation(m_pickedCPs[m_selectedCP],m_pTheOtherCP);
		}
	}
}


void CProgram::RenderAnnotations(){
	glPushAttrib(GL_ALL_ATTRIB_BITS);
	//glPushAttrib(GL_ALL_ATTRIB_BITS);
	glDisable(GL_LIGHTING);
	glDisable(GL_DEPTH_TEST);
	if(WIREFRAMEMODE == m_renderMode){
		glColor3f(0,0,0);
	}else{
		glColor3f(1,1,1);
	}
	for_each(m_axisAlignedCPSys,m_axisAlignedCPSys+3,mem_fun(&CControlPlaneSystem::RenderAnnotations));
	//for_each(m_pWardrobeCells.begin(),m_pWardrobeCells.end(),mem_fun(&IWardrobeCell::ShowCellDimensions));
	glColor3f(1,0.2,0);
	for_each(m_pAnnotated.begin(),m_pAnnotated.end(),mem_fun(&IAnnotated::ShowAnnotation));
	glColor3f(0.5,0,1);
	matrix44 view(GetViewTransform());
	matrix44 viewProj(GetViewProjTransform());
	{/*
		matrix44 view;
		glGetFloatv(GL_MODELVIEW_MATRIX,(GLfloat*)&view);
		matrix44 viewInv(view);
		viewInv.invert();*/
//		GetSingleton<CAnnotationStyle>()->SetCamUpWS((viewInv*vector4(0,1,0,0)).xyz());
//		GetSingleton<CAnnotationStyle>()->SetCamRightWS((viewInv*vector4(1,0,0,0)).xyz());
	}
	for(int iAxis(0); iAxis<3; ++iAxis){
		if(abs((view*CCoordSysAxis(iAxis).Vector4()).z)>0.8) 
			continue;
		CAxialAABBAnnotation *p(NULL);
		float dm(-0.5);
		for(std::map< CAxialAABBAnnotation *,vector3 >::iterator itr(m_pWallAnnotations[iAxis].begin());
			itr!=m_pWallAnnotations[iAxis].end();
			++itr
			)
		{
			float d( -(viewProj*vector4(itr->second,0)).z );
			//float d(itr->second.x);
			if(d>dm){
				dm = d;
				p = itr->first;
			}
		}
		if(p) p->ShowAnnotation();
	}
	glColor3f(0,0.5,1);
	if(m_pCurrentWardrobeCell) m_pCurrentWardrobeCell->VisualizeCell();
	glColor3f(0,1,0.5);
	if(m_pCurrentlyPicked) m_pCurrentlyPicked->VisualizePick();

	//{
	//	IBoxy *p(dynamic_cast<IBoxy*>(m_pCurrentlyPicked));
	//	if(p){
	//		GetSingleton<CAnnotationStyle>()->DrawAABBSelection(p->GetAABB());
	//	}
	//}
	//glPopAttrib();	
	glPopAttrib();	
}


void CProgram::RenderFloatingSymbols(){
	glPushAttrib(GL_ALL_ATTRIB_BITS);
	glDisable(GL_LIGHTING);
	glDisable(GL_DEPTH_TEST);
	if(WIREFRAMEMODE == m_renderMode){
		glColor3f(0,0,0);
	}else{
		glColor3f(1,1,1);
	}
	for_each(m_pFloatingSymbols.begin(),m_pFloatingSymbols.end(),mem_fun(&IFloatingSymbol::RenderSymbol));
	glPopAttrib();
}


void CProgram::RegisterFloatingSymbol(class IFloatingSymbol *p){
	m_pFloatingSymbols.insert(p);
}


CTexture *CProgram::GetTexture(const std::wstring &name){
	CTexture *&pTex = m_textures[name];
	if(NULL==pTex){
		pTex = new CTexture(name);
	}
	return pTex;
}


template<typename T> 
T CProgram::GetConfigVar(const std::wstring &path) const{
	return m_config.get<T>(path);
}
#define INST( type ) template type CProgram::GetConfigVar(const std::wstring &path) const
INST(bool);
INST(int);
INST(float);
#undef INST



template<typename T>
void CProgram::SetConfigVar(const std::wstring &path,const T &val){
	m_config.put<T>(path,val);
	
	m_minCellSize = m_propNodeMinCellSize->second.get_value<float>();
}
#define INST( type ) template void CProgram::SetConfigVar(const std::wstring &path,const type &val)
INST(bool);
INST(int);
INST(float);
#undef INST




int CProgram::SetConfigVar(std::wistream &in, std::wostream &out){
	using namespace boost::property_tree;
	std::wstring name;
	in>>name;
	wptree::assoc_iterator itr = m_config.find(name);
	if( m_config.not_found() == itr ){
		out<<L"No such config var.\n";
		return -1;
	}
	in>>itr->second.data();
	out<<name<<L' '<<itr->second.data()<<'\n';
	
	m_minCellSize = m_propNodeMinCellSize->second.get_value<float>();
	return 0;
}



int CProgram::ToggleBoolConfigVar(std::wistream &in, std::wostream &out){
	using namespace boost::property_tree;
	std::wstring name;
	in>>name;
	wptree::assoc_iterator itr = m_config.find(name);
	if( m_config.not_found() == itr ){
		out<<L"No such config var.\n";
		return -1;
	}
	itr->second.put_value(!itr->second.get_value<bool>());
	out<<name<<L' '<<itr->second.data()<<'\n';
	return 0;
}



void CProgram::RegisterPickable(int group, IPickable *p){
	m_pRegisteredPickables[group].insert(p);
}


void CProgram::UnregisterPickable(int group, IPickable *p){
	m_pRegisteredPickables[group].erase(p);
}


void CProgram::Pick(int group){
	UnpickWardrobeCell();
	Unpick();
	std::map< int,std::set<IPickable *> >::iterator itrGroup(m_pRegisteredPickables.find(group)); 
	if(m_pRegisteredPickables.end() == itrGroup){
		return;
	}
	matrix44 view,proj;
	glGetFloatv(GL_MODELVIEW_MATRIX,(GLfloat*)&view);
	glGetFloatv(GL_PROJECTION_MATRIX,(GLfloat*)&proj);
	matrix44 viewProj = proj*view;
	float nearest(100000.0f);
	for(std::set<IPickable*>::iterator i(itrGroup->second.begin()); i!=itrGroup->second.end(); ++i){
		float t((*i)->Pick(m_cursorHS,viewProj));
		if(t<nearest){
			assert(t>=0);
			nearest = t;
			m_pCurrentlyPicked = *i;
		}	
	}
}


void CProgram::Unpick(){
	m_pCurrentlyPicked = NULL;
}



void CProgram::BeginDrag(){
	EndDrag();
	m_pCurrentlyDragged = dynamic_cast<IDraggable*>(m_pCurrentlyPicked);
}


void CProgram::EndDrag(){
	m_pCurrentlyDragged = NULL;
	m_dragDelta.set(0,0);
}

void CProgram::RotatePicked(){
	CSceneNodeBoxyDraggableContainer *p = dynamic_cast<CSceneNodeBoxyDraggableContainer*>(m_pCurrentlyPicked);
	if(p){
		p->Rotate90Deg();
	}
}

void CProgram::RegisterWardrobeCell(class IWardrobeCell *pCell){
	m_pWardrobeCells.insert(pCell);
}

void CProgram::UnregisterWardrobeCell(class IWardrobeCell *pCell){
	//UnpickWardrobeCell();
	if(pCell == m_pCurrentWardrobeCell)  m_pCurrentWardrobeCell = NULL;
	m_pWardrobeCells.erase(pCell);
}


void CProgram::PickWardrobeCell(){
	Unpick();
	UnpickWardrobeCell();
	matrix44 viewProj(GetViewProjTransform());
	float nearest(10000.0f);
	for(std::set<class IWardrobeCell*>::iterator itr(m_pWardrobeCells.begin()); itr!= m_pWardrobeCells.end(); ++itr){
		float t((*itr)->PickCell(m_cursorHS,viewProj));
		if(t<nearest){
			assert(t>=0);
			nearest = t;
			m_pCurrentWardrobeCell = *itr;
		}
	}
}

void CProgram::UnpickWardrobeCell(){
	m_pCurrentWardrobeCell = NULL;
}

void CProgram::SplitWardrobeCell(int iAxis){
	if(m_pCurrentWardrobeCell){
		m_pCurrentWardrobeCell->SplitCell(iAxis);
	}
}

void CProgram::DeployNewBornIntoPickedCell(){
	if(!PeekNewBorn()){
		assert(0);
		return;
	}
	ISceneNode *p(PopNewBorn());
	if(m_pCurrentWardrobeCell&&dynamic_cast<ISceneNodeBoxy*>(p)){
		m_pCurrentWardrobeCell->InsertContent(dynamic_cast<ISceneNodeBoxy*>(p));
	}else{
		delete(p);
	}
}

const matrix44 CProgram::GetViewProjTransform()const{
	matrix44 view,proj;
	glGetFloatv(GL_MODELVIEW_MATRIX,(GLfloat*)&view);
	glGetFloatv(GL_PROJECTION_MATRIX,(GLfloat*)&proj);
	return proj*view;
}



void CProgram::RegisterAnnotated(class IAnnotated *p){
	m_pAnnotated.insert(p);
}



void CProgram::UnregisterAnnotated(class IAnnotated *p){
	m_pAnnotated.erase(p);
}



void CProgram::RemovePicked(){
	IRemovable *pRemovable(dynamic_cast<IRemovable*>(m_pCurrentlyPicked));
	Unpick();
	if(pRemovable&&pRemovable->Removable()) {
		CSceneNodeBinaryWardrobeTreeNode *pbwt(dynamic_cast<CSceneNodeBinaryWardrobeTreeNode *>(pRemovable));
		if(pbwt){
			pbwt=pbwt->Root();
		}
		pRemovable->Remove();
		if(pbwt){
			pbwt->ConsistencyCheck();
		}
	}
}




void CProgram::AlignPickedWithPointedAt(int group,unsigned dirMask){
	IPickable *pPrevPicked(m_pCurrentlyPicked);
	IBoxy *pBoxy(dynamic_cast<IBoxy*>(pPrevPicked));
	if( !pBoxy ) return;
	Pick(group);
	IBoxy *pPointedAt(dynamic_cast<IBoxy*>(m_pCurrentlyPicked));
	if(pPointedAt){
		AABB aabb(pBoxy->GetAABB());
		if(dirMask&1){
			aabb.first.x = pPointedAt->GetAABB().first.x;
		}
		if(dirMask&2){
			aabb.first.y = pPointedAt->GetAABB().first.y;
		}
		if(dirMask&4){
			aabb.first.z = pPointedAt->GetAABB().first.z;
		}
		if(dirMask&8){
			aabb.second.x = pPointedAt->GetAABB().second.x;
		}
		if(dirMask&16){
			aabb.second.y = pPointedAt->GetAABB().second.y;
		}
		if(dirMask&32){
			aabb.second.z = pPointedAt->GetAABB().second.z;
		}
		pBoxy->SetAABB(aabb);
	}
	m_pCurrentlyPicked = pPrevPicked;
}


void CProgram::ConnectPickedToPointedAt(int group){
	IPickable *pPrevPicked(m_pCurrentlyPicked);
	IConnectible *pConnectible(dynamic_cast<IConnectible*>(pPrevPicked));
	if( !pConnectible ) return;
	Pick(group);
	IConnectible *pPointedAt(dynamic_cast<IConnectible*>(m_pCurrentlyPicked));
	if(pPointedAt){
		pPointedAt->ConnectTo(*pConnectible);
		pConnectible->ConnectTo(*pPointedAt);
	}
	m_pCurrentlyPicked = pPrevPicked;
}

void CProgram::DisconnectPickedFromPointedAt(int group){
	IPickable *pPrevPicked(m_pCurrentlyPicked);
	IConnectible *pConnectible(dynamic_cast<IConnectible*>(pPrevPicked));
	if( !pConnectible ) return;
	Pick(group);
	IConnectible *pPointedAt(dynamic_cast<IConnectible*>(m_pCurrentlyPicked));
	if(pPointedAt){
		pPointedAt->DisconnectFrom(*pConnectible);
		pConnectible->DisconnectFrom(*pPointedAt);
	}
	m_pCurrentlyPicked = pPrevPicked;
}

CAxialAABBAnnotation *CProgram::GetWallAnnotation(const vector3 &facing, int iAxis) const{
	for(map< class CAxialAABBAnnotation *,vector3 >::const_iterator i(m_pWallAnnotations[iAxis].begin()); i!=m_pWallAnnotations[iAxis].end(); ++i){
		if(DotProduct(i->second,facing)>0.99f){
			return i->first;
		}
	}
	return NULL;
}



void CProgram::RegisterWallAnnotation(class CAxialAABBAnnotation *p, const vector3 &facing, int iAxis){
	//RegisterAnnotated(p);
	m_pWallAnnotations[iAxis].insert(make_pair(p,facing));
}



void CProgram::UnregisterWallAnnotation(class CAxialAABBAnnotation *p, int iAxis){
	m_pWallAnnotations[iAxis].erase(p);
	//UnregisterAnnotated(p);
}



const matrix44 CProgram::GetViewTransform(){
	matrix44 view;
	glGetFloatv(GL_MODELVIEW_MATRIX,(GLfloat*)&view);
	return view;
}



void CProgram::LoadMaterials(){
	_wfinddata_t found;
	intptr_t h(_wfindfirst( L"Materials\\*.xml" , &found));
	assert(-1 != h);
	do{
		using namespace boost::property_tree;
		wptree tr;
		wchar_t path[260] = L"Materials\\";
		wcscat_s( path, 260, found.name );
		//wifstream f( _wfopen(path,L"rb") );
		wifstream f( path );
		f.imbue( locale(locale(),new boost::archive::detail::utf8_codecvt_facet) );
		if(65279==f.peek()) f.ignore();
		//f.ignore();
		//{
		//	wstring ws;
		//	wchar_t buf[100];
		//	f.getline(buf,90);
		//	f>>ws;
		//	f>>ws;
		//	f>>ws;
		//}
		assert( f.is_open() );
		try{
			Stupid::StupidlyReadXmlToCreateWPTree(f,tr);
			//read_xml(f,tr);
		}catch(xml_parser_error &err){
			assert(0);
		}
		for(wptree::iterator i(tr.begin()); i != tr.end(); ++i){
			assert(i->first == L"SimpleMaterial");
			CSimpleMaterial *p( new CSimpleMaterial(i->second) );
			m_pMaterials.insert( std::pair<CIdentifier,CSimpleMaterial*>(p->GetIdentifier(),p) );
		}
	}while(-1 != _wfindnext(h,&found));
	_findclose(h);
	if(!m_pMaterials.empty())	m_pCurrentMaterial = m_pMaterials.begin()->second;
}



class IMaterial *CProgram::GetCurrentMaterial() const{
	return m_pCurrentMaterial;
}


void CProgram::SetCurrentMaterial(const class CIdentifier &id){
	m_pCurrentMaterial = m_pMaterials.find(id)->second;
}



namespace CProgram_impl{
	void LoadDxfHeader(const std::wstring &directory, std::map< class CIdentifier , class CDxfHeader * > &container){
		_wfinddata_t found;
		intptr_t h( _wfindfirst( (directory+L"\\*.txt").c_str() , &found ) );
		assert(-1 != h);
		do{
			container.insert(make_pair( CIdentifier(found.name),new CDxfHeader(directory+L"\\"+wstring(found.name)) ));
		}while(-1 != _wfindnext(h,&found));
	}
}



void CProgram::LoadDxfHeaders(){
	LoadDxfHeader(L"DxfHeaders\\Tables",m_pTablesSecDxfHeaders);
	m_pCurrentTablesSecDxfHeader = m_pTablesSecDxfHeaders.begin()->second;
}



CDxfHeader *CProgram::GetCurrentDxfHeaderTablesSec() const{
	return m_pCurrentTablesSecDxfHeader;
}


void CProgram::SetCurrentDxfHeaderTablesSec(const CIdentifier &id){
	m_pCurrentTablesSecDxfHeader = m_pTablesSecDxfHeaders.find(id)->second;
}


void CProgram::FormatPickedToDXF(const std::wstring &path){
	const CBoard *pBoard(NULL);
	CSceneNodeBoard *p( dynamic_cast<CSceneNodeBoard*>(m_pCurrentlyPicked) );
	if(p){
		p->PunchHoles();
		pBoard = &p->GetBoard();
	}
	//IBoardSource *pSource( dynamic_cast<IBoardSource*>(m_pCurrentlyPicked) );
	//if(pSource){
	//	pBoard = pSource->GetBoard();
	//}
	if(!pBoard) return;
	std::wofstream os(path.c_str());
	pBoard->WriteToDXF<std::wostream>(os);
}






IDrillingRule *CProgram::GetCurrentDrillingRule(unsigned direction){
	if(direction%3 == 0) return NULL;
	static CClassicDrillingRule r;
	return &r;
}



void CProgram::LoadHoleStyles(){
	_wfinddata_t found;
	intptr_t h(_wfindfirst( L"HoleStyles\\*.xml" , &found));
	assert(-1 != h);
	do{
		using namespace boost::property_tree;
		wptree tr;
		wchar_t path[260] = L"HoleStyles\\";
		wcscat_s( path, 260, found.name );
		wifstream f( path );
		assert( f.is_open() );
		f.imbue( locale(locale(),new boost::archive::detail::utf8_codecvt_facet) );
		if(65279==f.peek()) f.ignore();
		/*{
			wstring s;
			f>>s;
			f>>s;
			f>>s;
			f>>s;
			f>>s;
		}*/
		try{
			Stupid::StupidlyReadXmlToCreateWPTree(f,tr);
			//read_xml(f,tr);
		}catch(xml_parser_error &err){
			assert(0);
		}
		for(wptree::iterator i(tr.begin()); i != tr.end(); ++i){
			m_pHoleStyles.insert(make_pair(i->first,new CHoleStyle(i->second)));
		}
	}while(-1 != _wfindnext(h,&found));
	_findclose(h);
	if(!m_pMaterials.empty())	m_pCurrentMaterial = m_pMaterials.begin()->second;
}




class IHoleStyle *CProgram::GetHoleStyle(const std::wstring &name)
{
	std::map< std::wstring , class IHoleStyle * >::iterator i( m_pHoleStyles.find(name) );
	if(i == m_pHoleStyles.end()) return NULL;
	return i->second;
}



void CProgram::LoadManufactureProperties(){
	_wfinddata_t found;
	intptr_t h(_wfindfirst( L"ManufactureProperties\\*.xml" , &found));
	assert(-1 != h);
	do{
		wchar_t path[260] = L"ManufactureProperties\\";
		wcscat_s( path, 260, found.name );
		LoadManufacturePropertiesFromFile(path);
	}while(-1 != _wfindnext(h,&found));
	_findclose(h);
}


void CProgram::LoadManufacturePropertiesFromFile(const std::wstring &path){
	using namespace boost::property_tree;
	wptree tr;
	wifstream f( path.c_str() );
	assert( f.is_open() );
	f.imbue( locale(locale(),new boost::archive::detail::utf8_codecvt_facet) );
	if(65279==f.peek()) f.ignore();
	try{
		Stupid::StupidlyReadXmlToCreateWPTree(f,tr);
		//read_xml(f,tr);
	}catch(xml_parser_error &err){
		assert(0);
	}
	m_manufactureProperties.insert(m_manufactureProperties.end(),tr.begin(),tr.end());
}


void CProgram::LoadGeneratorsFromFile(const std::wstring &path){
	using namespace boost::property_tree;
	wptree tr;
	wifstream f( path.c_str() );
	assert( f.is_open() );
	f.imbue( locale(locale(),new boost::archive::detail::utf8_codecvt_facet) );
	if(65279==f.peek()) f.ignore();
	try{
		Stupid::StupidlyReadXmlToCreateWPTree(f,tr);
		//read_xml(f,tr);
	}catch(xml_parser_error &err){
		assert(0);
	}
	m_generators.insert(m_generators.end(),tr.begin(),tr.end());
	{
		using namespace boost::property_tree;
		for(wptree::iterator i( tr.begin() ); i!=tr.end(); ++i){
			std::wstring s(i->first);
			s;
		}
	}
}


void CProgram::LoadGenerators(){
	_wfinddata_t found;
	intptr_t h(_wfindfirst( L"Generators\\*.xml" , &found));
	assert(-1 != h);
	do{
		wchar_t path[260] = L"Generators\\";
		wcscat_s( path, 260, found.name );
		LoadGeneratorsFromFile(path);
	}while(-1 != _wfindnext(h,&found));
	_findclose(h);
}


namespace CProgram_impl{
	template<typename _InputItr> void WriteBoardList(std::wostream &os, _InputItr first, _InputItr last, size_t startingId){
		struct Item{
			const CIdentifier &material;
			const std::wstring &boardName;
			const unsigned szX,szY;
			const std::wstring &comments;
			explicit Item(const CBoard *p)
				:
				material(p->GetMaterial()->GetIdentifier()),
				boardName(p->GetName()),
				szX(p->GetSize().x),
				szY(p->GetSize().y),
				comments(p->GetComments())
			{}
			bool operator<(const Item &rhs)const{
				if(material < rhs.material){
					return true;
				}else if(material == rhs.material){
					if(boardName < rhs.boardName){
						return true;
					}else if(boardName == rhs.boardName){
						if(szX < rhs.szX){
							return true;
						}else if(szX == rhs.szX){
							if(szY < rhs.szY){
								return true;
							}else if(szY == rhs.szY){
								return comments<rhs.comments;
							}
						}
					}
				}
				return false;
			}
			bool operator==(const Item &rhs)const{
				return material==rhs.material && boardName==rhs.boardName && szX==rhs.szX && szY==rhs.szY && comments==rhs.comments;
			}
			const Item &operator=(const Item &rhs){
				memcpy(this,&rhs,sizeof(Item));
				return *this;
			}
		};
		vector<Item> m_items;
		//m_items.reserve(last-first);
		//copy(first,last,back_inserter(m_items));
		for(_InputItr i(first); i!=last; ++i) if((*i)->IsEnabled()){
			m_items.push_back(Item(*i));
		}
		sort(m_items.begin(),m_items.end());
		size_t id(startingId);
		{
			size_t id(startingId);
			vector<Item>::const_iterator i(m_items.begin());
			while(i!=m_items.end()){
				vector<Item>::const_iterator j(i);
				size_t c(0);
				do{
					++c;
					++j;
				}while(j!=m_items.end() && *j==*i);
				os<<id++<<L'\t'<<i->material.GetString()<<L'\t'<<i->boardName<<L'\t'<<i->szX<<L'\t'<<i->szY<<L'\t'<<c<<L'\t'<<i->comments<<endl;
				i=j;
			}
		}
	}
};

void CProgram::OutputBoardList(std::wostream &os){
	os<<L"序号\t材质\t名称\t长\t宽\t数量\t工艺说明\n";
	//int id(0);
	//for(std::set<const CBoard *>::const_iterator i( m_boards.begin() ); i!=m_boards.end(); ++i){
	//	os<<id++<<L'\t'<<(*i)->GetMaterial()->GetIdentifier().GetString()<<L'\t'<<(*i)->GetSize().x<<L'\t'<<(*i)->GetSize().y<<std::endl;
	//}
	WriteBoardList(os, m_boards.begin(), m_boards.end(), 0);
}



void CProgram::ActivateCamera(){
	if(!m_pCamera.get()){
		m_pCamera.reset(new CTargetCamera());
	}
	m_pCamera->Activate();
}



void CProgram::DeactivateCamera(){
	if(!m_pCamera.get()) return;
	m_pCamera->Deactivate();
}


void CProgram::CenterCameraView(){
	m_pCamera->SetTarget(GetCenter(m_pScene->GetAABB()));
	//m_pScene
}


void CProgram::FitView(){
	using namespace MyUtilities;
	AABB aabbVS( GetViewTransform()*m_pScene->GetAABB() );
	float w = 1000,h = 1000,zNear = 1,zFar = 50000;
	ClampL( h, abs(aabbVS.first.y) );
	ClampL( h, abs(aabbVS.second.y) );
	ClampL( w, abs(aabbVS.first.x) );
	ClampL( w, abs(aabbVS.second.x) );
	h*=2.4;
	w*=2.4;
	{
		float vp[4];
		glGetFloatv(GL_VIEWPORT,vp);
		float aspectRatio(vp[2]/vp[3]);
		float newW = aspectRatio*h;
		float newH = w/aspectRatio;
		ClampL( w , newW );
		ClampL( h , newH );
	}
	glPushAttrib(GL_TRANSFORM_BIT);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(-w*0.5,w*0.5,-h*0.5,h*0.5,zNear,zFar);
	glPopAttrib();
}


namespace CProgram_impl{
	void LoadPropertyFile(const std::wstring &path , boost::property_tree::wptree &addTo){
		using namespace boost::property_tree;
		wptree tr;
		wifstream f( path.c_str() );
		assert(f.is_open());
		f.imbue( locale(locale(),new boost::archive::detail::utf8_codecvt_facet) );
		if(65279==f.peek()) f.ignore();
		try{
			Stupid::StupidlyReadXmlToCreateWPTree(f,tr);
		}catch(xml_parser_error &err){
			assert(0);
		}
		addTo.insert(addTo.end(),tr.begin(),tr.end());
	}
	void LoadAllXmlFilesUnderDirectory(const std::wstring &path , boost::property_tree::wptree &addTo){
		_wfinddata_t found;
		intptr_t h( _wfindfirst( (path+L"\\*.xml").c_str() , &found ) );
		assert(-1!=h);
		do{
			LoadPropertyFile(path+L"\\"+found.name,addTo);
		}while(-1 != _wfindnext(h,&found));
		_findclose(h);
	}
	template<class _Func>void ForEachFileDo(const std::wstring &path , const std::wstring &type , _Func func){
		_wfinddata_t found;
		intptr_t h( _wfindfirst( (path+L"\\"+type).c_str(), &found) );
		assert(-1!=h);
		do{
			func(path+L"\\"+found.name);
		}while(-1 != _wfindnext(h,&found));
		_findclose(h);
	}
	
}

void CProgram::LoadGenericComponentSchemeFromFile(const std::wstring &path){
	LoadPropertyFile(path,m_generics);
}


void CProgram::LoadGenericComponentSchemes(){
	LoadAllXmlFilesUnderDirectory(L"Generics",m_generics);
}


void CProgram::LoadGenericComponentVariantTableFromFile(std::wstring path){
	GenericComponentVariantTableRegistryType &addTo(m_genericComponentVariantTables);
	wifstream is(path.c_str());
	assert(is.is_open());
	is.imbue( locale(locale(),new boost::archive::detail::utf8_codecvt_facet) );
	if(65279==is.peek()) is.ignore();
	GenericComponentVariantTableRegistryType::iterator i( addTo.end() );
	while(!is.eof()){
		wstring line;
		getline(ws(is),line);
		if(line.empty())	continue;
		if(line[0] == L'\\'){
			wstring name;
			ws(wistringstream(line).ignore())>>name;
			i = addTo.lower_bound(name);
			if( addTo.end()==i || name<i->first){
				i = addTo.insert(i,make_pair(name,GenericComponentVariantTableType()));
			}
			//CProgram::
		}else{
			if(addTo.end()==i){
				assert(0);
			}else{
				wstring id;
				GenericComponentVariantType variant;
				wstringstream(line)>>id>>variant;
				i->second.insert(make_pair(id,variant));
			}
		}
	}
}


void CProgram::LoadGenericComponentVariantTables(){
	ForEachFileDo(L"VariantTables",L"*.txt",bind1st(mem_fun(&CProgram::LoadGenericComponentVariantTableFromFile),this));
}


class IMaterial *CProgram::GetMaterial(const class CIdentifier &id) const{
	std::map< class CIdentifier , class CSimpleMaterial * >::const_iterator i(m_pMaterials.find(id));
	return m_pMaterials.end()==i?NULL:i->second;
}


ISceneNodeBoxy *CProgram::CreateGenericComponentVariant(const std::wstring &scheme, const std::wstring &variant, unsigned flags){
	//p = new CSceneNode
//	return p.release();
	//make_pair(variant,m_genericComponentVariantTables[scheme][variant]);
	//return NULL;
	return new CSceneNodeBoxyGenericComponentVariant(
		*new CSceneNodeBoxyGenericComponent(GetGenericComponentScheme().get_child(scheme)),
		*m_genericComponentVariantTables.find(scheme)->second.find(variant),
		flags
		);
}


void CProgram::OnResize(unsigned int w, unsigned int h){
	glViewport(0,0,w,h);
}


void CProgram::ListMaterials(std::wostream &out) const{
	for(std::map< class CIdentifier , class CSimpleMaterial * >::const_iterator i(m_pMaterials.begin());i!=m_pMaterials.end();++i){
		out<<i->first.GetString()<<std::endl;
	}
}


void CProgram::ListVariants(const std::wstring &scheme, std::wostream &out) const{
	const GenericComponentVariantTableType &tbl(m_genericComponentVariantTables.find(scheme)->second);
	for(GenericComponentVariantTableType::const_iterator i(tbl.begin()); i!=tbl.end(); ++i){
		out<<i->first<<std::endl;
	}
}


template<> const std::vector<class CSceneNodeRoom*> &CProgram::GetOrderedObjectRegistry() const{	return m_roomList;	}
template<> const std::vector<class CSceneNodeWardrobe*> &CProgram::GetOrderedObjectRegistry() const{	return m_wardrobeList; }

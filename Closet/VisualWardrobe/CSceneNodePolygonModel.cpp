#include "CSceneNodePolygonModel.h"
#include "CProgram.h"
#include "singleton.h"
#include "3d.h"
#include <cassert>
#include <algorithm>
#include <sstream>
#include "CControlPlaneSystem.h"
#include "CTexture.h"
#include "CRegistryStack.h"
#include "CSceneNodeWallDef.h"
#include "stdext.h"

using namespace std;
CSceneNodePolygonModel::CSceneNodePolygonModel(const boost::property_tree::wptree &tr):
m_pDiffuseTex(NULL){
	using namespace boost::property_tree;
	//m_minAABB = pt.get<vector3>("Scene.Min");
	//m_maxAABB = pt.get<vector3>("Scene.Max");
	//m_name = tr.get<wstring>( L"Name" );
	{
		const wptree &vs = tr.get_child( L"Verts" );
		int nV = vs.get<int>( L"<xmlattr>.num" );
		m_verts.clear();
		m_verts.reserve(nV);
		for(wptree::const_iterator itr = vs.begin(); itr!=vs.end(); ++itr){
			if( itr->first == L"Vertex" ){
				VERTEX v;
				v.pos = itr->second.get_value<vector3>();
				m_verts.push_back(v);
			}else{
				//wstring ss = itr->first;
				assert( itr->first == L"<xmlattr>" );
			}
		}
		assert(m_verts.size() == nV);
	}
	{
		const wptree &fs = tr.get_child( L"Faces" );
		int nF = fs.get<int>( L"<xmlattr>.num" );
		m_faces.clear();
		m_faces.reserve(nF);
		for(wptree::const_iterator itr = fs.begin(); itr!=fs.end(); ++itr){
			if( itr->first == L"Face" ){
				FACE f;
				f.degree = itr->second.get<int>(L"<xmlattr>.degree");
				wistringstream(itr->second.get_child(L"Verts").data())>>f.verts;
				wistringstream(itr->second.get_child(L"Edges").data())>>f.edges;
				//wistringstream(itr->second.get_child(L"MapVerts").data())>>f.mapVerts;
				assert(f.verts.size() == f.degree);
				assert(f.edges.size() == f.degree);
				//assert(f.mapVerts.size() == f.degree);
				f.normal = itr->second.get<vector3>(L"Normal");
				m_faces.push_back(f);
			}else{
				assert( itr->first == L"<xmlattr>" );
			}
		}
		assert(m_faces.size() == nF);
	}
	//{
	//	const wptree &mvs = tr.get_child( L"MapVerts");
	//	int nMV = mvs.get<int>( L"<xmlattr>.num" );
	//	m_mapVerts.clear();
	//	m_mapVerts.reserve(nMV);
	//	for(wptree::const_iterator itr = mvs.begin(); itr!=mvs.end(); ++itr){
	//		if( itr->first == L"MapVert" ){
	//			MAPVERT mv;
	//			mv.uv = itr->second.get_value<vector2>();
	//			m_mapVerts.push_back(mv);
	//		}else{
	//			assert( itr->first == L"<xmlattr>" );
	//		}
	//	}
	//	assert(m_mapVerts.size() == nMV);
	//}
	{
		const wptree &es = tr.get_child( L"Edges" );
		int nE = es.get<int>( L"<xmlattr>.num" );
		m_edges.clear();
		m_edges.reserve(nE);
		for(wptree::const_iterator itr = es.begin(); itr!=es.end(); ++itr){
			if( itr->first == L"Edge" ){
				EDGE e;
				wistringstream(itr->second.get_child(L"Verts").data())>>e.verts[0]>>e.verts[1];
				wistringstream(itr->second.get_child(L"Faces").data())>>e.faces[0]>>e.faces[1];
				m_edges.push_back(e);
			}else{
				assert( itr->first == L"<xmlattr>" );
			}
		}
	}
	//{
	//	wchar_t axisName[3] = {L'X',L'Y',L'Z'};
	//	for(int iAxis=0 ; iAxis<3 ; ++iAxis){
	//		wstring nodeName(wstring(L"ControlPlane")+axisName[iAxis]+L"Set");
	//		const wptree &cpSet = tr.get_child(nodeName);
	//		for(wptree::const_iterator itr = cpSet.begin(); itr!=cpSet.end(); ++itr){
	//			if( itr->first == L"ControlPlane" ){
	//				CControlPlane *cp = new CControlPlane( *this, itr->second );
	//				m_pControlPlanesAxisAligned[iAxis].push_back(cp);
	//			}else{
	//				assert( 0 );
	//			}
	//		}
	//	}
	//}
	if(tr.find(L"DiffuseMap")!=tr.not_found()){
		m_pDiffuseTex = GetSingleton<CProgram>()->GetTexture(tr.get_child(L"DiffuseMap").data());
	}
	//GetSingleton<CRegistryStack<CSceneNodePolygonModel *>>()->Insert(m_name,this);
	//pt.get<
}
void CSceneNodePolygonModel::Render() const{
	glPushAttrib(GL_ALL_ATTRIB_BITS);
	if(m_pDiffuseTex){
		NTexture::Bind(*m_pDiffuseTex);
		assert(GL_NO_ERROR == glGetError());
		glEnable(GL_TEXTURE_2D);
		assert(GL_NO_ERROR == glGetError());
		glDisable(GL_COLOR_MATERIAL);
		assert(GL_NO_ERROR == glGetError());
	}
	for_each(m_faces.begin(),m_faces.end(),DrawFace(m_verts,NULL));//&m_mapVerts));
	assert(GL_NO_ERROR == glGetError());
	glPopAttrib();
}


//CSceneNodePolygonModel::CControlPlane::CControlPlane(CSceneNodePolygonModel &poly,const boost::property_tree::wptree &tr):
//m_polyModel(poly)
//{
//	wistringstream(tr.get_child(L"Verts").data())>>m_verts;
//	wistringstream(tr.get_child(L"Faces").data())>>m_faces;
//	sort(m_verts.begin(),m_verts.end());
//	sort(m_faces.begin(),m_faces.end());
//}




void CSceneNodePolygonModel::DrawFace::operator()(const FACE &face) const{
	assert(face.edges.size() == face.degree);
	assert(face.verts.size() == face.degree);
	//assert(face.mapVerts.size() == face.degree);
	glBegin(GL_POLYGON);
	glNormal3f(face.normal.x,face.normal.y,face.normal.z);
	//for(std::vector<int>::const_iterator iId = face.verts.begin(); iId != face.verts.end(); ++iId){
	//	glVertex3f(m_verts[*iId].pos.x,m_verts[*iId].pos.y,m_verts[*iId].pos.z);
	//}
	//if(m_pMapVerts){
	//	for(std::vector<int>::const_iterator iId = face.mapVerts.begin(); iId != face.mapVerts.end(); ++iId){
	//		glTexCoord2f((*m_pMapVerts)[*iId].uv.x,(*m_pMapVerts)[*iId].uv.y);
	//	}
	//}
	for(int i=0;i<face.degree;++i){
		if(m_pMapVerts){
			//glTexCoord2f((*m_pMapVerts)[face.mapVerts[i]].uv.x,(*m_pMapVerts)[face.mapVerts[i]].uv.y);
		}
		glVertex3f(m_verts[face.verts[i]].pos.x,m_verts[face.verts[i]].pos.y,m_verts[face.verts[i]].pos.z);
	}
	glEnd();
}


//float CSceneNodePolygonModel::PickFace::operator()(const FACE &face) const{
//	assert(face.edges.size() == face.degree);
//	assert(face.verts.size() == face.degree);
//	bool strictLeft = false;
//	bool strictRight = false;
//	float zmax = -1.0f;
//	for(int i=0; i<face.degree; ++i){
//		vector4 head4 = m_viewProj*vector4(m_verts[face.verts[i]].pos,1.0f);
//		vector4 tail4 = m_viewProj*vector4(m_verts[face.verts[(i+1)%face.degree]].pos,1.0f);
//		vector2 head2(head4.x,head4.y);
//		vector2 tail2(tail4.x,tail4.y);
//		float c = CrossProduct(tail2-head2,m_hpos-head2);
//		const static float epsilon = 0.01;
//		strictLeft|=(c>epsilon);
//		strictRight|=(c<-epsilon);
//		//head4.z = -head4.z;
//		if(head4.z>zmax){
//			zmax = head4.z;
//		}
//	}
//	//if(GetSingleton<CProgram>()->IsInOrthoView()){
//	//if( GetSingleton<CProgram>()->IsInWireFrameMode() && (strictLeft||strictRight) ){
//	//	return -1.0f;
//	//}
//	if(strictLeft && strictRight){
//		return -1.0f;
//	}else{
//		return 1.0f;//zmax;
//	}
//}


//float CSceneNodePolygonModel::PickFaceEdges::operator()(const FACE &face) const{
//	assert(face.edges.size() == face.degree);
//	assert(face.verts.size() == face.degree);
//	//float zmax = -1.0f;
//	unsigned int onLine(GetSingleton<CProgram>()->IsInOrthoView()?0:face.edges.size());
//	unsigned int onLineSeg(0);
//	for(std::vector<int>::const_iterator iEdgeID=face.edges.begin(); iEdgeID!=face.edges.end(); ++iEdgeID){
//		const EDGE &edge(m_edges[*iEdgeID]);
//		const VERTEX &v1st(m_verts[edge.verts[0]]);
//		const VERTEX &v2nd(m_verts[edge.verts[1]]);
//		vector2 h1((m_viewProj*vector4(v1st.pos,1.0f)).xy()),h2((m_viewProj*vector4(v2nd.pos,1.0f)).xy());
//		vector2 d(h1-h2),n(-d.y,d.x);
//		const float epsilon = 0.005;
//		if(d.lengthSqr()<epsilon*epsilon) {
//			++onLine;
//			continue;
//		}
//		d.normalize();
//		n.normalize();
//		float pD( DotProduct(m_hpos,d) );
//		float u1( DotProduct(h1,d) );
//		float u2( DotProduct(h2,d) );
//		float pN( DotProduct(m_hpos,n) );
//		float v( DotProduct(h1,n) );
//		if(abs(pN-v)<epsilon){
//			++onLine;
//			if( min(u1,u2)<=pD && pD<=max(u1,u2) ) {
//				++onLineSeg;
//			}
//		}
//	}
//	return (face.edges.size()<=onLine && onLineSeg)?1.0f:-1.0f;
//}



//void CSceneNodePolygonModel::BakeTransform(int x_mm,int y_mm,int z_mm,int rotZ_90deg){
//	if(1 == rotZ_90deg){
//		for(std::vector<VERTEX>::iterator iV = m_verts.begin(); iV != m_verts.end(); ++iV){
//			iV->pos = vector3(-iV->pos.y,iV->pos.x,iV->pos.z);
//		}
//		for(std::vector<FACE>::iterator iF = m_faces.begin(); iF != m_faces.end(); ++iF){
//			iF->normal = vector3(-iF->normal.y,iF->normal.x,iF->normal.z);
//		}
//	}else if(2 == rotZ_90deg){
//		for(std::vector<VERTEX>::iterator iV = m_verts.begin(); iV != m_verts.end(); ++iV){
//			iV->pos = vector3(-iV->pos.x,-iV->pos.y,iV->pos.z);
//		}
//		for(std::vector<FACE>::iterator iF = m_faces.begin(); iF != m_faces.end(); ++iF){
//			iF->normal = vector3(-iF->normal.x,-iF->normal.y,iF->normal.z);
//		}
//	}else if(3 == rotZ_90deg){
//		for(std::vector<VERTEX>::iterator iV = m_verts.begin(); iV != m_verts.end(); ++iV){
//			iV->pos = vector3(iV->pos.y,-iV->pos.x,iV->pos.z);
//		}
//		for(std::vector<FACE>::iterator iF = m_faces.begin(); iF != m_faces.end(); ++iF){
//			iF->normal = vector3(iF->normal.y,-iF->normal.x,iF->normal.z);
//		}
//	}
//	for(std::vector<VERTEX>::iterator iV = m_verts.begin(); iV != m_verts.end(); ++iV){
//		iV->pos.x+=x_mm;
//		iV->pos.y+=y_mm;
//		iV->pos.z+=z_mm;
//	}
//	if(rotZ_90deg%2){
//		m_pControlPlanesAxisAligned[0].swap(m_pControlPlanesAxisAligned[1]);
//	}
//	for(int iAxis=0; iAxis<3; ++iAxis){
//		for_each(m_pControlPlanesAxisAligned[iAxis].begin(),m_pControlPlanesAxisAligned[iAxis].end(),
//			bind2nd(mem_fun( &CControlPlane::RegisterToSystem ), GetSingleton<CProgram>()->GetAxisAlignedCPSystem(iAxis) ) );
//	}
//}


void CSceneNodePolygonModel::OnInsertion(){
	//GetSingleton<CProgram>()->RequestTransformBaking(this);
}


//void CSceneNodePolygonModel::RegisterWallDef(CSceneNodeWallDef &wallDef){
//	for_each(
//		m_pControlPlanesAxisAligned[wallDef.GetAxis()].begin(),
//		m_pControlPlanesAxisAligned[wallDef.GetAxis()].end(),
//		bind2nd(mem_fun(&CControlPlane::RegisterWallDef),&wallDef)
//		);
//}


CSceneNodePolygonModel::~CSceneNodePolygonModel(){
	//for(int iAxis = 0; iAxis<3 ; ++iAxis){
	//	for(std::vector<CControlPlane*>::iterator itr = m_pControlPlanesAxisAligned[iAxis].begin(); itr!= m_pControlPlanesAxisAligned[iAxis].end(); ++itr){
	//		GetSingleton<CProgram>()->GetAxisAlignedCPSystem(iAxis)->Erase(*itr);
	//		delete *itr;
	//	}
	//}
}


void CSceneNodePolygonModel::EnlargeAABB(AABB &aabb)const{
	for(std::vector<VERTEX>::const_iterator i(m_verts.begin()); i!=m_verts.end(); ++i){
		Enlarge(aabb,i->pos);
	}
}








//void CSceneNodePolygonModel::CControlPlane::Render() const{
//	for(std::vector<int>::const_iterator iF = m_faces.begin(); iF!=m_faces.end(); ++iF){
//		DrawFace(m_polyModel.m_verts)(m_polyModel.m_faces[*iF]);
//	}
//}
//
//
//float CSceneNodePolygonModel::CControlPlane::Pick(const class vector2 &posHS,const class matrix44 &viewProj) const{
//	float zmax = -1.0f;
//	for(std::vector<int>::const_iterator iF = m_faces.begin(); iF!=m_faces.end(); ++iF){
//		float z = 
//			GetSingleton<CProgram>()->IsInWireFrameMode() ? 
//				PickFaceEdges(m_polyModel.m_edges,m_polyModel.m_verts,viewProj,posHS)(m_polyModel.m_faces[*iF])
//				: PickFace(m_polyModel.m_verts,viewProj,posHS)(m_polyModel.m_faces[*iF]);
//		if(z>zmax){
//			zmax = z;
//		}
//	}
//	return zmax;
//}
//
//
//void CSceneNodePolygonModel::CControlPlane::RegisterToSystem(class CControlPlaneSystem *p){
//	{
//		vector3 normal = m_polyModel.m_faces[*m_faces.begin()].normal;
//		assert(abs(DotProduct(normal,p->GetDir()))>0.99f);
//	}
//	p->Insert(this);
//}
//
//int CSceneNodePolygonModel::CControlPlane::GetAxis() const{
//	int iAxis = 0;
//	for(int i=1;i<3;++i){
//		if(abs(m_polyModel.m_faces[m_faces[0]].normal[i])>abs(m_polyModel.m_faces[m_faces[0]].normal[iAxis])){
//			iAxis = i;
//		}
//	}
//	return iAxis;
//}
//
//int CSceneNodePolygonModel::CControlPlane::Move(int delta){
//	int iAxis = GetAxis();
//	static std::vector<int> verts;
//	verts.assign(m_verts.begin(),m_verts.end());
//	if(GetSingleton<CProgram>()->GetConfigVar<bool>(L"FixWallThickness")){
//		for(std::vector<CSceneNodeWallDef *>::iterator itr = m_pWallDefs.begin(); itr != m_pWallDefs.end(); ++itr){
//			verts.insert(verts.end(),(*itr)->GetSortedRangeBegin(),(*itr)->GetSortedRangeEnd());
//		}
//	}
//	sort(verts.begin(),verts.end());
//	verts.erase( unique(verts.begin(),verts.end()), verts.end() );
//	//for(std::vector<int>::iterator iiV = m_verts.begin(); iiV!= m_verts.end(); ++iiV){
//	for(std::vector<int>::iterator iiV = verts.begin(); iiV!= verts.end(); ++iiV){
//		m_polyModel.m_verts[*iiV].pos[iAxis] = float(int(m_polyModel.m_verts[*iiV].pos[iAxis])+delta);
//	}
//	return delta;
//}
//
//
//int CSceneNodePolygonModel::CControlPlane::MoveTo(int p){
//	int iAxis = GetAxis();
//	static const vector3 base[3] = { vector3(1,0,0), vector3(0,1,0), vector3(0,0,1) };
//	std::pair<float,float> range = CastRange(base[iAxis]);
//	assert(int(range.first) == int(range.second));
//	return Move(p-int(range.first));
//}
//
//
//std::pair<float,float> CSceneNodePolygonModel::CControlPlane::CastRange( const class vector3 &dir ) const{
//	float d = DotProduct(dir,m_polyModel.m_verts[*m_verts.begin()].pos);
//	std::pair<float,float> ret(d,d);
//	for(std::vector<int>::const_iterator iiV = m_verts.begin(); iiV!= m_verts.end(); ++iiV){
//		d = DotProduct(dir, m_polyModel.m_verts[*iiV].pos);
//		if(d<ret.first){
//			ret.first = d;
//		}else if(d>ret.second){
//			ret.second = d;
//		}
//	}
//	return ret;
//}
//
//
//void CSceneNodePolygonModel::CControlPlane::RegisterWallDef(CSceneNodeWallDef *pWallDef){
//	assert(GetAxis() == pWallDef->GetAxis());
//	if(stdext::set_intersect(pWallDef->GetSortedRangeBegin(),pWallDef->GetSortedRangeEnd(),m_verts.begin(),m_verts.end())){
//		m_pWallDefs.push_back(pWallDef);
//		assert(includes(pWallDef->GetSortedRangeBegin(),pWallDef->GetSortedRangeEnd(),m_verts.begin(),m_verts.end()));
//	}
//}


const vector3 *CSceneNodePolygonModel::GetVertPositionConstPtr(int id){
	return &m_verts[id].pos;
}


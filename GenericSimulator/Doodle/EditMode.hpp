#pragma once

#include <BRepPrimAPI_MakeCone.hxx>
#include <BRepPrimAPI_MakeTorus.hxx>
#include "Shape.hpp"
#include <gce_MakeScale.hxx>
#include <boost/function.hpp>

class EditMode{
public:
	struct GameStatesRef{
		std::vector<SimpleBoxComponent> &boxes;
		std::vector<SimpleBoxComponent> &protoBoxes;
		std::vector<std::size_t> &sbcToDelete;
	};
private:
	GameStatesRef m_gs;

	std::size_t m_hoveringOverBox,m_selectedBox/*,m_draggingBox*/;
	static std::size_t InvalidId(){ return std::numeric_limits<std::size_t>::max(); }

	Ogre::Vector2 m_mouseH;

	Ogre::SceneManager *m_pSceneManager;

	static Standard_Real NearestRayIntersection( const gp_Lin &rayOS, const TopoDS_Shape &s ){
		IntCurvesFace_ShapeIntersector intersector;
		intersector.Load(s,0);
		intersector.Perform/*Nearest*/(rayOS,0,RealLast());
		Standard_Real w = intersector.NbPnt()?intersector.WParameter(1):RealLast();
		for(int i=2;i<=intersector.NbPnt();++i){
			NumericRange::ExtendL( intersector.WParameter(i), w );
		}
		return w;
	}
	static Standard_Real NearestRayIntersection( const gp_Lin &ray, const gp_Trsf &transformToOS, const TopoDS_Shape &s ){
		return NearestRayIntersection(ray.Transformed(transformToOS),s);
	}
	template< typename _PositionVec,typename _OrientationQuat >
	static gp_Trsf GetWS2OSTransform( const _PositionVec &pos, const _OrientationQuat &ort ){
		gp_Trsf rotation;
		rotation.SetRotation( boost::fusion::invoke( boost::value_factory<gp_Quaternion>(), boost::fusion::as_nview<1,2,3,0>(ort) ) );
		return (gce_MakeTranslation( boost::fusion::invoke( boost::value_factory<gp_Vec>(), pos ) ).Value()*rotation).Inverted();
	}

	struct OCCTRayIntersector{
		const gp_Lin rayWS;

		template<typename E>
		typename boost::enable_if<GenericSim::Meta::IsEntity<E>, Standard_Real>::type operator()(const E &e) const{
			return OnEntityByConcept( e, typename GenericSim::Meta::GetEntityConcept<E>::Type() );
		}

		Standard_Real operator()(const boost::fusion::vector< const gp_Trsf &, const TopoDS_Shape& > &p) const{
			using namespace boost::fusion;
			return NearestRayIntersection(rayWS, at_c<0>(p), at_c<1>(p))/at_c<0>(p).ScaleFactor();
		}

	private:

		template<typename E>
		Standard_Real OnEntityByConcept( const E &e, EC::Component ) const{
			return NearestRayIntersection( 
				rayWS,
				GetWS2OSTransform(
					e.at_e<EC::Component::RigidBodyStates>().at_e<EC::RigidBodyDynamics3D::PositionVector>(),
					e.at_e<EC::Component::RigidBodyStates>().at_e<EC::RigidBodyDynamics3D::OrientationQuat>()
				),
				GenericShapeToOCCT(e.at_e<EC::Component::Shape>())
			);
		}
	};


	template< typename _SP_R_Range, typename Intersector >
	static std::pair< Standard_Real, typename boost::range_difference<_SP_R_Range>::type >
	SelectNearest( const _SP_R_Range &r, const Intersector &f, const Standard_Real &upperLimit ){
		boost::range_iterator<const _SP_R_Range>::type pos = boost::end(r);
		Standard_Real m = upperLimit;
		for( boost::range_iterator<const _SP_R_Range>::type i = boost::begin(r); i!=boost::end(r); ++i ){
			Standard_Real d = f(*i);
			if(d<m){
				m = d;
				pos = i;
			}
		}
		return std::make_pair(m,pos-boost::begin(r));
	}
	

	Ogre::SceneNode * m_spatialTransformDraggablesOgreSceneNode;
	std::string m_translationArrowMeshName;
	TopoDS_Shape m_translationArrowOCCTShape;
	boost::array<gp_Trsf,6> m_translationArrowOCCTShapeWS2OSTransforms;
	std::size_t m_hoveringOverTranslationArrow;
	std::size_t m_draggingTranslationArrow;

	std::string m_rotationHandleMeshName;
	TopoDS_Shape m_rotationHandleOCCTShape;
	boost::array<gp_Trsf,6> m_rotationHandleOCCTShapeWS2OSTransforms;
	std::size_t m_hoveringOverRotationHandle;
	std::size_t m_draggingRotationHandle;

	void UpdateTransformDraggablesAuxTransforms(){
		Real3 pos = boost::qvm::make<Real3>(m_gs.boxes[m_selectedBox].at_e<EC::Component::RigidBodyStates>().at_e<EC::RigidBodyDynamics3D::PositionVector>());
		m_spatialTransformDraggablesOgreSceneNode->setPosition(boost::qvm::make<Ogre::Vector3>(pos));
		Real radius = sqrt(GetShapeBoundingSphereRadiusSquared(m_gs.boxes[m_selectedBox].at_e<EC::Component::Shape>()));
		//Ogre::Node::ChildNodeIterator itr = m_spatialTransformDraggablesOgreSceneNode->getChildIterator();
		using namespace boost::qvm;
		{
			boost::array<Real,4> qs[6] = {
				{sqrt(0.5),0,sqrt(0.5),0},
				{-sqrt(0.5),sqrt(0.5),0,0},
				{1,0,0,0},
				{-sqrt(0.5),0,sqrt(0.5),0},
				{sqrt(0.5),sqrt(0.5),0,0},
				{0,1,0,0}
			};
			float r = 1.5*radius;
			Ogre::Node *p = m_spatialTransformDraggablesOgreSceneNode->getChild("T");
			for(int i=0;i<6;++i){
				Real a = i/3?-r:r;
				int iDim = i%3;
				Ogre::Node *pN = p->getChild(i);//itr.getNext();
				pN->setPosition(VectorParallelToCoordSysBase<Ogre::Vector3>(iDim,a));
				pN->setOrientation(make<Ogre::Quaternion>(qs[i]));
				m_translationArrowOCCTShapeWS2OSTransforms[i] = GetWS2OSTransform(VectorParallelToCoordSysBase<Real3>(iDim,a)+pos,qs[i]);
			}
		}
		{
			boost::array<Real,4> qa[3] = {
				{-sqrt(0.5),0,sqrt(0.5),0},
				{sqrt(0.5),sqrt(0.5),0,0},
				{1,0,0,0},
			};
			boost::array<Real,4> qb[2] = {
				{1,0,0,0},
				{0,-sqrt(0.5),sqrt(0.5),0}
			};
			boost::array<Real,4> qc = {	cos(PI/12),0,0,sin(PI/12) };
			float r = 1.5*radius;
			Ogre::Node *p = m_spatialTransformDraggablesOgreSceneNode->getChild("R");
			p->setScale( r,r,r );
			for(int i=0;i<6;++i){
				int iDim = i%3;
				int iDir = i/3;
				//Real3 t = zero_v<Real,3>();
				//t[(iDim+2)%3] = t[(iDim+1)%3] = iDir ? -1:1;
				Ogre::Node *pN = p->getChild(i);
				//pN->setPosition(make<Ogre::Vector3>(t));
				boost::array<Real,4> q = qa[iDim]*qb[iDir]*qc;
				//normalize(q);
				pN->setOrientation(make<Ogre::Quaternion>(q));
				m_rotationHandleOCCTShapeWS2OSTransforms[i] = gce_MakeScale(gp_Pnt(),1/r).Value()*GetWS2OSTransform(pos,q);
			}
		}
	}

	template<typename F>
	void CallOnSelectedComponent( F &f ){
		if(InvalidId()!=m_selectedBox) f(m_gs.boxes[m_selectedBox]);
		else assert(0);
	}

	struct ToGetComponentPos{ Real3 ret;	template< typename C > void operator()( const C &c ){
		boost::qvm::assign(ret,c.at_e<EC::Component::RigidBodyStates>().at_e<EC::RigidBodyDynamics3D::PositionVector>());
	}};

	struct ToSetComponentPos{ const Real3 &v;	template< typename C > void operator()( C &c ) const{
		boost::qvm::assign(c.at_e<EC::Component::RigidBodyStates>().at_e<EC::RigidBodyDynamics3D::PositionVector>(),v);
	}};
	
	struct ToGetComponentOrientation{ Quaternion ret;	template< typename C > void operator()( const C &c ){
		boost::qvm::assign(boost::qvm::qref(ret),c.at_e<EC::Component::RigidBodyStates>().at_e<EC::RigidBodyDynamics3D::OrientationQuat>());
	}};

	struct ToSetComponentOrientation{ const Quaternion &q;	template< typename C > void operator()( C &c ) const{
		boost::qvm::assign(c.at_e<EC::Component::RigidBodyStates>().at_e<EC::RigidBodyDynamics3D::OrientationQuat>(),boost::qvm::qref(q));
	}};
	
	

	Real3 GetSelectedComponentPos(){
		ToGetComponentPos f;
		CallOnSelectedComponent(f);
		return f.ret;
	}
	void SetSelectedComponentPos(const Real3 &pos){
		ToSetComponentPos f = {pos};
		CallOnSelectedComponent(f);
	}
	Quaternion GetSelectedComponentOrientation(){
		ToGetComponentOrientation f;
		CallOnSelectedComponent(f);
		return f.ret;
	}
	void SetSelectedComponentOrientation(const Quaternion &q){
		ToSetComponentOrientation f = {q};
		CallOnSelectedComponent(f);
	}
	//void UpdateTransformDraggablesOgreSceneNodeSubNodes(){
	//}

	bool m_primaryPointerDown, m_secondaryPointerDown;

	template< typename Intersector >
	struct IntersectionSelector{
		const Intersector intersector;
		Standard_Real nearest;
		std::size_t elementId;
		std::size_t *pToSet;
		template< typename _SP_R_Range >
		void operator()(const _SP_R_Range &r, std::size_t *pRangeResult){
			std::pair< Standard_Real, std::size_t > s = SelectNearest( r,intersector,nearest );
			if(s.first < nearest){
				pToSet = pRangeResult;
				nearest = s.first;
				elementId = s.second;
			}
		}
	};

	bool HasSelectedSomething(){ return InvalidId()!=m_selectedBox; }
	
	static const Ogre::Vector3 ComputeOgreCameraRay(Ogre::Vector2 posH,Ogre::Camera *pCam)
	{
		const Ogre::Matrix4 mp = pCam->getProjectionMatrix();
		const Ogre::Matrix4 m = (mp*pCam->getViewMatrix()).inverse();
		Ogre::Vector4 v = m*Ogre::Vector4( posH.x, posH.y, -mp[2][2], 1 );
		return Ogre::Vector3(v.x,v.y,v.z);
	}
	void Deselect(){
		m_hoveringOverBox = m_selectedBox = InvalidId();
		m_hoveringOverTranslationArrow = m_draggingTranslationArrow = InvalidId();
		m_hoveringOverRotationHandle = m_draggingRotationHandle = InvalidId();
		if(m_spatialTransformDraggablesOgreSceneNode)	m_spatialTransformDraggablesOgreSceneNode->setVisible(false);
	}

	template< typename _UnaryFunc >
	struct LinearDraggableFunc{
		const _UnaryFunc f;
		//const Real3 rail; Just use (0,0,1)
		void operator()(const Real3 &ray0, const Real3 &ray1, const Real3 &eye, const Real3 &camAxis) const{// all in object space
			using namespace boost::qvm;
			const Real3 rail = {0,0,1};
			Real3 normal = cross(rail,cross(camAxis,rail));
			Real signedDist = dot(-eye,normal);
			Real k1 = signedDist/dot(ray1,normal)*dot(ray1,rail);
			Real k0 = signedDist/dot(ray0,normal)*dot(ray0,rail);
			f(k1-k0);
		}
	};

	template< typename _UnaryFunc >
	struct RotaryDraggableFunc{
		_UnaryFunc f;
		void operator()(const Real3 &ray0, const Real3 &ray1, const Real3 &eye, const Real3 &camAxis)const{// all in object space
			using namespace boost::qvm;
			unsigned i = 2, j = 0, k = 1;
			Real3 d = -eye;
			Real r1 = d[i]/ray1[i];
			Real r0 = d[i]/ray0[i];
			Real2 s1 = { -d[j]+ray1[j]*r1, -d[k]+ray1[k]*r1 };
			Real2 s0 = { -d[j]+ray0[j]*r0, -d[k]+ray0[k]*r0 };
			Real x = dot(s0,s1);
			Real y = -s0[1]*s1[0]+s0[0]*s1[1];
			Real a = atan2( y, x )*0.5;
			f(a);
		}
	};

	template< typename _HandleToScalar >
	struct Simple1DDragInterpretationFunc{
		_HandleToScalar h;
		void operator() (const Real &delta) const {	*h+=delta;	}
	};

	struct Parameterizer{
		boost::function<void (const Real3 &,const Real3 &,const Real3 &,const Real3 &)> f;
		boost::function<SpatialTransform ()> s;
	};

	std::vector<Parameterizer> m_parameterizers;


	template<typename _ParameterizerContainer>
	struct ToAttachParameterizer{
		//typedef boost::function<void (const Real3 &,const Real3 &,const Real3 &,const Real3 &)> result_type;
		_ParameterizerContainer &cont;
		template<typename H>
		void operator()( const H &h ){
			cont.clear();
			Add(h);
		}
	private:
		template<typename _HandleToSimpleBoxComponent>
		void Add( const _HandleToSimpleBoxComponent &pSbc){
			using Utility::Make;
			using Utility::MakeAggr;
			using namespace GenericSim::Handles;
			typedef HandleToEntityState<_HandleToSimpleBoxComponent, EC::Component::Shape> H0;
			typedef HandleToEntityState< H0, EC::Box::Size > H1;
			H1 hs( (H0(pSbc)) ); //additional paren to avoid parsing as func
			struct STF{
				H1 hs;
				int iAxis;
				SpatialTransform operator()(){
					return SpatialTransform();
				}
			};
			for(int i=0;i<3;++i){
				cont.push_back( MakeAggr<Parameterizer>(
					MakeAggr<LinearDraggableFunc>(
						MakeAggr<Simple1DDragInterpretationFunc>(
							//&sbc.at_e<EC::Component::Shape>().at_e<EC::Box::Size>()[i]
							Make<HandleToElementOfRandomAccessRange>( hs, i )
						)
					),
					MakeAggr<STF>(hs,i)
				));
			}
		}
	};

public:
	EditMode(const GameStatesRef &gs):m_gs(gs),
		m_hoveringOverBox(InvalidId()),m_selectedBox(InvalidId()),/*m_draggingBox(InvalidId()),*/
//			m_hoveringOverRotationHandle(InvalidId()),
		m_pSceneManager(NULL),m_primaryPointerDown(false), m_secondaryPointerDown(false),
		m_spatialTransformDraggablesOgreSceneNode(NULL),
		m_translationArrowOCCTShape(BRepPrimAPI_MakeCone(0.1,0.02,0.5)),
		m_translationArrowMeshName("translationArrowOgreMeshH2J2DFUFZK91417PNGA"+boost::lexical_cast<std::string>(clock())),
		m_hoveringOverTranslationArrow(InvalidId()),
		m_draggingTranslationArrow(InvalidId()),
		m_rotationHandleOCCTShape(/*BRepPrimAPI_MakeSphere(0.2)),*/BRepPrimAPI_MakeTorus(1,0.05,PI/6)),
		m_rotationHandleMeshName("rotationHandleOgreMeshN94T2U245SRT7STYS8"+boost::lexical_cast<std::string>(clock())),
		m_hoveringOverRotationHandle(InvalidId()),
		m_draggingRotationHandle(InvalidId())
	{
	}
	void OnPrimaryPointerDown(){
		if(m_primaryPointerDown) return;
		m_primaryPointerDown = true;
		std::cout<<"d\n";
//			if(InvalidId()!=m_hoveringOverRotationHandle){
//			}else if(InvalidId()!=m_hoveringOverTranslationAxisHandle){
//			}else 
		if(InvalidId()!=m_hoveringOverBox){
			m_selectedBox = m_hoveringOverBox;

			if(!m_spatialTransformDraggablesOgreSceneNode){
				m_spatialTransformDraggablesOgreSceneNode = m_pSceneManager->getRootSceneNode()->createChildSceneNode();
				Ogre::SceneNode *pT = m_spatialTransformDraggablesOgreSceneNode->createChildSceneNode("T");
				for(int i=0;i<6;++i){
					if(!Ogre::MeshManager::getSingleton().resourceExists(m_translationArrowMeshName)){
						OCCTMesh2OGRE::FillBlankOgreMeshUsingOCCTShape(
							Ogre::MeshManager::getSingleton().createManual( m_translationArrowMeshName , "__OCCTGEN" ),
							m_translationArrowOCCTShape,
							1
						);
					}
					pT->createChildSceneNode()->attachObject(m_pSceneManager->createEntity( 
						"translationArrowOgreEntitiyNG92HY20JQY95R"+boost::lexical_cast<std::string>(clock())+"|"+boost::lexical_cast<std::string>(unsigned(this)+i),
						m_translationArrowMeshName,
						"__OCCTGEN"
					));
				}
				Ogre::SceneNode *pR = m_spatialTransformDraggablesOgreSceneNode->createChildSceneNode("R");
				for(int i=0;i<6;++i){
					if(!Ogre::MeshManager::getSingleton().resourceExists(m_rotationHandleMeshName)){
						OCCTMesh2OGRE::FillBlankOgreMeshUsingOCCTShape(
							Ogre::MeshManager::getSingleton().createManual( m_rotationHandleMeshName , "__OCCTGEN" ),
							m_rotationHandleOCCTShape,
							0.1
						);
					}
					pR->createChildSceneNode()->attachObject(m_pSceneManager->createEntity(
						"rotationHandleOgreEntityYH220JS0OR2WNSDF"+boost::lexical_cast<std::string>(clock())+"|"+boost::lexical_cast<std::string>(unsigned(this)+i),
						m_rotationHandleMeshName,
						"__OCCTGEN"
					));
				}
			}
			m_spatialTransformDraggablesOgreSceneNode->setVisible(true);
			UpdateTransformDraggablesAuxTransforms();
			using Utility::Make;
			using Utility::MakeAggr;
			using Utility::TieIntoAggr;
			using namespace GenericSim::Handles;
			TieIntoAggr<ToAttachParameterizer>(m_parameterizers)(Make<HandleToElementOfRandomAccessRange>(&m_gs.boxes,m_selectedBox));
		}else if(InvalidId()!=m_hoveringOverTranslationArrow){
			m_draggingTranslationArrow = m_hoveringOverTranslationArrow;
		}else if(InvalidId()!=m_hoveringOverRotationHandle){
			m_draggingRotationHandle = m_hoveringOverRotationHandle;
		}else{
			Deselect();
		}
	}
	void OnPrimaryPointerUp(){
		if(!m_primaryPointerDown) return;
		m_primaryPointerDown = false;
		std::cout<<"u\n";
		m_draggingTranslationArrow = InvalidId();
		m_draggingRotationHandle = InvalidId();
		//m_draggingBox = InvalidId();
	}
	void OnSecondaryPointerDown(){
		m_secondaryPointerDown = true;
	}
	void OnSecondaryPointerUp(){
		m_secondaryPointerDown = false;
	}
	void DeleteSelected(){
		if(m_selectedBox!=InvalidId()){
			m_gs.sbcToDelete.push_back(m_selectedBox);
			Deselect();
		}
	}
	void SetOgreScene(Ogre::SceneManager *pSceneManager){
		if(m_pSceneManager){
			if(m_spatialTransformDraggablesOgreSceneNode){
				DestroyOgreSceneNodeAttachedMovableObjects(m_spatialTransformDraggablesOgreSceneNode,m_pSceneManager);
				m_pSceneManager->destroySceneNode(m_spatialTransformDraggablesOgreSceneNode); 
				m_spatialTransformDraggablesOgreSceneNode = NULL;
			}
		}
		m_pSceneManager = pSceneManager;
	}

	void Update(Real elapsedSeconds,const Ogre::Vector2 &mouseH,Ogre::Camera *pCam)
	{
		if(HasSelectedSomething()){
			assert(m_pSceneManager);
			UpdateTransformDraggablesAuxTransforms();
		}
		if(InvalidId()!=m_draggingTranslationArrow||InvalidId()!=m_draggingRotationHandle){
			using namespace boost::qvm;
			Real3 eye = make<Real3>(pCam->getRealPosition());
			Real3 ray1 = make<Real3>(ComputeOgreCameraRay(mouseH,pCam));
			Real3 ray0 = make<Real3>(ComputeOgreCameraRay(m_mouseH,pCam));
			Real3 pos = GetSelectedComponentPos();
			Quaternion ort = GetSelectedComponentOrientation();
			if(InvalidId()!=m_draggingTranslationArrow){
				//Ogre::Vector2 dMouseH = mouseH - m_mouseH;
				Real3 cam = make<Real3>(pCam->getRealDirection().normalisedCopy());
				Real3 rail = VectorParallelToCoordSysBase<Real3>(m_draggingTranslationArrow%3,1);
				Real3 normal = cross(rail,cross(cam,rail));
				Real3 eye = make<Real3>(pCam->getRealPosition());
				Real signedDist = dot(pos-eye,normal);
				Real k1 = signedDist/dot(ray1,normal)*dot(ray1,rail);
				Real k0 = signedDist/dot(ray0,normal)*dot(ray0,rail);
				pos[m_draggingTranslationArrow%3]+=k1-k0;
				SetSelectedComponentPos(pos);
			}else if(InvalidId()!=m_draggingRotationHandle){
				unsigned i = m_draggingRotationHandle%3, j = (m_draggingRotationHandle+1)%3, k = (m_draggingRotationHandle+2)%3;
				Real3 d = pos-eye;
				Real r1 = d[i]/ray1[i];
				Real r0 = d[i]/ray0[i];
				Real2 s1 = { -d[j]+ray1[j]*r1, -d[k]+ray1[k]*r1 };
				Real2 s0 = { -d[j]+ray0[j]*r0, -d[k]+ray0[k]*r0 };
				Real x = dot(s0,s1);
				Real y = -s0[1]*s1[0]+s0[0]*s1[1];
				Real a = atan2( y, x )*0.5;
//					Real t[] = { ::atan2( 1,0 ), ::atan2(2,0), ::atan2(-1,0) };
				Real4 q = { cos(a),0,0,0 };
				q[1+i] = sin(a);
				SetSelectedComponentOrientation(normalized(qref(q*ort)));
			}
		}
		if(m_primaryPointerDown){
		}else{
			Ogre::Vector3 dir = ComputeOgreCameraRay(mouseH,pCam);
			Ogre::Vector3 pos = pCam->getRealPosition();
			gp_Lin rayWS( gp_Pnt(pos.x,pos.y,pos.z), gp_Dir(dir.x,dir.y,dir.z) );

			IntersectionSelector<OCCTRayIntersector> is = {
				{rayWS},
				RealLast(),
				InvalidId(),
				NULL
			};
			m_hoveringOverBox = InvalidId();
			m_hoveringOverTranslationArrow = InvalidId();
			m_hoveringOverRotationHandle = InvalidId();
			
			is(m_gs.boxes,&m_hoveringOverBox);
			if(HasSelectedSomething()){
				is(boost::adaptors::transform(
					m_translationArrowOCCTShapeWS2OSTransforms,
					boost::bind( boost::value_factory< boost::fusion::vector< const gp_Trsf &, const TopoDS_Shape& > >(), _1, m_translationArrowOCCTShape )
				), &m_hoveringOverTranslationArrow);
				is(boost::adaptors::transform(
					m_rotationHandleOCCTShapeWS2OSTransforms,
					boost::bind( boost::value_factory< boost::fusion::vector< const gp_Trsf &, const TopoDS_Shape& > >(), _1, m_rotationHandleOCCTShape )
				), &m_hoveringOverRotationHandle);
			}
			
			if(is.pToSet)	*is.pToSet = is.elementId;
			std::cout<<is.elementId<<' '<<is.nearest<<'\n';
		}
		m_mouseH = mouseH;
	}
};

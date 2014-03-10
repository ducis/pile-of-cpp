
#define FUSION_MAX_VECTOR_SIZE 16

#define BOOST_FUSION_INVOKE_MAX_ARITY 16
#define BOOST_FUSION_INVOKE_PROCEDURE_MAX_ARITY 16
#define BOOST_FUSION_INVOKE_FUNCTION_OBJECT_MAX_ARITY 16
#define BOOST_FUSION_UNFUSED_MAX_ARITY 16
#define BOOST_FUSION_UNFUSED_TYPE_MAX_ARITY 16

#include <iostream>

#include <boost/fusion/include/boost_array.hpp>
#include <entity_as_boost_fusion_sequence.hpp>

#include <core/ENTITY_CONCEPT.hpp>

#include <meta/plaintuple.hpp>
#include <entities/ConceptedRASeq.hpp>
#include <meta/format.hpp>
#include <string>


#define dDOUBLE
#include <ode/ode.h>
#include <ode/odedirect.h>
#include <drawstuff/drawstuff.h>

#include <plainQVM.hpp>
#include <boost_array_as_boost_qvm.hpp>

#include <Ogre.h>
#include <CEGUI.h>
#include <RendererModules/Ogre/CEGUIOgreRenderer.h>
#include "OISRouter.hpp"
#include <map>


namespace EntityConcepts{
	GENSIM_ENTITY_CONCEPT(			RigidBodyDynamics3D,
		PositionVector,
		OrientationQuat,
		LinearVelocityVector,
		AngularVelocityVector
		)

	//This more generic version is reserved for future use
	//GENSIM_ENTITY_CONCEPT(			RigidBodyDynamics,
	//	Position,
	//	Orientation,
	//	LinearVelocity,
	//	AngularVelocity
	//	)

	//discard PR and PU joints (asymmetry)
	//discard Hinge-2 (same as universal)
	//discard Fixed
	//GENSIM_ENTITY_CONCEPT(			JointAttachment,
	//	BodyIdentifier,
	//	SpatialConfiguration
	//)

	GENSIM_ENTITY_CONCEPT(			Joint,
		AttachedObjectIdentifierPair,
		SpatialConfigurationPair
	)


	//namespace JointAttachmentGeometryParam{
	//	GENSIM_ENTITY_CONCEPT(			Ball,
	//		AnchorPosition,
	//	)

	//	GENSIM_ENTITY_CONCEPT(			Hinge,
	//		AnchorPosition,
	//		AxisDirectionVector
	//	)

	//	GENSIM_ENTITY_CONCEPT(			Slider,
	//		AnchorPosition,
	//		AnchorOrientationQuat
	//	)

	//	GENSIM_ENTITY_CONCEPT(			Universal,
	//		AnchorPosition,
	//		AxisDirectionVector
	//	)

	//	GENSIM_ENTITY_CONCEPT(			Piston,
	//		AnchorPosition,
	//		AxisDirectionVector
	//	)
	//}


	GENSIM_ENTITY_CONCEPT(			Component,
		GameplayStates,
		Shape,
		MassDistribution,
		RigidBodyStates
		)

	//GENSIM_ENTITY_CONCEPT(			SeparableComponentParameter,
	//	GameplayStates,
	//	Shape,
	//	MassDistribution
	//	)

	GENSIM_ENTITY_CONCEPT(			Cylinder,
		Length,
		Radius
		)

	GENSIM_ENTITY_CONCEPT(			Box,
		Size
		)

	GENSIM_ENTITY_CONCEPT(			Sphere,
		Radius
		)

	GENSIM_ENTITY_CONCEPT(			ConstantDensityMassDistribution,
		Density
		)

	GENSIM_ENTITY_CONCEPT(			SpatialConfiguration,
		PositionVector,
		OrientationQuat
		)

	GENSIM_ENTITY_CONCEPT(			SpatialTransform,
		Translation,
		Rotation,
		Scale
		)

	GENSIM_ENTITY_CONCEPT(			GeometryInstance,
		Geometry,
		SpatialConfiguration
		)

	//Assume that orientation is always quat for now.
	//GENSIM_ENTITY_CONCEPT(			Orientation,
	//	Quaternion,
	//	LinearTransformMatrix,
	//	AffineTransformMatrix,
	//	EularAngle
	//	)
}

struct EmptyStruct{};

namespace EC = EntityConcepts;


typedef dReal Real;

typedef boost::array<Real,2> Real2;
typedef boost::array<Real,3> Real3;
typedef boost::array<Real,4> Real4;
typedef MakeSquarePlainMatrix<Real,4,false>::Type Real44;
typedef MakeSquarePlainMatrix<Real,3,false>::Type Real33;
typedef PlainMatrix<Real,4,3,false> Real43;
typedef boost::array<Real,4> Quaternion;

typedef Real Scalar;
typedef Real2 Vector2;
typedef Real3 Vector3;
typedef Real4 Vector4;
typedef Real44 Matrix44;
typedef Real43 Matrix43;

typedef Vector3 Vector;
typedef Matrix44 AffineMatrix;

//
typedef Vector3 PositionVector;
typedef Quaternion OrientationQuat;
typedef GenericSim::Meta::MakePlainTuple< EC::RigidBodyDynamics3D, PositionVector, OrientationQuat, Vector, Vector >::Type RigidBodyInst;
typedef GenericSim::Meta::MakePlainTuple< EC::SpatialConfiguration, PositionVector, OrientationQuat >::Type SpatialConfiguration;
//typedef GenericSim::Meta::MakePlainTuple< EC::Component,int,Box,
typedef GenericSim::Meta::MakePlainTuple< EC::ConstantDensityMassDistribution, Real >::Type ConstantDensityMassDistribution;
typedef GenericSim::Meta::MakePlainTuple< EC::SpatialTransform, PositionVector, OrientationQuat, Real >::Type SpatialTransform;
template< typename _Shape > struct MakeSimpleComponent
{
	typedef typename GenericSim::Meta::MakePlainTuple< EC::Component, int, _Shape, ConstantDensityMassDistribution, RigidBodyInst >::Type Type;
};
//typedef boost::array< SpatialConfiguration, 2 > JointAttachmentPair;

typedef int ShortIdentifier;

//typedef GenericSim::Meta::MakePlainTuple< EC::Joint, JointAttachmentPair>::Type BallJoint;
typedef boost::array< ShortIdentifier, 2 > JointAttachedObjectIdentifierPair;
typedef boost::array< SpatialConfiguration, 2> JointSpatialConfigurationPair;
typedef GenericSim::Meta::MakePlainTuple< EC::Joint, JointAttachedObjectIdentifierPair, JointSpatialConfigurationPair >::Type BallJoint;

#include "utility/PairedCall.hpp"

#include <vector>

template< class _EntityConcept, BOOST_PP_ENUM_PARAMS_WITH_A_DEFAULT( GENSIM_PLAIN_TUPLE_MAX_SIZE, class _T, void ) > 
struct MakeRenderListItem{
	typedef typename GenericSim::Meta::MakePlainTuple< _EntityConcept, BOOST_PP_ENUM_PARAMS( GENSIM_PLAIN_TUPLE_MAX_SIZE, _T ) >::Type Type;
};

typedef GenericSim::Meta::MakePlainTuple< EC::Cylinder, Scalar, Scalar >::Type Cylinder;
typedef GenericSim::Meta::MakePlainTuple< EC::Box, Vector >::Type Box;
typedef GenericSim::Meta::MakePlainTuple< EC::Sphere, Scalar >::Type Sphere;

//typedef MakeSimpleComponent<Cylinder>::Type SimpleCylinderComponent;
typedef MakeSimpleComponent<Box>::Type SimpleBoxComponent;
//typedef MakeSimpleComponent<Sphere>::Type SimpleSphereComponent;

static dWorldID world;
static dSpaceID space;
//static dBodyID body;
//static dGeomID geom;
//static dMass m;
static dJointGroupID contactgroup;


static void nearCallback (void *data, dGeomID o1, dGeomID o2)
{
    dBodyID b1 = dGeomGetBody(o1);
    dBodyID b2 = dGeomGetBody(o2);
    dContact contact;
    contact.surface.mode = dContactBounce | dContactSoftCFM;
    // friction parameter
    contact.surface.mu = dInfinity;
    // bounce is the amount of "bouncyness".
    contact.surface.bounce = 0.9;
    // bounce_vel is the minimum incoming velocity to cause a bounce
    contact.surface.bounce_vel = 0.01;
    // constraint force mixing parameter
    contact.surface.soft_cfm = 0.001;
    if (int numc = dCollide (o1,o2,1,&contact.geom,sizeof(dContact))) {
        dJointID c = dJointCreateContact (world,contactgroup,&contact);
        dJointAttach (c,b1,b2);
    }
	//std::cout<<"nearCallack\n";
}

static void start()
{
    static float xyz[3] = {2.0f,-2.0f,1.7600f};
    static float hpr[3] = {140.000f,-17.0000f,0.0000f};
    dsSetViewpoint (xyz,hpr);
}

#include <algorithm>
#include <short_array.hpp>
#include <boost/utility/enable_if.hpp>
#include <boost/type_traits/is_same.hpp>

static void simLoop (int pause)
{
    dSpaceCollide (space,0,&nearCallback);
    dWorldStep (world,0.01);
    dJointGroupEmpty (contactgroup);
}


template< int N, typename T >
T (&PointerAsCArray( T *p ))[N]{
	return 
		static_cast<T (&)[N]>(
			*static_cast<T(*)[N]>(
				static_cast<typename boost::mpl::if_<boost::is_const<T>,const void *,void*>::type>(p)
			)
		);
}

#include <boost/qvm/v_traits_array.hpp>
#include <boost/qvm/sw.hpp>

template<typename _SpatialConfigurationPair>
void SetOpenDEBallJointSpatialConfiguration( const _SpatialConfigurationPair &c, dxJointBall *p ){
	using namespace boost::qvm;
	using namespace boost::fusion;
	boost::qvm::assign( p->anchor1,	at_c<0>(c).at_e<EC::SpatialConfiguration::PositionVector>()%XYZ0() );
	boost::qvm::assign( p->anchor2, at_c<1>(c).at_e<EC::SpatialConfiguration::PositionVector>()%XYZ0() );
	//p->anchor1[3]=p->anchor2[3]=0;
}




#include <boost/foreach.hpp>
#include <boost/range.hpp>
#include <boost/fusion/functional/invocation/invoke_procedure.hpp>
#include <boost/fusion/include/push_front.hpp>
#include <boost/fusion/include/array.hpp>
#include <boost/qvm/qvm.hpp>
#include <boost/iterator/zip_iterator.hpp>
#include <boost/tuple/tuple.hpp>
#include <boost/fusion/adapted/boost_tuple.hpp>
#include <boost/fusion/include/make_fused.hpp>
#include <boost/fusion/include/at.hpp>
#include <boost/range/algorithm/equal.hpp>
#include <boost/range/adaptor/transformed.hpp>
#include <boost/range/irange.hpp>
#include <boost/lambda/lambda.hpp>
#include <boost/range/algorithm/transform.hpp>
#include <iterator>
#include <boost/qvm/v_traits_array.hpp>
#include <boost/range/metafunctions.hpp> 
#include <WRAP_TMP_PROC.hpp>
#include <boost/fusion/include/boost_array.hpp>

template< typename _RigidBody3DEntity > 
typename boost::enable_if_c
<
	GenericSim::Meta::IsEntity<_RigidBody3DEntity>::value 
	&& boost::is_same<typename _RigidBody3DEntity::entity_concept_type, EC::RigidBodyDynamics3D>::value
>::type
SetOpenDERigidBody( const _RigidBody3DEntity &body, dBodyID odeBodyID )
{
	//using namespace boost::qvm;
	//dBodySetPosition
	//(
	//	*itrOdeBody, 
	//	itrBody->at_e<EC::RigidBodyDynamics3D::PositionVector>()%X,
	//	itrBody->at_e<EC::RigidBodyDynamics3D::PositionVector>()%Y,
	//	itrBody->at_e<EC::RigidBodyDynamics3D::PositionVector>()%Z
	//);
	boost::fusion::invoke_procedure
	( 
		dBodySetPosition, 
		boost::fusion::push_front( body.at_e<EC::RigidBodyDynamics3D::PositionVector>(), odeBodyID )
	);
	dQuaternion q;
	boost::qvm::assign( q,body.at_e<EC::RigidBodyDynamics3D::OrientationQuat>() );
	//dBodySetQuaternion( odeBodyID, boost::qvm::make<dQuaternion>(body.at_e<EC::RigidBodyDynamics3D::OrientationQuat>()) );
	dBodySetQuaternion( odeBodyID, q );
	boost::fusion::invoke_procedure
	(
		dBodySetLinearVel,
		boost::fusion::push_front( body.at_e<EC::RigidBodyDynamics3D::LinearVelocityVector>(), odeBodyID )
	);
	boost::fusion::invoke_procedure
	(
		dBodySetAngularVel,
		boost::fusion::push_front( body.at_e<EC::RigidBodyDynamics3D::AngularVelocityVector>(), odeBodyID )
	);
}
WRAP_TMP_PROC( 2, SetOpenDERigidBody );

template< typename _RigidBody3DEntity >
typename boost::enable_if_c
<
	GenericSim::Meta::IsEntity<_RigidBody3DEntity>::value
	&& boost::is_same<typename _RigidBody3DEntity::entity_concept_type, EC::RigidBodyDynamics3D>::value
>::type
GetOpenDERigidBody( _RigidBody3DEntity &body, dBodyID odeBodyID )
{
	//exit(0);
	//typedef const dReal r3[3];
	//TODO: change to fusion algorithm   assign() and make()
	boost::qvm::assign( body.at_e<EC::RigidBodyDynamics3D::PositionVector>(), PointerAsCArray<3>(dBodyGetPosition(odeBodyID)) );
	boost::qvm::assign(	body.at_e<EC::RigidBodyDynamics3D::OrientationQuat>(), PointerAsCArray<4>(dBodyGetQuaternion(odeBodyID)) );
	boost::qvm::assign( body.at_e<EC::RigidBodyDynamics3D::LinearVelocityVector>(), PointerAsCArray<3>(dBodyGetLinearVel(odeBodyID)) );
	boost::qvm::assign( body.at_e<EC::RigidBodyDynamics3D::AngularVelocityVector>(), PointerAsCArray<3>(dBodyGetAngularVel(odeBodyID)) );
}
WRAP_TMP_PROC( 2, GetOpenDERigidBody );



template<
	typename _Deletion_SinglePassReadableRange,
	typename _Moving_SinglePassReadableRange,
	typename _Mapping_AssociativeContainer,
	typename _ValuePredestructionProc
>
void MappingFollowDeletionAndMoving(
	const _Deletion_SinglePassReadableRange &deletionList,
	const _Moving_SinglePassReadableRange &movingList,
	_Mapping_AssociativeContainer &map,
	_ValuePredestructionProc f
){
	using namespace boost::fusion;
	typedef typename _Mapping_AssociativeContainer::iterator itr_type;
	BOOST_FOREACH( typename boost::range_reference<const _Deletion_SinglePassReadableRange>::type p, deletionList )
	{
		assert(0);// simple deletion is not used for now.
		itr_type i = map.find(p);
		assert(i!=map.end());
		f(i->second);
		map.erase(i);
	}
	BOOST_FOREACH( typename boost::range_reference<const _Moving_SinglePassReadableRange>::type p, movingList )
	{
		using namespace boost::fusion;
		itr_type from = map.find(at_c<0>(p));
		assert(from!=map.end());
		if( (!map.key_comp()(at_c<0>(p),at_c<1>(p))) &&
			(!map.key_comp()(at_c<1>(p),at_c<0>(p)))
		){
			f(from->second);
		}else{
			itr_type to = map.lower_bound(at_c<1>(p));
			assert(from!=to);
			if( map.end()!=to && !map.key_comp()(at_c<1>(p),to->first) ){
				f(to->second);
				to->second = from->second;
			}else{
				assert(0);//This should not happen for now.
				map.insert(to,std::make_pair(at_c<1>(p),from->second));
			}
		}
		map.erase(from);
	}
}

void DestroyODEBody(dBodyID b){
	assert(0==dBodyGetNumJoints(b));
	dBodyDestroy(b);
}

template<
	typename _RigidBodyInstHandleDeletion_SinglePassReadableRange,
	typename _RigidBodyInstHandleMoving_SinglePassReadableRange,
	typename _RigidBodyInstHandleInsertion_SinglePassReadableRange,
	typename _BallJointHandleDeletion_SinglePassReadableRange,
	typename _BallJointHandleMoving_SinglePassReadableRange,
	typename _BallJointHandleInsertion3tuple_SinglePassReadableRange,
	typename _RigidBodyInstHandleODEMapping_AssociativeContainer,
	typename _BallJointHandleODEMapping_AssociativeContainer
>
void ManageODEObjectLifetime(
	const _RigidBodyInstHandleDeletion_SinglePassReadableRange		&rigidBodyDeletionList,
	const _RigidBodyInstHandleMoving_SinglePassReadableRange		&rigidBodyMovingList,
	const _RigidBodyInstHandleInsertion_SinglePassReadableRange		&rigidBodyInsertionList,
	const _BallJointHandleDeletion_SinglePassReadableRange			&ballJointDeletionList,
	const _BallJointHandleMoving_SinglePassReadableRange			&ballJointMovingList,
	const _BallJointHandleInsertion3tuple_SinglePassReadableRange	&ballJointInsertionList,
	_RigidBodyInstHandleODEMapping_AssociativeContainer		&odeRigidBodies,
	_BallJointHandleODEMapping_AssociativeContainer			&odeBallJoints,
	dWorldID	odeWorld
)
{
	using namespace boost::fusion;
	typedef typename _RigidBodyInstHandleODEMapping_AssociativeContainer::iterator itrRigidBody;
	MappingFollowDeletionAndMoving(ballJointDeletionList,ballJointMovingList,odeBallJoints,&dJointDestroy);
	MappingFollowDeletionAndMoving(rigidBodyDeletionList,rigidBodyMovingList,odeRigidBodies,&DestroyODEBody);
	BOOST_FOREACH( typename boost::range_reference<const _RigidBodyInstHandleInsertion_SinglePassReadableRange>::type p, rigidBodyInsertionList )
	{
		assert( odeRigidBodies.find(p)==odeRigidBodies.end() );
		odeRigidBodies.insert(std::make_pair(p,dBodyCreate(odeWorld)));
	}
	BOOST_FOREACH( typename boost::range_reference< const _BallJointHandleInsertion3tuple_SinglePassReadableRange >::type p, ballJointInsertionList )
	{
		dJointID j = dJointCreateBall(odeWorld,0);
		odeBallJoints.insert(std::make_pair(at_c<0>(p),j));
		dJointAttach(j, odeRigidBodies.find(at_c<1>(p))->second, odeRigidBodies.find(at_c<2>(p))->second);
		SetOpenDEBallJointSpatialConfiguration( at_c<0>(p)->at_e<EC::Joint::SpatialConfigurationPair>() , dynamic_cast<dxJointBall*>(j) );
	}
}

template<typename _ODEStepProc>
class ODEWorldVariableStrideStep:public std::binary_function<dWorldID,dReal,void>,boost::noncopyable
{
	_ODEStepProc m_odeStepProc;
	dReal m_fixedODEStepSize;
	dReal m_remainingTime;
	dReal m_stopCriteria;
public:
	void operator()( dWorldID world, dReal deltaTime )
	{
		m_remainingTime+=deltaTime;
		while(m_remainingTime>m_stopCriteria){
			m_odeStepProc(world, m_fixedODEStepSize);
			m_remainingTime-=m_fixedODEStepSize;
		}
	}
	ODEWorldVariableStrideStep(_ODEStepProc odeProc, dReal fixedStep, dReal stopRatio): 
		m_odeStepProc(odeProc), 
		m_fixedODEStepSize(fixedStep), 
		m_remainingTime(0),
		m_stopCriteria(stopRatio*fixedStep)
	{}
};


template< class F, class _Pair >
void CallHandleToObjectMappingPairProc( const F &f, const _Pair &p ){
	f( *p.first, p.second );
}
WRAP_TMP_PROC( 2, CallHandleToObjectMappingPairProc );

template<class _Proc> 
class SimpleProcCEGUIEventHandler{
	_Proc m_p;
public:
	explicit SimpleProcCEGUIEventHandler(const _Proc &p):m_p(p){}
	bool operator()(const CEGUI::EventArgs&){
		m_p();
		return true;
	}
};

template <typename _ProcL, typename _ProcR>
class CEGUIMouseEventSimpleSplitter2{
	_ProcL m_l;
	_ProcR m_r;
public:
	CEGUIMouseEventSimpleSplitter2(const _ProcL &l, const _ProcR &r):m_l(l),m_r(r){}
	bool operator()(const CEGUI::EventArgs& e){
		switch(dynamic_cast<const CEGUI::MouseEventArgs &>(e).button)
		{
		case CEGUI::LeftButton:
			m_l();
			break;
		case CEGUI::RightButton:
			m_r();
			break;
		}
		return true;
	}
};

#include <boost/qvm/v.hpp>
template<typename V>
const V VectorParallelToCoordSysBase(unsigned iDim,typename boost::qvm::v_traits<V>::scalar_type const &signedLength){
	assert(iDim<boost::qvm::v_traits<V>::dim);
	V v;
	boost::qvm::set_zero(v);
	boost::qvm::v_traits<V>::iw(iDim,v) = signedLength;
	return v;
}

#include <Ogre_boost_qvm.hpp>
#include <boost/qvm/v.hpp>
#include <boost/qvm/q.hpp>

template<typename E>
typename boost::enable_if<GenericSim::Meta::IsEntity<E>>::type 
SetOgreSceneNodeSpatialConfig(Ogre::SceneNode *sn, const E &e){
	SetOgreSceneNodeSpatialConfigByEntityConcept(sn, e, typename GenericSim::Meta::GetEntityConcept<E>::Type());
}

template<typename E>
void SetOgreSceneNodeSpatialConfigByEntityConcept(Ogre::SceneNode *sn, const E &e, EC::RigidBodyDynamics3D){
	sn->setPosition( boost::qvm::make<Ogre::Vector3>(
		e.at_e<EC::RigidBodyDynamics3D::PositionVector>()
	) );
	sn->setOrientation( boost::qvm::make<Ogre::Quaternion>(
		e.at_e<EC::RigidBodyDynamics3D::OrientationQuat>()
	) );
}

//struct NullProc(){
//	template<typename T>
//	void operator(T &){}
//};
//
//namespace Tree{
//	template<typename T,typename _PreProc,typename _PostProc>
//	std::pair<_PreProc,_PostProc> Traversal(T &treeNode, _PreProc pre, _PostProc post ){
//		struct impl{ void operator()( T &treeNode,_PreProc &pre,_PostProc &post ){
//			pre(treeNode);
//			std::for_each(TreeChildrenBegin(treeNode),TreeChildrenEnd(treeNode),impl());
//			post(treeNode);
//		} };
//		return std::make_pair(pre,post);
//	}
//}

//void DestroyOgreSceneNodeAttachedMovableObjects(Ogre::SceneNode *sn,Ogre::SceneManager *pScene){
//	struct impl{  
//		Ogre::SceneManager *pScene; 
//		void operator()( Ogre::SceneNode &nd ) const{
//			for(Ogre::SceneNode::ObjectIterator i = nd.getAttachedObjectIterator();i.hasMoreElements();){
//				Ogre::MovableObject *p = i.getNext();
//				pScene->destroyMovableObject( p );
//			}
//		}
//	};
//	Tree::Traversal(*sn,NullProc(),Utility::MakeAggr<impl>(pScene));
//}

void DestroyOgreSceneNodeAttachedMovableObjects(Ogre::SceneNode *sn,Ogre::SceneManager *pScene){
	for(Ogre::SceneNode::ChildNodeIterator i = sn->getChildIterator();i.hasMoreElements();){
		DestroyOgreSceneNodeAttachedMovableObjects(static_cast<Ogre::SceneNode*>(i.getNext()),pScene);
	}
	for(Ogre::SceneNode::ObjectIterator i = sn->getAttachedObjectIterator();i.hasMoreElements();){
		Ogre::MovableObject *p = i.getNext();
		pScene->destroyMovableObject( p );
	}
}


#include "GenericShapeToOCCT.hpp"
#include <boost/fusion/include/io.hpp>
#include <fstream>
#include <serialization/NaiveTabularTupleListStream.hpp>
//#include <sqlite3.h>
#include <boost/random.hpp>
#include <boost/functional/value_factory.hpp>
#include "OCCTMesh2OGRE.hpp"
#include <boost/lexical_cast.hpp>
#include <ctime>
#include <boost/fusion/include/make_vector.hpp>
#include <boost/fusion/include/nview.hpp>
#include "NumericRange.hpp"
#include <boost/variant.hpp>
#include <conio.h>
#include <Utility/Make.hpp>
#include <Handles/HandleToElementOfRandomAccessRange.hpp>
#include <Handles/HandleToEntityState.hpp>
#include <Handles/PolyHandle.hpp>
#include <boost/range/algorithm/max_element.hpp>
#include <Ogre_boost_qvm.hpp>
#include "OCCTMeshUtils.hpp"
#include <Standard_TypeDef.hxx>
#include <BRepClass3d_SolidExplorer.hxx>
#include <TopoDS_Shape.hxx>
#include <BRepMesh.hxx>
#include <BRep_Tool.hxx>
#include <Poly_Triangulation.hxx>
#include <TShort_Array1OfShortReal.hxx>
#include <StdPrs_ToolShadedShape.hxx>
#include <Poly_Connect.hxx>
#include <TColgp_Array1OfDir.hxx>
#include <BRepFilletAPI_MakeFillet.hxx>
#include <BRepAlgoAPI_Fuse.hxx>
#include <gce_MakeTranslation.hxx>
#include <gce_MakeRotation.hxx>
#include <gp_Quaternion.hxx>
#include <IntCurvesFace_ShapeIntersector.hxx>
#include <gp_Lin.hxx>
#include "EditMode.hpp"

//#include <boost/statechart/event.hpp>
//#include <boost/statechart/state_machine.hpp>
//#include <boost/statechart/simple_state.hpp>
//#include <boost/statechart/transition.hpp>

int main(int argc, char **argv){
	boost::mt19937 randGen;
	std::auto_ptr<Ogre::Root> pOgreRoot(new Ogre::Root(
#ifdef _DEBUG
		"plugins_d.cfg",
#else
		"plugins.cfg",
#endif
		"graphics.cfg",
		"graphics.log"
	));

	if(!pOgreRoot->showConfigDialog()){
		return 0;
	}
	Ogre::RenderWindow *pWindow = pOgreRoot->initialise(true,"The Catapult Game");

	OISSystem ois(pWindow);
	assert(ois.Keyboards().size()==1);
	assert(ois.Mice().size() == 1);


	{
		Ogre::ResourceGroupManager& lRgMgr = Ogre::ResourceGroupManager::getSingleton();
		pOgreRoot->addResourceLocation("datafiles","FileSystem","CEGUI",true);
		pOgreRoot->addResourceLocation("gui","FileSystem","CEGUI",true);
		pOgreRoot->addResourceLocation("","FileSystem");
		lRgMgr.initialiseAllResourceGroups();
	}
	CEGUI::OgreRenderer &ceguiRenderer = CEGUI::OgreRenderer::bootstrapSystem(*pWindow);
	{
		CEGUI::ResourceProvider* rp = //dynamic_cast<CEGUI::DefaultResourceProvider*>
			(CEGUI::System::getSingleton().getResourceProvider());/*
		rp->setResourceGroupDirectory("schemes", "datafiles/schemes/");
		rp->setResourceGroupDirectory("imagesets", "datafiles/imagesets/");
		rp->setResourceGroupDirectory("fonts", "datafiles/fonts/");
		rp->setResourceGroupDirectory("layouts", "datafiles/layouts/");
		rp->setResourceGroupDirectory("looknfeels", "datafiles/looknfeel/");
		rp->setResourceGroupDirectory("lua_scripts", "datafiles/lua_scripts/");
		rp->setResourceGroupDirectory("schemas", "datafiles/xml_schemas/");*/
		// set the default resource groups to be used
		CEGUI::Imageset::setDefaultResourceGroup("CEGUI");
		CEGUI::Font::setDefaultResourceGroup("CEGUI");
		CEGUI::Scheme::setDefaultResourceGroup("CEGUI");
		CEGUI::WidgetLookManager::setDefaultResourceGroup("CEGUI");
		CEGUI::WindowManager::setDefaultResourceGroup("CEGUI");
		CEGUI::ScriptModule::setDefaultResourceGroup("CEGUI");

		// setup default group for validation schemas
		CEGUI::XMLParser* parser = CEGUI::System::getSingleton().getXMLParser();
		if (parser->isPropertyPresent("SchemaDefaultResourceGroup"))
			parser->setProperty("SchemaDefaultResourceGroup", "CEGUI");
	}
	{
		using namespace CEGUI;
		std::string a,b,c;
		std::ifstream("gui/r.cfg")>>a>>b>>c;
		SchemeManager::getSingleton().createAll( a,"CEGUI" );
		FontManager::getSingleton().createAll( b,"CEGUI" );
		ImagesetManager::getSingleton().createAll( c,"CEGUI" );
		std::ifstream("gui/d.cfg")>>a>>b>>c;
		System::getSingleton().setDefaultFont(a);
		System::getSingleton().setDefaultMouseCursor(b,c);
		Window* myRoot = WindowManager::getSingleton().loadWindowLayout( "game.layout" );
		System::getSingleton().setGUISheet( myRoot );
	}

	Utility::PairedCall<dInitODE,dCloseODE> ode;

	CEGUI::Window *pSceneWindow = CEGUI::WindowManager::getSingleton().getWindow("_scene");
	assert(pSceneWindow);
	Ogre::SceneManager *pSceneManager(pOgreRoot->createSceneManager(Ogre::ST_GENERIC));
	Ogre::Camera *pCam = pSceneManager->createCamera("zzz");
	Ogre::Viewport *pViewport = pWindow->addViewport(pCam);
	{
		float x=8,y=8,z=8;
		std::ifstream("cam.txt")>>x>>y>>z;
		pCam->setPosition(Ogre::Vector3(x,y,z));
	}
	pCam->lookAt(Ogre::Vector3(0,0,0));
	pCam->setNearClipDistance(0.1);
	pCam->setFarClipDistance(20000);
	//pCam->setNearClipDistance(4);
	//pCam->setFarClipDistance(8);

	pSceneManager->showBoundingBoxes(true);
	pSceneManager->setSkyDome(true,"sky",10,8,4000,true,Ogre::Quaternion::IDENTITY,16,16);
	//{
	//	Ogre::StaticGeometry *pSG = pSceneManager->createStaticGeometry("floor");
	//	pSG->addEntity(pSceneManager->createEntity("tqqq","zzz.mesh"),Ogre::Vector3(0,0,0));
	//	pSG->build();
	//}
	{
		Ogre::Light *pL = pSceneManager->createLight("lll");
		pL->setType(Ogre::Light::LT_DIRECTIONAL);
		pL->setDirection(-1,-1,-3);
		pL->setDiffuseColour(1,1,1);
		pL->setSpecularColour(1,1,1);
		pSceneManager->setAmbientLight(Ogre::ColourValue(0,0,0));
	}

	Ogre::ResourceGroupManager::getSingleton().createResourceGroup("__OCCTGEN");
	bool go = true;
	
	std::vector<SimpleBoxComponent> boxes;
	std::vector<SimpleBoxComponent> protoBoxes;
	NaiveTabularTupleListStream::UseAsTupleListStream( std::ifstream("boxesIn.txt") )>>boxes;
	NaiveTabularTupleListStream::UseAsTupleListStream( std::ifstream("protoBoxes.txt") )>>protoBoxes;

	typedef GenericSim::Handles::HandleToElementOfRandomAccessRange< std::vector<SimpleBoxComponent>* > SimpleBoxComponentHandle;
	std::vector<SimpleBoxComponentHandle> sbcDeletionList;
	std::vector< boost::array<SimpleBoxComponentHandle,2> > sbcMovingList;
	std::vector<SimpleBoxComponentHandle> sbcInsertionList;
	
	typedef GenericSim::Handles::PolyHandle<RigidBodyInst> RigidBodyInstHandle;
	std::vector<RigidBodyInstHandle> rigidBodyDeletionList;
	std::vector< boost::array<RigidBodyInstHandle,2> > rigidBodyMovingList;
	std::vector<RigidBodyInstHandle> rigidBodyInsertionList;

	struct PostSimpleBoxComponentConstruction{
		std::vector<SimpleBoxComponent> &boxes;
		std::vector<SimpleBoxComponentHandle> &sbcInsertionList;
		std::vector<RigidBodyInstHandle> &rigidBodyInsertionList;
		void operator()(std::size_t i){
			using namespace GenericSim::Handles;
			rigidBodyInsertionList.push_back( MakePoly( 
				MakeHandleToEntityState<EC::Component,EC::Component::RigidBodyStates >(
					Utility::Make<HandleToElementOfRandomAccessRange>( &boxes, i )
				)
			));
			sbcInsertionList.push_back( Utility::Make<HandleToElementOfRandomAccessRange>( &boxes, i ) );
		}
	};
	PostSimpleBoxComponentConstruction postAddingSimpleBoxComponent = { boxes, sbcInsertionList, rigidBodyInsertionList };

	struct ToAddSimpleBoxComponent{
		std::vector<SimpleBoxComponent> &boxes;
		GenericSim::Handles::PolyHandle< SimpleBoxComponent > hProto;
		PostSimpleBoxComponentConstruction postProc;
		void operator()(){
			int j = boxes.size();
			boxes.push_back(*hProto);
			postProc(j);
		}
	};
	

	BOOST_FOREACH( std::size_t i, boost::irange<std::size_t>(0,boxes.size()) ){
		postAddingSimpleBoxComponent(i);
	}

	BOOST_FOREACH( std::size_t i, boost::irange<std::size_t>(0,protoBoxes.size()) ){
		using namespace GenericSim::Handles;
		CEGUI::PushButton *pBtnSBCAdd = dynamic_cast<CEGUI::PushButton *>(
			CEGUI::WindowManager::getSingleton().getWindow( "_simpleBoxComponentAdd" + boost::lexical_cast<std::string>(i) )
		);
		assert(pBtnSBCAdd);
		pBtnSBCAdd->subscribeEvent(
			CEGUI::PushButton::EventClicked,
			Utility::Make<SimpleProcCEGUIEventHandler>(
				Utility::TieIntoAggr<ToAddSimpleBoxComponent>(
					boxes, 
					MakePoly( Utility::Make<HandleToElementOfRandomAccessRange>( &protoBoxes, i ) ),
					postAddingSimpleBoxComponent
				)
			)
		);
	}

	std::vector<BallJoint> ballJoints;	
	typedef GenericSim::Handles::HandleToElementOfRandomAccessRange< std::vector<BallJoint>* > BallJointHandle;

	NaiveTabularTupleListStream::UseAsTupleListStream( std::ifstream("BallJointsIn.txt") )>>ballJoints;
	std::vector< BallJointHandle > ballJointDeletionList;
	std::vector< boost::array<BallJointHandle,2> > ballJointMovingList;
	std::vector< boost::fusion::vector<BallJointHandle,RigidBodyInstHandle, RigidBodyInstHandle> > ballJointInsertionList;
	BOOST_FOREACH( std::size_t i, boost::irange<std::size_t>(0,ballJoints.size()) )
	{
		using namespace boost::fusion;
		using namespace GenericSim::Handles;
		ballJointInsertionList.push_back( make_vector(
			Utility::Make<HandleToElementOfRandomAccessRange>( &ballJoints, i),
			MakePoly(	MakeHandleToEntityState<EC::Component,EC::Component::RigidBodyStates>(
				Utility::Make<HandleToElementOfRandomAccessRange>( &boxes, at_c<0>(ballJoints[i].at_e<EC::Joint::AttachedObjectIdentifierPair>()) )
			) ),
			MakePoly(	MakeHandleToEntityState<EC::Component,EC::Component::RigidBodyStates>(
				Utility::Make<HandleToElementOfRandomAccessRange>( &boxes, at_c<1>(ballJoints[i].at_e<EC::Joint::AttachedObjectIdentifierPair>()) )
			) )
		) );
	}


	std::map< SimpleBoxComponentHandle, Ogre::SceneNode *> pBoxOgreSceneNodes;

    static dWorldID odeWorld = dWorldCreate();
	{
		dWorldSetGravity (odeWorld,0,0,0);
		dWorldSetERP(odeWorld,0.8);
	}

	ODEWorldVariableStrideStep<int (*)(dWorldID,dReal)> odeWorldStepper(dWorldStep,0.02,0.5);

	std::map<RigidBodyInstHandle, dBodyID> odeRigidBodies;
	std::map<BallJointHandle, dJointID> odeBallJoints;

	pCam->setAutoAspectRatio(true);
	time_t currentTime = clock();

	//Ogre::SceneNode *pTestSceneNode = pSceneManager->getRootSceneNode()->createChildSceneNode();
	//pTestSceneNode->attachObject(pSceneManager->createEntity("testSceneNode","unitSphere.mesh"));

	std::vector<std::size_t> sbcToDelete;

	CEGUI::PushButton *pBtnBJAdd = dynamic_cast<CEGUI::PushButton *>(CEGUI::WindowManager::getSingleton().getWindow("_ballJointAdd"));
	assert(pBtnBJAdd);

	std::size_t sbcSelected = std::numeric_limits<std::size_t>::max();

	EditMode::GameStatesRef emgs = {boxes,protoBoxes,sbcToDelete};
	EditMode editMode(emgs);
	editMode.SetOgreScene(pSceneManager);
	pSceneWindow->subscribeEvent(
		CEGUI::Window::EventMouseButtonDown,
		Utility::Make<CEGUIMouseEventSimpleSplitter2>(
			boost::bind(&EditMode::OnPrimaryPointerDown,&editMode),
			boost::bind(&EditMode::OnSecondaryPointerDown,&editMode)
		)
		//Utility::Make<SimpleProcCEGUIEventHandler>(boost::bind(&EditMode::OnMouseDown,&editMode))
	);
	pSceneWindow->subscribeEvent(
		CEGUI::Window::EventMouseButtonUp,
		Utility::Make<CEGUIMouseEventSimpleSplitter2>(
			boost::bind(&EditMode::OnPrimaryPointerUp,&editMode),
			boost::bind(&EditMode::OnSecondaryPointerUp,&editMode)
		)
		//Utility::Make<SimpleProcCEGUIEventHandler>(boost::bind(&EditMode::OnMouseUp,&editMode))
	);
	struct SceneWindowKeyHandler{
		EditMode &editMode;
		bool OnKeyDown(const CEGUI::EventArgs &e){
			const CEGUI::Key::Scan k = dynamic_cast<const CEGUI::KeyEventArgs &>(e).scancode;
			using namespace CEGUI;
			switch(k){
				case Key::Delete: editMode.DeleteSelected(); break;
				//case Key::LeftShift: editMode.SetRotatingMode(true); break;
			}
			return true;
		}
		bool OnKeyUp(const CEGUI::EventArgs &e){
			const CEGUI::Key::Scan k = dynamic_cast<const CEGUI::KeyEventArgs &>(e).scancode;
			using namespace CEGUI;
			switch(k){
				//case Key::Delete: editMode.DeleteSelected(); break;
				//case Key::LeftShift: editMode.SetRotatingMode(false); break;
			}
			return true;
		}
	};
	SceneWindowKeyHandler keyHandler = {editMode};
	pSceneWindow->subscribeEvent(
		CEGUI::Window::EventKeyDown,
		CEGUI::SubscriberSlot(&SceneWindowKeyHandler::OnKeyDown,&keyHandler)
	);
	pSceneWindow->subscribeEvent(
		CEGUI::Window::EventKeyUp,
		CEGUI::SubscriberSlot(&SceneWindowKeyHandler::OnKeyUp,&keyHandler)
	);
	pSceneWindow->subscribeEvent(
		CEGUI::Window::EventMouseEnters,
		Utility::Make<SimpleProcCEGUIEventHandler>(boost::bind(&CEGUI::Window::activate,pSceneWindow))
	);
	//pSceneWindow->subscribeEvent(
	//	CEGUI::Window::EventMouseClick,
	//	Utility::Make<SimpleProcCEGUIEventHandler>(boost::bind(&EditMode::OnMouseClick,&editMode))
	//);
	
	//ToProcessSceneWindowMouseDown sceneWindowMouseDown;
	//ToProcessSceneWindowMouseUp sceneWindowMouseUp;
	///ToProcessSceneWindowMouseClick sceneWindowMouseClick;
	//pSceneWindow->subscribeEvent(CEGUI::Window::EventMouseButtonDown,sceneWindowMouseDown);
	//pSceneWindow->subscribeEvent(CEGUI::Window::EventMouseButtonUp,sceneWindowMouseUp);
	//pSceneWindow->subscribeEvent(CEGUI::Window::EventMouseClick,sceneWindowMouseClick);
	//pSceneWindow->subscribeEvent(CEGUI::Window::EventMouseButtonMove,sceneWindowMouseEventHandler);

	while(go&&!pWindow->isClosed()){
		{
			assert(sbcToDelete.empty()||*boost::max_element(sbcToDelete)<boxes.size());
			for(std::vector<std::size_t>::iterator i = sbcToDelete.begin(); i != sbcToDelete.end(); ++i)
			{
				std::size_t &p = *i; // the deletion query list is to be modified.
				if(p>=boxes.size()){
					p = *(i-(p-boxes.size()+1));
				}
				boxes[p] = boxes.back();
				boxes.pop_back();
				using namespace GenericSim::Handles;
				{
					boost::array<SimpleBoxComponentHandle,2> a = {
						Utility::Make<HandleToElementOfRandomAccessRange>( &boxes,boxes.size() ),
						Utility::Make<HandleToElementOfRandomAccessRange>( &boxes,p )
					};
					sbcMovingList.push_back(a);
				}
				{
					boost::array<RigidBodyInstHandle,2> a = {
						MakePoly( MakeHandleToEntityState<EC::Component,EC::Component::RigidBodyStates >(
							Utility::Make<HandleToElementOfRandomAccessRange>( &boxes, boxes.size() )
						) ),
						MakePoly( MakeHandleToEntityState<EC::Component,EC::Component::RigidBodyStates >(
							Utility::Make<HandleToElementOfRandomAccessRange>( &boxes, p )
						) )
					};
					rigidBodyMovingList.push_back(a);
				}
			}
			sbcToDelete.clear();
		}
		if(_kbhit()){
			char ch = _getch();
			if('`'==ch) while(true){
				//PLAN:(first step - just box components and ball joints)
				//0. just get the console working
				//1. refactor into 1 stateless function call each console command
				//2. build the gui calling the functions

				int cmd = _getch();
				if('a'==cmd){
					std::cout<<"\nadd:";
					std::size_t i = protoBoxes.size();
					std::cin>>i;
					assert(i<protoBoxes.size());
					if(i<protoBoxes.size()){
						//Insertions can happen any time in the loop between a deletion sweep and the simulation
						//boxes.push_back( protoBoxes[i] );
						//rigidBodyInsertionList.push_back( &boxes.back().at_e<EC::Component::RigidBodyStates>() );
					}
				}else if('r'==cmd){	
					std::cout<<"\nremove:";
					std::size_t i = boxes.size();
					std::cin>>i;
					assert(i<boxes.size());
					if(i<boxes.size()){
						sbcToDelete.push_back(i);
						//Actual deletions must be centralized. Insertions must happen after deletions.
						//But deletions caused by deletions( e.g. joint deletions caused by a rigid body deletion ) should be resolved here
					}
				}else if('p'==cmd){
					std::cout<<"\nid:";
					std::size_t i = boxes.size();
					std::cin>>i;
					assert(i<boxes.size());
					std::cout<<"\nset position:";
					NaiveTabularTupleListStream::UseAsTupleStream( std::cin )>>boxes[i].at_e<EC::Component::RigidBodyStates>().at_e<EC::RigidBodyDynamics3D::PositionVector>();
				}else if('o'==cmd){
					std::cout<<"\nid:";
					std::size_t i = boxes.size();
					std::cin>>i;
					assert(i<boxes.size());
					std::cout<<"\nset orientation:";
					NaiveTabularTupleListStream::UseAsTupleStream( std::cin )>>boxes[i].at_e<EC::Component::RigidBodyStates>().at_e<EC::RigidBodyDynamics3D::OrientationQuat>();
				}else if('z'==cmd){
					std::cout<<"\nid:";
					std::size_t i = boxes.size();
					std::cin>>i;
					assert(i<boxes.size());
					std::cout<<"\nset size:";
					NaiveTabularTupleListStream::UseAsTupleStream( std::cin )>>boxes[i].at_e<EC::Component::Shape>().at_e<EC::Box::Size>(); 
					assert(0);
				}else if('s'==cmd){ // this command is irrelevant to UI
					std::cout<<"\nsave to file.\n";
					NaiveTabularTupleListStream::UseAsTupleListStream( std::ofstream("boxesOut.txt") )<<boxes;
				}else if('`'==cmd){
					break;
				}
			}else if('w'==ch){
				pCam->setPolygonMode (Ogre::PM_WIREFRAME);
			}else if('s'==ch){
				pCam->setPolygonMode (Ogre::PM_SOLID);
			}else if('p'==ch){
				pCam->setPolygonMode (Ogre::PM_POINTS);
			}
		}

		time_t elapsedTime;
		{
			time_t nextTime = clock();
			elapsedTime = nextTime-currentTime;
			currentTime = nextTime;
			if(elapsedTime>100) elapsedTime = 100;
		}
		Real elapsedSeconds = elapsedTime*(1.0e-3);
		CEGUI::System::getSingleton().injectTimePulse(elapsedSeconds);

		{	//apply drag in edit mode
			Real c = exp(-elapsedSeconds);
			BOOST_FOREACH( SimpleBoxComponent &sbc, boxes ){ 
				using namespace boost::qvm;
				sbc.at_e<EC::Component::RigidBodyStates>().at_e<EC::RigidBodyDynamics3D::LinearVelocityVector>()*=c;
				sbc.at_e<EC::Component::RigidBodyStates>().at_e<EC::RigidBodyDynamics3D::AngularVelocityVector>()*=c;
			}
		}

		ManageODEObjectLifetime(
			rigidBodyDeletionList, rigidBodyMovingList, rigidBodyInsertionList,
			ballJointDeletionList, ballJointMovingList, ballJointInsertionList,
			odeRigidBodies, odeBallJoints, odeWorld
		);

		boost::for_each( odeRigidBodies, boost::bind( CallHandleToObjectMappingPairProc_PROC_OBJ_2(),SetOpenDERigidBody_PROC_OBJ_2(),_1 ) );

		odeWorldStepper(odeWorld,elapsedSeconds);
		
		boost::for_each( odeRigidBodies, boost::bind( CallHandleToObjectMappingPairProc_PROC_OBJ_2(),GetOpenDERigidBody_PROC_OBJ_2(),_1 ) );
		
		{
			class ToDestroyComponentOgreSceneNode{
				Ogre::SceneManager *m_pSceneManager;
			public:
				void operator()(Ogre::SceneNode *p){
					//for(Ogre::SceneManager::MovableObjectIterator i = m_pSceneManager->getMovableObjectIterator("Entity"); i.hasMoreElements(); ){
					//	std::cout<<i.getNext()->getName()<<'\n';
					//}
					//std::cout<<"===\n";
					//for(Ogre::SceneNode::ObjectIterator i = p->getAttachedObjectIterator();i.hasMoreElements();){
					//	Ogre::MovableObject *p = i.getNext();
					//	m_pSceneManager->destroyMovableObject( p );
					//}
					DestroyOgreSceneNodeAttachedMovableObjects(p,m_pSceneManager);
					m_pSceneManager->destroySceneNode(p);
					//for(Ogre::MeshManager::ResourceMapIterator &i = Ogre::MeshManager::getSingleton().getResourceIterator(); i.hasMoreElements(); ){
					//	std::cout<<i.getNext()->getName()<<'\n';
					//}
					//std::cout<<"---\n";
					//for(Ogre::SceneManager::MovableObjectIterator i = m_pSceneManager->getMovableObjectIterator("Entity"); i.hasMoreElements(); ){
					//	std::cout<<i.getNext()->getName()<<'\n';
					//}
					//std::cout<<"===\n";
				}
				
				ToDestroyComponentOgreSceneNode(Ogre::SceneManager *pSM):m_pSceneManager(pSM){ assert(pSM); }
			};
			MappingFollowDeletionAndMoving(sbcDeletionList,sbcMovingList,pBoxOgreSceneNodes,ToDestroyComponentOgreSceneNode(pSceneManager));
			if(sbcDeletionList.size())	Ogre::MeshManager::getSingleton().removeUnreferencedResources(false);
		}

		BOOST_FOREACH( const SimpleBoxComponentHandle &hSBC, sbcInsertionList )
		{
			TopoDS_Shape box = GenericShapeToOCCT(hSBC->at_e<EC::Component::Shape>());
			BRepFilletAPI_MakeFillet mf(box);
			for(TopExp_Explorer e(box,TopAbs_EDGE);e.More();e.Next()){
				mf.Add(TopoDS::Edge(e.Current()));
			}
			Real r=0.02;
			std::ifstream("fillet.txt")>>r;
			for (Standard_Integer i = 1;i<=mf.NbContours();++i) {
				mf.SetRadius(r,i,1);
			}
			//boxEntityNames.push_back("boxEntityNames"+boost::lexical_cast<std::string>(boxEntityNames.size()));
			std::string meshName = "boxMeshName"+boost::lexical_cast<std::string>(clock())+"|"+boost::lexical_cast<std::string>(pBoxOgreSceneNodes.size());
			std::string entityName = "boxEntityName"+boost::lexical_cast<std::string>(clock())+"|"+boost::lexical_cast<std::string>(pBoxOgreSceneNodes.size());
			OCCTMesh2OGRE::FillBlankOgreMeshUsingOCCTShape(
				Ogre::MeshManager::getSingleton().createManual( meshName , "__OCCTGEN" ),
				mf.Shape(),
				1
			);
			Ogre::SceneNode *pSN = pSceneManager->getRootSceneNode()->createChildSceneNode();
			pSN->attachObject( pSceneManager->createEntity(entityName,meshName,"__OCCTGEN") );
			pBoxOgreSceneNodes.insert( std::make_pair( hSBC, pSN ) );
		}
		{
			typedef const std::pair< SimpleBoxComponentHandle, Ogre::SceneNode *> &ptype;
			BOOST_FOREACH( ptype p , pBoxOgreSceneNodes ){
				SetOgreSceneNodeSpatialConfig(p.second,p.first->at_e<EC::Component::RigidBodyStates>());
			}
		}

		sbcDeletionList.clear();
		sbcMovingList.clear();
		sbcInsertionList.clear();
		rigidBodyDeletionList.clear();
		rigidBodyMovingList.clear();
		rigidBodyInsertionList.clear();
		ballJointDeletionList.clear();
		ballJointMovingList.clear();
		ballJointInsertionList.clear();

		{
			CEGUI::Rect r = pSceneWindow->getClipRect();
			CEGUI::Vector2 mousePos = CEGUI::MouseCursor::getSingleton().getPosition() - r.getPosition();
			Ogre::Real 
				xh = mousePos.d_x / r.getWidth() * 2 - 1, 
				yh = -(mousePos.d_y / r.getHeight() * 2 - 1);
			editMode.Update(elapsedSeconds,Ogre::Vector2(xh,yh),pCam);
		}

		pViewport->setDimensions(
			pSceneWindow->getClipRect().getPosition().d_x/pWindow->getWidth(),
			pSceneWindow->getClipRect().getPosition().d_y/pWindow->getHeight(),
			pSceneWindow->getClipRect().getWidth()/pWindow->getWidth(),
			pSceneWindow->getClipRect().getHeight()/pWindow->getHeight()
		);
		//pCam->setAspectRatio(Ogre::Real(pViewport->getActualWidth())/Ogre::Real(pViewport->getActualHeight()));
		{
			boost::uniform_real<float> dist(0.4f,0.6f);
			boost::variate_generator< boost::mt19937&,boost::uniform_real<float> > die(randGen,dist);
			pViewport->setBackgroundColour(Ogre::ColourValue(die(),die(),die()));
		}
		unsigned int windowWidthToHold=50,windowHeightToHold=50;
		{
			unsigned int a=0;
			int b=0,c=0;
			pWindow->getMetrics(windowWidthToHold,windowHeightToHold,a,b,c);
		}
		Ogre::WindowEventUtilities::messagePump();
		{
			unsigned int w=50,h=50,a=0;
			int b=0,c=0;
			pWindow->getMetrics(w,h,a,b,c);
			if(w!=windowWidthToHold || h!=windowHeightToHold){
				pWindow->resize(windowWidthToHold,windowHeightToHold);
			}
		}
		RouteOISUserInput(*ois.Mice().front(),OIS2CEGUIMouseListener(CEGUI::System::getSingleton()));
		RouteOISUserInput(*ois.Keyboards().front(),OIS2CEGUIKeyListener(CEGUI::System::getSingleton()));
		//System::injectTimePulse( float timeElapsed );
		pOgreRoot->renderOneFrame();
	}
	NaiveTabularTupleListStream::UseAsTupleListStream( std::ofstream("boxesOut.txt") )<<boxes;

	return 0;

    dsFunctions fn;
    fn.version = DS_VERSION;
    fn.start = &start;
    fn.step = &simLoop;
    fn.stop = 0;
    fn.command = 0;
    fn.path_to_textures = "E:\\Libraries\\dynamics\\opende\\drawstuff\\textures";
    world = dWorldCreate();
    space = dHashSpaceCreate (0);
    dWorldSetGravity (world,0,0,-9.8);
	dWorldSetERP(world,0.8);
	dCreatePlane(space,0,0,1,0);
    contactgroup = dJointGroupCreate (0);
//    body = dBodyCreate (world);
//    geom = dCreateSphere (space,0.5);
//    dMassSetSphere (&m,1,0.5);
//    dBodySetMass (body,&m);
//    dGeomSetBody (geom,body);
//    dBodySetPosition (body,0,0,3);
    dsSimulationLoop (argc,argv,1280,720,&fn);
    dJointGroupDestroy (contactgroup);
    dSpaceDestroy (space);
    dWorldDestroy (world);
	return 0;
}


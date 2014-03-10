#pragma once

//the generated shapes are properly centered and rotated

#include <TopoDS_Shape.hxx>

inline const TopoDS_Shape &GenericShapeToOCCT( const TopoDS_Shape &s ){
	return s;
}

#include <Meta/entity.hpp>
#include <boost/utility/enable_if.hpp>

template<typename E>
typename boost::enable_if<GenericSim::Meta::IsEntity<E>,const TopoDS_Shape>::type 
GenericShapeToOCCT( const E &e ){
	return ShapeEntityToOCCTByConcept( e, typename GenericSim::Meta::GetEntityConcept<E>::Type() );
}

#include <BRepPrimAPI_MakeBox.hxx>
template< typename E >
const TopoDS_Shape ShapeEntityToOCCTByConcept( const E &e, EC::Box ){
	gp_XYZ size = boost::fusion::invoke( boost::value_factory<gp_XYZ>(), e.at_e<EC::Box::Size>() );
	return BRepPrimAPI_MakeBox(gp_Pnt(size*-0.5),gp_Pnt(size*0.5));
}

#include <BRepPrimAPI_MakeCylinder.hxx>
#include <gp_Ax2.hxx>
#include <gp_XYZ.hxx>
template< typename E >
const TopoDS_Shape ShapeEntityToOCCTByConcept( const E &e, EC::Cylinder ){
	return BRepPrimAPI_MakeCylinder( 
		gp_Ax2(gp_Pnt(0,0,-e.at_e<EC::Cylinder::Length>()*0.5),gp_XYZ(0,0,1)),
		e.at_e<EC::Cylinder::Radius>(), 
		e.at_e<EC::Cylinder::Length>() 
	);
}

#include <BRepPrimAPI_MakeSphere.hxx>
template< typename E >
const TopoDS_Shape ShapeEntityToOCCTByConcept( const E &e, EC::Sphere ){
	return BRepPrimAPI_MakeSphere( e.at_e<EC::Sphere::Radius>() );
}

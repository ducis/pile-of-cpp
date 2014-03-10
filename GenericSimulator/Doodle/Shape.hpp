#pragma once


//template<typename S>
//Real GetShapeBoundingSphereRadiusSquared(const S &s){
//}

#include <Meta/entity.hpp>
#include <boost/utility/enable_if.hpp>

template<typename S>
typename boost::enable_if<GenericSim::Meta::IsEntity<S>,const Real>::type 
GetShapeBoundingSphereRadiusSquared(const S &s){
	return GetShapeBoundingSphereRadiusSquared_Impl::ByEntityConcept( s, typename GenericSim::Meta::GetEntityConcept<S>::Type() );
}

#include <boost/qvm/v.hpp>
#include <cmath>
namespace GetShapeBoundingSphereRadiusSquared_Impl{
	template<typename E>
	Real ByEntityConcept( const E &e, EC::Box ){
		return boost::qvm::mag2(e.at_e<EC::Box::Size>())*Real(0.25);
	}

	template<typename E>
	Real ByEntityConcept( const E &e, EC::Sphere ){
		return pow(e.at_e<EC::Sphere::Radius>(),2);
	}

	template<typename E>
	Real ByEntityConcept( const E &e, EC::Cylinder ){
		return pow(e.at_e<EC::Cylinder::Length>(),2)*Real(0.25)+pow(e.at_e<EC::Cylinder::Radius>(),2);
	}
}


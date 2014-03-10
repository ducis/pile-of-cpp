#pragma once

//#include <Ogre.h>
#include <OgreVector2.h>
#include <OgreVector3.h>
#include <OgreVector4.h>
#include <OgreQuaternion.h>
#include <boost/qvm/v_traits.hpp>
#include <boost/qvm/q_traits.hpp>

namespace boost{ namespace qvm{

#define IMPL_RW_H78G2G25HXPJPNZ( I, V, E )										\
		template<> static inline scalar_type r<I>( V const &v ){ return v.E; }	\
		template<> static inline scalar_type &w<I>( V &v ){ return v.E; }

#define IMPL_DSGJ325Y225H_RW2( V )			\
		IMPL_RW_H78G2G25HXPJPNZ( 0, V, x )	\
		IMPL_RW_H78G2G25HXPJPNZ( 1, V, y )	\

#define IMPL_DSGJ325Y225H_RW3( V )			\
		IMPL_DSGJ325Y225H_RW2( V )			\
		IMPL_RW_H78G2G25HXPJPNZ( 2, V, z )

#define IMPL_DSGJ325Y225H_RW4( V )			\
		IMPL_DSGJ325Y225H_RW3( V )			\
		IMPL_RW_H78G2G25HXPJPNZ( 3, V, w )
		

#define DEF_OGRE_QVM_4OISEYSE5352H0S095PG5UBETWEYS43( N )										\
	template<> struct v_traits<Ogre::Vector##N>{												\
		static int const dim = N;																\
		typedef Ogre::Real scalar_type;															\
		template <int I> static inline scalar_type r( Ogre::Vector##N const & );				\
		template <int I> static inline scalar_type &w( Ogre::Vector##N & );						\
		static inline scalar_type ir( int i, Ogre::Vector##N const & v){ return v[i]; }			\
		static inline scalar_type &iw( int i, Ogre::Vector##N & v){ return v[i]; }				\
		IMPL_DSGJ325Y225H_RW##N( Ogre::Vector##N )												\
	};

	DEF_OGRE_QVM_4OISEYSE5352H0S095PG5UBETWEYS43(2)
	DEF_OGRE_QVM_4OISEYSE5352H0S095PG5UBETWEYS43(3)
	DEF_OGRE_QVM_4OISEYSE5352H0S095PG5UBETWEYS43(4)

#undef DEF_OGRE_QVM_4OISEYSE5352H0S095PG5UBETWEYS43
#undef IMPL_DSGJ325Y225H_RW4
#undef IMPL_DSGJ325Y225H_RW3
#undef IMPL_DSGJ325Y225H_RW2

	template<> struct q_traits<Ogre::Quaternion>{
		typedef Ogre::Real scalar_type;
		template <int I> static inline scalar_type r( Ogre::Quaternion const & );
		template <int I> static inline scalar_type &w( Ogre::Quaternion & );
		IMPL_RW_H78G2G25HXPJPNZ( 0 , Ogre::Quaternion , w )
		IMPL_RW_H78G2G25HXPJPNZ( 1 , Ogre::Quaternion , x )
		IMPL_RW_H78G2G25HXPJPNZ( 2 , Ogre::Quaternion , y )
		IMPL_RW_H78G2G25HXPJPNZ( 3 , Ogre::Quaternion , z )
	};
#undef IMPL_RW_H78G2G25HXPJPNZ
}}

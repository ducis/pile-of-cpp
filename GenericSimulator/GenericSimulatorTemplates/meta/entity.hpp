#pragma once
#include <boost/mpl/pair.hpp>
#include <boost/type_traits/is_const.hpp>
#include <boost/type_traits/is_class.hpp>
#include <boost/mpl/if.hpp>
#include <boost/utility/enable_if.hpp>
#include <cstddef>

namespace GenericSim{ namespace Meta{
	template <typename T, typename _Enable = void>
	struct IsEntity: boost::mpl::false_{};

	template <typename T>
	struct IsEntity<T, typename boost::mpl::pair<typename T::entity_concept_type,void>::second>: boost::mpl::true_{};

	template <typename _Entity>
	struct GetEntityConcept{
		typedef typename _Entity::entity_concept_type Type;
	};

	template <typename _Entity>
	struct GetEntityLabels{
		typedef typename GetEntityConcept<_Entity>::Type::Labels Type;
	};

	template <typename _Entity>
	struct GetEntityDimension{
		static const std::size_t Value = _Entity::entity_concept_type::NDIM;
	};

	template <typename _Entity, std::size_t N>
	struct GetEntityAtResultType{ //With regard to constness of _Entity 
		//typedef typename _Entity::AtResultType< typename GetEntityLabels<_Entity>::Type(N) >::Type Type;
		typedef typename boost::mpl::if_< 
			boost::is_const<_Entity>,
			typename _Entity::ConstAtResultType< typename GetEntityLabels<_Entity>::Type(N) >::Type,
			typename _Entity::AtResultType< typename GetEntityLabels<_Entity>::Type(N) >::Type
		>::type Type;
	};

	template <typename _Entity, std::size_t N>
	struct GetEntityStateType{ //With regard to constness of _Entity
		//typedef typename _Entity::StateType< typename GetEntityLabels<_Entity>::Type(N) >::Type Type;
		//typedef typename boost::mpl::if_< 
		//	boost::is_const<_Entity>,
		//	typename _Entity::ConstStateType< typename GetEntityLabels<_Entity>::Type(N) >::Type,
		//	typename _Entity::StateType< typename GetEntityLabels<_Entity>::Type(N) >::Type
		//>::type Type;
		typedef typename _Entity::StateType< typename GetEntityLabels<_Entity>::Type(N) >::Type Type;
	};
//#ifndef GENSIM_MAX_ENTITY_NESTING_LEVEL
//#define GENSIM_MAX_ENTITY_NESTING_LEVEL 16
//#endif
	//template <
	//	typename _Entity, 
	//	typename GetEntityLabels<_Entity>::Type N0
	//>
	//struct GetEntityStateType_E1
	//{
	//	typedef typename GetEntityStateType<_Entity,N0>::Type Type;
	//};

	//template <
	//	typename _Entity,
	//	typename GetEntityLabels< _Entity >::Type N0,
	//	typename GetEntityLabels< typename GetEntityStateType_E1<_Entity,N0>::Type >::Type N1
	//>
	//struct GetEntityStateType_E2
	//{
	//	typedef typename GetEntityStateType< typename GetEntityStateType_E1<_Entity,N0>::Type, N1 >::Type Type;
	//};

	//template <
	//	typename _Entity,
	//	typename GetEntityLabels< _Entity >::Type N0,
	//	typename GetEntityLabels< typename GetEntityStateType_E1<_Entity,N0>::Type >::Type N1,
	//	typename GetEntityLabels< typename GetEntityStateType_E2<_Entity,N0,N1>::Type >::Type N2
	//>
	//struct GetEntityStateType_E3
	//{
	//	typedef typename GetEntityStateType< typename GetEntityStateType_E2<_Entity,N0,N1>::Type, N2 >::Type Type;
	//};

	//template <
	//	typename _Entity,
	//	typename GetEntityLabels< _Entity >::Type N0,
	//	typename GetEntityLabels< typename GetEntityStateType_E1<_Entity,N0>::Type >::Type N1,
	//	typename GetEntityLabels< typename GetEntityStateType_E2<_Entity,N0,N1>::Type >::Type N2,
	//	typename GetEntityLabels< typename GetEntityStateType_E3<_Entity,N0,N1,N2>::Type >::Type N3
	//>
	//struct GetEntityStateType_E4
	//{
	//	typedef typename GetEntityStateType< typename GetEntityStateType_E3<_Entity,N0,N1,N2>::Type, N3 >::Type Type;
	//};



	//template < 
	//	typename _Entity,
	//	typename GetEntityLabels< _Entity >::Type N0
	//>
	//struct GetEntityAtResultType_E1
	//{
	//	typedef typename GetEntityAtResultType< _Entity,N0 >::Type Type;
	//};

	//template <
	//	typename _Entity,
	//	typename GetEntityLabels< _Entity >::Type N0,
	//	typename GetEntityLabels< typename GetEntityStateType_E1<_Entity,N0>::Type >::Type N1
	//>
	//struct GetEntityAtResultType_E2
	//{
	//	typedef typename GetEntityAtResultType< typename GetEntityStateType_E1<_Entity,N0>::Type, N1 >::Type Type;
	//};

	//template <
	//	typename _Entity,
	//	typename GetEntityLabels< _Entity >::Type N0,
	//	typename GetEntityLabels< typename GetEntityStateType_E1<_Entity,N0>::Type >::Type N1,
	//	typename GetEntityLabels< typename GetEntityStateType_E2<_Entity,N0,N1>::Type >::Type N2
	//>
	//struct GetEntityAtResultType_E3
	//{
	//	typedef typename GetEntityAtResultType< typename GetEntityStateType_E2<_Entity,N0,N1>::Type, N2 >::Type Type;
	//};

	//template <
	//	typename _Entity,
	//	typename GetEntityLabels< _Entity >::Type N0,
	//	typename GetEntityLabels< typename GetEntityStateType_E1<_Entity,N0>::Type >::Type N1,
	//	typename GetEntityLabels< typename GetEntityStateType_E2<_Entity,N0,N1>::Type >::Type N2,
	//	typename GetEntityLabels< typename GetEntityStateType_E3<_Entity,N0,N1,N2>::Type >::Type N3
	//>
	//struct GetEntityAtResultType_E4
	//{
	//	typedef typename GetEntityAtResultType< typename GetEntityStateType_E3<_Entity,N0,N1,N2>::Type,N3 >::Type Type;
	//};
}}


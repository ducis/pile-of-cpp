//#pragma once
//
//#include <Meta/entity.hpp>
//#include <entity_as_boost_fusion_sequence.hpp>
//
//namespace GenericSim{
//	namespace Entities{
//		template <
//			typename GenericSim::Meta::GetEntityLabels<_Entity>::Type N0,
//			typename _Entity
//		>	
//		typename GenericSim::Meta::GetEntityAtResultType_E1<_Entity,N0>::Type 
//		At_E(_Entity &e){// overload this function for different types
//			return e.at_e<N0>();
//		}
//		
//		template <
//			typename GenericSim::Meta::GetEntityLabels<_Entity>::Type N0,
//			typename GetEntityLabels< typename GetEntityStateType_E<_Entity,N0>::Type >::Type N1,
//			typename _Entity
//		>	
//		typename GenericSim::Meta::GetEntityAtResultType_E2<_Entity,N0,N1>::Type 
//		At_E(_Entity &e){
//			return At_E1<N1>(At_E1<N0>(e));
//		}
//		
//		template <
//			typename GenericSim::Meta::GetEntityLabels<_Entity>::Type N0,
//			typename GetEntityLabels< typename GetEntityStateType_E<_Entity,N0>::Type >::Type N1,
//			typename GetEntityLabels< typename GetEntityStateType_E<_Entity,N0,N1>::Type >::Type N2,
//			typename _Entity
//		>	
//		typename GenericSim::Meta::GetEntityAtResultType_E3<_Entity,N0,N1,N2>::Type 
//		At_E(_Entity &e){
//			return At_E1<N2>(At_E2<N0,N1>(e));
//		}
//		
//		template <
//			typename GenericSim::Meta::GetEntityLabels<_Entity>::Type N0,
//			typename GetEntityLabels< typename GetEntityStateType_E<_Entity,N0>::Type >::Type N1,
//			typename GetEntityLabels< typename GetEntityStateType_E<_Entity,N0,N1>::Type >::Type N2,
//			typename GetEntityLabels< typename GetEntityStateType_E<_Entity,N0,N1,N2>::Type >::Type N3,
//			typename _Entity
//		>	
//		typename GenericSim::Meta::GetEntityAtResultType_E4<_Entity,N0,N1,N2,N3>::Type 
//		At_E(_Entity &e){
//			return At_E1<N3>(At_E3<N0,N1,N2>(e));
//		}
//	}
//}
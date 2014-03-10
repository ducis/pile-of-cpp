#pragma once

#include "meta/entity.hpp"
#include "core/EntityFreeFunc.hpp"
#include <boost/fusion/iterator/iterator_facade.hpp>
#include <boost/mpl/int.hpp>
#include <boost/mpl/minus.hpp>
#include <boost/fusion/include/tag_of_fwd.hpp>







namespace boost{	namespace fusion{	struct random_access_traversal_tag;		}	}


namespace GenericSim{
	namespace BoostFusionBinding{
		struct entity_tag;

		template< typename _Entity, int N >
		struct entity_iterator: boost::fusion::iterator_facade< entity_iterator<_Entity,N>, boost::fusion::random_access_traversal_tag >
		{
		private:
			_Entity &m_e;
		public:
			typedef _Entity entity_type;
			typedef boost::mpl::int_<N> index;

			entity_iterator(_Entity &e):m_e(e){}
			
			template< typename _Itr >
			struct value_of
			{
				typedef typename GenericSim::Meta::GetEntityStateType<typename _Itr::entity_type, _Itr::index::value>::Type type;
			};

			template< typename _Itr >
			struct deref
			{
				typedef typename GenericSim::Meta::GetEntityAtResultType<typename _Itr::entity_type, _Itr::index::value>::Type type;
				static type call(_Itr const &i){ 
					return i.m_e.at_e<typename GenericSim::Meta::GetEntityLabels<typename _Itr::entity_type>::Type(_Itr::index::value)>(); 
					//return GenericSim::Entities::At_E
					//<
					//	typename GenericSim::Meta::GetEntityLabels<typename _Itr::entity_type>::Type(_Itr::index::value)
					//>(i.m_e);
				}
			};

			template< typename _Itr, typename N >
			struct advance
			{
				typedef entity_iterator<typename _Itr::entity_type, _Itr::index::value+N::value> type;
				static type call(_Itr const &i){ return type(i.m_e); }
			};

			template <typename _Itr>
			struct next : advance<_Itr, boost::mpl::int_<1> > {};

			template <typename _Itr>
			struct prior : advance<_Itr, boost::mpl::int_<-1> > {};

			template <typename I1, typename I2>
			struct distance : boost::mpl::minus<typename I2::index, typename I1::index>
			{
				typedef typename
					boost::mpl::minus<
						typename I2::index, typename I1::index
					>::type 
				type;

				static type
				call(I1 const&, I2 const&)
				{
					return type();
				}
			};
		};
	}
}


namespace boost{	namespace fusion{


	namespace traits
	{
		template< typename _Entity >
		struct tag_of< _Entity, typename boost::enable_if<typename GenericSim::Meta::IsEntity<_Entity>,void>::type >
		//struct tag_of< _Entity, typename GenericSim::Meta::EnableOnEntity<_Entity,void>::Type >
		//struct tag_of< _Entity, typename boost::mpl::pair<typename GenericSim::Meta::EntityCheck<_Entity>::Type, void>::second >
		{
			typedef GenericSim::BoostFusionBinding::entity_tag type;
		};

		//template< typename _Entity, int N>
		//struct tag_of< GenericSim::BoostFusionBinding::entity_iterator<_Entity,N> >{
		//	typedef GenericSim::BoostFusionBinding::entity_iterator_tag type;
		//};
	}

	namespace extension
	{
		
		template<typename T>
		struct category_of_impl;

		template<>
		struct category_of_impl<GenericSim::BoostFusionBinding::entity_tag>
		{
			template<typename _Entity>
			struct apply
			{
				typedef random_access_traversal_tag type;
			};
		};




        template<typename T>
        struct at_impl;

		template<>
		struct at_impl<GenericSim::BoostFusionBinding::entity_tag>
		{
			template<typename _Entity, typename N>
			struct apply
			{
				typedef typename GenericSim::Meta::GetEntityAtResultType<_Entity,N::value>::Type type;
				static type call(_Entity &e)
				{
					return e.at_e<typename GenericSim::Meta::GetEntityLabels<_Entity>::Type(N::value)>();
					//return GenericSim::Entities::At_E
					//<
					//	typename GenericSim::Meta::GetEntityLabels<_Entity>::Type(N::value)
					//>(e);
				}
			};
		};




		template<typename T>
		struct begin_impl;

		template<>
		struct begin_impl<GenericSim::BoostFusionBinding::entity_tag>
		{
			template<typename _Entity>
			struct apply
			{
				typedef GenericSim::BoostFusionBinding::entity_iterator<_Entity,0> type;
				static type call(_Entity &e)
				{
					return type(e);
				}
			};
		};



        template <typename Tag>
        struct end_impl;

		template<>
		struct end_impl<GenericSim::BoostFusionBinding::entity_tag>
		{
			template<typename _Entity>
			struct apply
			{
				typedef GenericSim::BoostFusionBinding::entity_iterator<_Entity,GenericSim::Meta::GetEntityDimension<_Entity>::Value> type;
				static type call(_Entity &e)
				{
					return type(e);
				}
			};
		};



		template<typename Tag>
        struct is_sequence_impl;

		template<>
		struct is_sequence_impl<GenericSim::BoostFusionBinding::entity_tag>
		{
			template<typename _Entity>
			struct apply: mpl::true_{};
		};



		template<typename Tag>
        struct is_view_impl;

        template<>
		struct is_view_impl<GenericSim::BoostFusionBinding::entity_tag>
        {
            template<typename _Entity>
            struct apply: mpl::false_{};
        };



        template<typename T>
        struct size_impl;

		template<>
        struct size_impl<GenericSim::BoostFusionBinding::entity_tag>
        {
            template<typename _Entity>
			struct apply: mpl::int_<GenericSim::Meta::GetEntityDimension<_Entity>::Value> {};
        };



		template<typename T>
        struct value_at_impl;

        template <>
		struct value_at_impl<GenericSim::BoostFusionBinding::entity_tag>
		{
			template<typename _Entity, typename N>
			struct apply
			{
				typedef typename GenericSim::Meta::GetEntityStateType<_Entity,N::value>::Type type;
			};
		};

	}
}}





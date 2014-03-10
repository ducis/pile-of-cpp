#pragma once

#include "static_assert.hpp"
#include <boost/fusion/include/size.hpp>
#include <boost/fusion/include/at_c.hpp>
#include <boost/fusion/include/value_at.hpp>
//#include <EntityImplGetSetInTermsOfAt.hpp>

namespace GenericSim{
	namespace Entities{
		template<class _EntityConcept, class _RandomAccessSequence>
		class ConceptedRASeq/*:public EntityImplGetSetInTermsOfAt<ConceptedRASeq>*/{
		public:
			typedef _RandomAccessSequence seq_type;
			typedef _EntityConcept entity_concept_type;

			template <typename entity_concept_type::Labels N>
			struct AtResultType{	typedef typename boost::fusion::result_of::at_c<seq_type,N>::type  Type;  };

			template <typename entity_concept_type::Labels N>
			struct ConstAtResultType{	 typedef typename boost::fusion::result_of::at_c<seq_type const,N>::type  Type;  };

			template <typename entity_concept_type::Labels N>
			struct StateType{	typedef typename boost::fusion::result_of::value_at_c<seq_type,N>::type Type;  };

			template <typename entity_concept_type::Labels N>
			typename AtResultType<N>::Type at_e()
			{
				return boost::fusion::at_c<N>(m_seq);
			}

			template <typename entity_concept_type::Labels N>
			typename ConstAtResultType<N>::Type  at_e() const
			{
				return boost::fusion::at_c<N>(m_seq);
			}

			ConceptedRASeq(const seq_type &seq):m_seq(seq){}
			ConceptedRASeq(){}

			STATIC_ASSERT( boost::fusion::result_of::size<seq_type>::value == entity_concept_type::NDIM );
		private:
			seq_type m_seq;
		};
	}
}

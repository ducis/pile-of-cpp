#pragma once

namespace GenericSim{
	template<typename _Entity>
	struct EntityImplGetSetInTermsOfAt
	{
		template <typename _Entity::entity_concept_type::Labels N>
		struct GetResultType
		{
			typedef typename _Entity::ConstAtResultType
		};

		template <typename _Entity::entity_concept_type::Labels N, typename T>
		void Set(const T &in)
		{
			boost::fusion::at_c<N>(m_seq) = in;
		}

	};
};
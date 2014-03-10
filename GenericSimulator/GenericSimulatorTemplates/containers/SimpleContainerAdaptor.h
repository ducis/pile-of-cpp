#pragma once

namespace GenericSim{
	namespace Containers{
		template<class _SimpleContainer,class _EntityConcept> //References can be passed as reference objects
		class SimpleContainerAdaptor{
		public:

		private:
			typedef _SimpleContainer container_type;
			typedef typename container_type::value_type entity_type;
			container_type m_container;
		};
	}
}

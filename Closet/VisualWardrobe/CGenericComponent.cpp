#include "CGenericComponent.h"
#include "ptree.h"

CGenericComponent::CGenericComponent(const boost::property_tree::wptree &tr):m_name(tr.get<std::wstring>(L"Name")){
}

const std::wstring &CGenericComponent::GetName() const{
	return m_name;
}

#pragma once

#include <string>
#include <boost/property_tree/ptree_fwd.hpp>

class CGenericComponent{//����е�����Ļ���
public:
	const std::wstring &GetName() const;
	CGenericComponent(const boost::property_tree::wptree &ptr);
private:
	std::wstring m_name;
};

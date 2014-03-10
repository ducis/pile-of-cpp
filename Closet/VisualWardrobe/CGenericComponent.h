#pragma once

#include <string>
#include <boost/property_tree/ptree_fwd.hpp>

class CGenericComponent{//领带盒等物体的基类
public:
	const std::wstring &GetName() const;
	CGenericComponent(const boost::property_tree::wptree &ptr);
private:
	std::wstring m_name;
};

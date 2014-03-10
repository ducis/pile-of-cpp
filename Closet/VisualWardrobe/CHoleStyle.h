#pragma once

#include "IHoleStyle.h"
#include <boost/property_tree/ptree_fwd.hpp> 
#include <memory>

class CHoleStyle:public IHoleStyle{//一般的从xml生成的孔式样，直接从xml中读取相应名字的值
public:
	virtual float GetDiameter() const;
	virtual float GetDepth() const;
	virtual const std::pair<float,IHoleStyle*> GetSecondaryHole() const;
	virtual float GetExtrude() const;
	explicit CHoleStyle(const boost::property_tree::wptree &tr);
private:
	float m_diameter;
	float m_depth;
	float m_extrude;
	float m_secondaryOffset;
	std::auto_ptr<CHoleStyle> m_pSecondary;
};

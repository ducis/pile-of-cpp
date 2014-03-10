#include "CHoleStyle.h"
#include "ptree.h"

CHoleStyle::CHoleStyle(const boost::property_tree::wptree &tr):
m_diameter(tr.get<float>(L"Diameter")),
m_depth(tr.get<float>(L"Depth")),
m_extrude(tr.get<float>(L"Extrude",0)),
m_secondaryOffset(tr.get<float>(L"SecondaryOffset",0))
{
	using namespace boost::property_tree;
	wptree::const_assoc_iterator i( tr.find( L"Secondary" ) );
	if( i != tr.not_found() ){
		m_pSecondary.reset( new CHoleStyle(i->second) );
	}
}



float CHoleStyle::GetDiameter() const{
	return m_diameter;
}



float CHoleStyle::GetDepth() const{
	return m_depth;
}



const std::pair<float,IHoleStyle*> CHoleStyle::GetSecondaryHole() const{
	return std::make_pair(m_secondaryOffset,m_pSecondary.get());
}



float CHoleStyle::GetExtrude() const{
	return m_extrude;
}


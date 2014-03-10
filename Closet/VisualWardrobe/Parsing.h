#pragma once

namespace Parsing{
	//读Generator信息的函数

	template < typename T > const T Evaluate(const boost::property_tree::wptree &g,const boost::property_tree::wptree &p, const std::wstring &prop, const T &defaultValue)
	{
		//读不到就返回输入的默认值
		boost::optional<const boost::property_tree::wptree &> r( g.get_child_optional(prop) );
		if(r.is_initialized())
			return Evaluate<T>(*r,p);
		else
			return defaultValue;
	}

	template < typename T > const T Evaluate(const boost::property_tree::wptree &g,const boost::property_tree::wptree &p, const std::wstring &prop)
	{
		//用prop在g(Generator)中定位
		return Evaluate<T>(g.get_child(prop),p);
	}

	template < typename T > static const T Evaluate( const boost::property_tree::wptree &tr, const boost::property_tree::wptree &p )
	{ 
		//如果tr节点(一般是属于Generator)的字符串能够直接装化为T类型的变量那就返回这个值, 否则就用tr节点(一般是属于ManufactureProperty)的字符串作为名字到p中找相应的值.
		boost::optional<T> r( tr.get_value_optional<T>() );
		if(r.is_initialized()) return *r;
		else return L'-'==tr.data()[0] ? -p.get<T>(std::wstring(tr.data().begin()+1 , tr.data().end())) : p.get<T>(tr.data());
	}

}
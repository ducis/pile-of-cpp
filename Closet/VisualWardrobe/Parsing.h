#pragma once

namespace Parsing{
	//��Generator��Ϣ�ĺ���

	template < typename T > const T Evaluate(const boost::property_tree::wptree &g,const boost::property_tree::wptree &p, const std::wstring &prop, const T &defaultValue)
	{
		//�������ͷ��������Ĭ��ֵ
		boost::optional<const boost::property_tree::wptree &> r( g.get_child_optional(prop) );
		if(r.is_initialized())
			return Evaluate<T>(*r,p);
		else
			return defaultValue;
	}

	template < typename T > const T Evaluate(const boost::property_tree::wptree &g,const boost::property_tree::wptree &p, const std::wstring &prop)
	{
		//��prop��g(Generator)�ж�λ
		return Evaluate<T>(g.get_child(prop),p);
	}

	template < typename T > static const T Evaluate( const boost::property_tree::wptree &tr, const boost::property_tree::wptree &p )
	{ 
		//���tr�ڵ�(һ��������Generator)���ַ����ܹ�ֱ��װ��ΪT���͵ı����Ǿͷ������ֵ, �������tr�ڵ�(һ��������ManufactureProperty)���ַ�����Ϊ���ֵ�p������Ӧ��ֵ.
		boost::optional<T> r( tr.get_value_optional<T>() );
		if(r.is_initialized()) return *r;
		else return L'-'==tr.data()[0] ? -p.get<T>(std::wstring(tr.data().begin()+1 , tr.data().end())) : p.get<T>(tr.data());
	}

}
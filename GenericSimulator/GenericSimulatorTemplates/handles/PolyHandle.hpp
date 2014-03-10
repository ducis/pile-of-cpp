#pragma once

#include "Meta/handle.hpp"
#include <boost/shared_ptr.hpp>
#include <boost/noncopyable.hpp>
#include <boost/type_traits/is_same.hpp>
#include "static_assert.hpp"
#include <typeinfo.h>
#include <cassert>

namespace GenericSim{ namespace Handles{

	template< typename _Element >
	class PolyHandle{//: boost::noncopyable{
	public:
		typedef _Element element_type;
		element_type &operator*() const{ assert(m_pI.get()); return **m_pI; }
		element_type *operator->() const{ return &**this; }
		template< typename H > explicit PolyHandle( const H &h ):m_pI(new ConcreteWrapper<H>(h)){}
		PolyHandle(const PolyHandle &ph):m_pI(ph.m_pI){}
		friend bool operator<(const PolyHandle &l, const PolyHandle &r){
			const type_info &tL = typeid(*l.m_pI), &tR = typeid(*r.m_pI);
			//return l.m_hR<r.m_hR ? true : r.m_hR<l.m_hR ? false : l.m_id<r.m_id;
			return tL.before(tR) ? true : tR.before(tL) ? false : (*l.m_pI)<(*r.m_pI);
		}
	private:
		class Interface: boost::noncopyable{
		public:
			virtual element_type &operator*() const=0;
			virtual bool operator<(const Interface &) const=0;
			virtual ~Interface(){}
		};
		template< typename H >
		class ConcreteWrapper:public Interface{
			STATIC_ASSERT(( boost::is_same< typename GenericSim::Meta::GetHandleElementType<H>::Type, element_type >::value ));
			H m_h;
		public:
			explicit ConcreteWrapper( const H &h ):m_h(h){}
			element_type &operator*() const{ return *m_h; }
			bool operator<(const Interface &r) const{
				assert(dynamic_cast<const ConcreteWrapper *>(&r));
				return m_h<static_cast<const ConcreteWrapper &>(r).m_h;
			}
		};
		boost::shared_ptr<Interface> m_pI;  //copy constructor??
	};

	template< typename H > PolyHandle< typename GenericSim::Meta::GetHandleElementType<H>::Type > MakePoly( const H &h ){
		return PolyHandle< typename GenericSim::Meta::GetHandleElementType<H>::Type >(h);
	}
}};

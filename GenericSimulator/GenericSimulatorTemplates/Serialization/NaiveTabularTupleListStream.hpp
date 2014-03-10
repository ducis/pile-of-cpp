#pragma once

#include <boost/mpl/bool_fwd.hpp>
#include <iterator>
#include <algorithm>
#include <boost/ref.hpp>
#include <boost/fusion/include/tag_of.hpp>
#include <boost/fusion/include/size.hpp>
#include <boost/utility/enable_if.hpp>
#include <boost/fusion/include/pop_front.hpp>
#include <boost/fusion/include/is_sequence.hpp>
#include <boost/ref.hpp>
#include <boost/fusion/include/for_each.hpp>
#include <functional>
#include <boost/lambda/lambda.hpp>
#include <boost/bind.hpp>
#include <boost/mpl/pair.hpp>


namespace NaiveTabularTupleListStream
{
		

	template<typename T, typename _Enable = void>
	struct IsList: boost::mpl::false_{};

	template<typename _List>
	struct IsList<_List, typename boost::mpl::pair<typename _List::iterator, typename boost::mpl::pair<typename _List::const_iterator, void>::second>::second>: 
		boost::mpl::true_{};

	//template<typename T, typename A>
	//struct ContainerIsList< std::deque<T,A> >: boost::mpl::true_{};
	//template<typename T, typename A>
	//struct ContainerIsList< std::list<T,A> >: boost::mpl::true_{};
	//template<typename T, typename A>
	//struct IsList< std::vector<T,A> >: boost::mpl::true_{};


	template < typename S, typename _Delimiter >
	class ListStreamAdaptor
	{
	private:
		S m_s;
		_Delimiter m_d;
	public:
		explicit ListStreamAdaptor( const S &stream, const _Delimiter &delimiter ):m_s(stream),m_d(delimiter){}
		
		bool operator!( ) const{ return !boost::unwrap_ref(m_s); }

		template< typename T >
		typename boost::enable_if< IsList<T>, ListStreamAdaptor & >::type operator<<(const T &t)
		{
			for(typename T::const_iterator i = t.begin(); i!=t.end(); ++i){
				boost::unwrap_ref(m_s)<<*i<<m_d;
			}
			return *this;
		}
		template< typename T >
		typename boost::disable_if< IsList<T>, ListStreamAdaptor & >::type operator<<(const T &t)
		{
			boost::unwrap_ref(m_s)<<t;
			return *this;
		}

		template< typename T >
		typename boost::enable_if< IsList<T>, ListStreamAdaptor & >::type operator>>(T &t)
		{
			t.clear();
			while(true){
				typename T::value_type temp;
				boost::unwrap_ref(m_s)>>temp;
				if(!boost::unwrap_ref(m_s)) break;
				t.push_back(temp);
			}
			return *this;
		}
		template< typename T >
		typename boost::disable_if< IsList<T>, ListStreamAdaptor & >::type operator>>(T &t)
		{
			boost::unwrap_ref(m_s)>>t;
			return *this;
		}
	};

	template < typename S >
	ListStreamAdaptor< boost::reference_wrapper<S>, const char * > 
	UseAsListStream( S &s ){
		return ListStreamAdaptor< boost::reference_wrapper<S>, const char * >( boost::ref(s), "\n" );
	}
		
	template < typename S, typename _Delimiter >
	class TupleStreamAdaptor
	{
	private:
		S m_s;
		_Delimiter m_d;
		template <typename T>
		typename boost::enable_if_c< (boost::fusion::result_of::size<T>::value>1) >::type
		OutputTupleImpl( const T &t )
		{
			*this<<boost::fusion::at_c<0>(t)<<m_d<<boost::fusion::pop_front(t);
		}

		template <typename T>
		typename boost::enable_if_c< (boost::fusion::result_of::size<T>::value==1) >::type
		OutputTupleImpl( const T &t )
		{
			*this<<boost::fusion::at_c<0>(t);
		}

		class InputFunctor{
		private:
			TupleStreamAdaptor &s;
		public:
			InputFunctor(TupleStreamAdaptor &_s):s(_s){}
			template <typename T> void operator() ( T &t ) const{ s>>t; }
		};
	public:
		explicit TupleStreamAdaptor( const S &stream, const _Delimiter &delimiter ):m_s(stream),m_d(delimiter){}

		bool operator!( ) const{ return !boost::unwrap_ref(m_s); }

		template< typename T >
		typename boost::enable_if< boost::fusion::traits::is_sequence<T>, TupleStreamAdaptor & >::type operator<<(const T &t)
		{
			OutputTupleImpl(t);
			return *this;
		}
		template< typename T >
		typename boost::disable_if< boost::fusion::traits::is_sequence<T>, TupleStreamAdaptor & >::type operator<<(const T &t)
		{
			boost::unwrap_ref(m_s)<<t;
			return *this;
		}
			
		template <typename T>
		typename boost::enable_if< boost::fusion::traits::is_sequence<T>, TupleStreamAdaptor & >::type operator>>(T &t)
		{
			boost::fusion::for_each( t, InputFunctor(*this) );
			return *this;
		}

		template <typename T>
		typename boost::disable_if< boost::fusion::traits::is_sequence<T>, TupleStreamAdaptor & >::type operator>>(T &t)
		{
			boost::unwrap_ref(m_s)>>t;
			return *this;
		}
	};

	template < typename S >
	TupleStreamAdaptor< boost::reference_wrapper<S>, const char * > 
	UseAsTupleStream( S &s ){
		return TupleStreamAdaptor< boost::reference_wrapper<S>, const char * >( boost::ref(s), "\t" );
	}


	template < typename S >
	ListStreamAdaptor< TupleStreamAdaptor< boost::reference_wrapper<S>, const char *>, const char * > 
	UseAsTupleListStream( S &s ){
		typedef TupleStreamAdaptor< boost::reference_wrapper<S>, const char * > ts_type;
		return ListStreamAdaptor< ts_type , const char * >( 
			ts_type( boost::ref(s), "\t" ), "\n");
	}

}
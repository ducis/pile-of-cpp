#pragma once

#include <functional>

namespace Basic{
	template<size_t _Sz>
	class ConstantSize{
	public:
		operator size_t() const{
			return _Sz;
		}
	};



	template<typename T, T val>
	class Constant{
	public:
		operator T() const{
			return val;
		}
	};
	


	class Unity{
	public:
		template<typename T>
		const T &operator*(const T &in) const{
			return in;
		}
	};



	template<typename _X, typename _Y, typename _T>
	class Constant2D_T:public std::binary_function< _X,_Y,_T >{
	private:
		const _T m_f;
	public:
		Constant2D_T( const _T &f ):m_f(f){}
		const _T operator()(const _X &, const _Y &) const{
			return m_f;
		}
	};

	template<typename _X, typename _Y, typename _T>
	const Constant2D_T<_X,_Y,_T> Constant2D(const _T &f){
		return Constant2D_T<_X,_Y,_T>(f);
	}



	class StackOnly{
	private:
		void* operator new(std::size_t);
		void* operator new(std::size_t,const std::nothrow_t&);
		void* operator new(std::size_t,void*);
	};




	template<typename _Function>
	struct FunctionTraits{
		typedef typename _Function::first_argument_type FirstArgType;
		typedef typename _Function::second_argument_type SecondArgType;
		typedef typename _Function::result_type ValueType;
	};



	template<typename _2D>
	size_t SizeX(const _2D &);

	template<typename _2D>
	size_t SizeY(const _2D &);



	template<typename _T, typename _OriginalFunction>
	class ResultCast2D_T:
		public std::binary_function< 
		typename FunctionTraits<_OriginalFunction>::FirstArgType, 
		typename FunctionTraits<_OriginalFunction>::SecondArgType,
		typename FunctionTraits<_OriginalFunction>::ValueType
		>
	{
		friend size_t SizeX(const ResultCast2D_T &f){
			return SizeX(f.m_f);
		}
		friend size_t SizeY(const ResultCast2D_T &f){
			return SizeY(f.m_f);
		}
	private:
		_OriginalFunction m_f;
	public:
		ResultCast2D_T( _OriginalFunction f ):m_f(f){}
		const _T operator()(const first_argument_type &x, const second_argument_type &y) const{
			return m_f(x,y);
		}
	};

	template<typename _T, typename _OriginalFunction>
	ResultCast2D_T<_T,_OriginalFunction> ResultCast2D(_OriginalFunction f){
		return ResultCast2D_T<_T,_OriginalFunction>(f);
	}



	template<typename _T>
	class Swap2:StackOnly{
	private:
		_T m_cur;
		_T m_prev;
		bool m_swapped;
	public:
		Swap2(const _T &current,const _T &previous):m_cur(current),m_prev(previous),m_swapped(false){
		}
		_T &Current(){
			return m_swapped?m_prev:m_cur;
		}
		_T &Previous(){
			return m_swapped?m_cur:m_prev;
		}
		const _T &Current()const{
			return m_swapped?m_prev:m_cur;
		}
		const _T &Previous()const{
			return m_swapped?m_cur:m_prev;
		}
		void Swap(){
			m_swapped = !m_swapped;
		}
	};

}

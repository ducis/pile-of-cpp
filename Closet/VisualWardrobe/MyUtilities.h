#pragma once
#include <cassert>
#include "vectorTraits.h"
#include <functional>

class vector2;

namespace MyUtilities{
	template<typename T> void ClampR( T &x, const T &upperLimit ){//��x = min(x,upperLimit)
		if(upperLimit<x){
			x = upperLimit;
		}
	}
	template<typename T> void ClampL( T &x, const T &lowerLimit ){//��x = max(x,lowerLimit)
		if(lowerLimit>x){
			x = lowerLimit;
		}
	}
	template<typename T> void Clamp( T &x, const T &lowerLimit, const T &upperLimit ){//��x = min(max(x,lowerLimit),upperLimit)
		assert(lowerLimit<=upperLimit);
		if(lowerLimit>x){
			x = lowerLimit;
		}else if(upperLimit<x){
			x = upperLimit;
		}
	}
	template<typename T> const T Clamped( const T &x, const T &lowerLimit, const T &upperLimit ){//����min(max(x,lowerLimit),upperLimit)
		if(lowerLimit>x){
			return lowerLimit;
		}else if(upperLimit<x){
			return upperLimit;
		}else{
			return x;
		}
	}
	template<typename T> bool IsInClosedRange( const T &x, const T &lowerLimit, const T &upperLimit ){//����x�Ƿ�������[lowerLimit,upperLimit]��
		return lowerLimit<=x && x<=upperLimit;
	}
	template<typename T> bool IsInOpenRange( const T &x, const T &lowerLimit, const T &upperLimit ){//����x�Ƿ�������(lowerLimit,upperLimit)��
		return lowerLimit<x && x<upperLimit;
	}
	inline bool IsInClosedRect( const vector2 &p, const vector2 &lowerLimits, const vector2 &upperLimits){
		//����p�Ƿ���{(x,y)|x����[lowerLimits.x,upperLimits.x]��y����[lowerLimits.y,upperLimits.y]
		return IsInClosedRange(p.x,lowerLimits.x,upperLimits.x) && IsInClosedRange(p.y,lowerLimits.y,upperLimits.y);
	}
	inline bool IsInClosedAngle( const vector2 &m, const vector2 &l, const vector2 &r ){//m�Ƿ���l,r֮��
		return CrossProduct(m,l)>=0 && CrossProduct(r,m)>=0;
	}
	inline bool IsInOpenAngle( const vector2 &m, const vector2 &l, const vector2 &r ){//m�Ƿ���l,r֮��
		return CrossProduct(m,l)>0 && CrossProduct(r,m)>0;
	}

//�����OperationOn*��ʽ�Ķ������������f(v)=g(v.x),��g��װΪf������
	template< class Operation >
	class OperationOnX:public std::unary_function <typename VectorTraits<typename Operation::argument_type>::ScalarType, typename Operation::result_type> {
	public:
		result_type operator()( argument_type arg ){
			return m_op(arg.x);
		}
		OperationOnX( const Operation &_Func ):m_op(_Func){
		}
	protected:
		Operation m_op;
	};
	template<class Operation> OperationOnX<Operation> OperateOnX( const Operation &_Func ){
		return OperationOnX<Operation>(_Func);
	}



	template< class Operation >
	class OperationOnY:public std::unary_function <typename VectorTraits<typename Operation::argument_type>::ScalarType, typename Operation::result_type> {
	public:
		result_type operator()( argument_type arg ){
			return m_op(arg.y);
		}
		OperationOnY( const Operation &_Func ):m_op(_Func){
		}
	protected:
		Operation m_op;
	};
	template<class Operation> OperationOnY<Operation> OperateOnY( const Operation &_Func ){
		return OperationOnY<Operation>(_Func);
	}


	template< class Operation , typename V>
	class OperationOnXes:public std::binary_function<
		typename Operation::first_argument_type, 
		typename Operation::second_argument_type,
		typename Operation::result_type> 
	{
	public:
		result_type operator()( const V &a1, const V &a2 ){
			return m_op(a1.x,a2.x);
		}
		explicit OperationOnXes( const Operation &_Func ):m_op(_Func){
		}
	protected:
		Operation m_op;
	};
	template< typename V , class Operation> OperationOnXes<Operation, V> OperateOnXes( const Operation &_Func ){
		return OperationOnXes<Operation, V>(_Func);
	}



	template< class Operation , typename V>
	class OperationOnYs:public std::binary_function<
		typename Operation::first_argument_type, 
		typename Operation::second_argument_type,
		typename Operation::result_type> 
	{
	public:
		result_type operator()( const V &a1, const V &a2 ){
			return m_op(a1.y,a2.y);
		}
		explicit OperationOnYs( const Operation &_Func ):m_op(_Func){
		}
	protected:
		Operation m_op;
	};
	template< typename V , class Operation> OperationOnYs<Operation, V> OperateOnYs( const Operation &_Func ){
		return OperationOnYs<Operation , V>(_Func);
	}



	template< typename S > void SnapTo( S &x , const S &target , const S &threshold ){ //x����targetʱ��x=target
		if(abs(x-target)<threshold)		x=target;
	}

};
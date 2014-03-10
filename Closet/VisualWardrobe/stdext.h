#pragma once

namespace stdext{
	template<class InputIterator1, class InputIterator2>
	bool set_intersect(
		InputIterator1 _First1, 
		InputIterator1 _Last1,
		InputIterator2 _First2, 
		InputIterator2 _Last2
		)//�ж�����Ԫ���Ѿ���С�����ź���ļ����Ƿ��й�ͬԪ��
	{
		while( _First1 != _Last1 && _First2 != _Last2 ){
			if( *_First1 < *_First2 ){
				++_First1;
			}else if( *_First2 < *_First1 ){
				++_First2;
			}else{
				return true;
			}
		}
		return false;
	}
	template<typename _Pair> const typename _Pair::second_type get2nd(const _Pair &p){ return p.second; }
	template<typename _Pair> const typename _Pair::first_type get1st(const _Pair &p){ return p.first; }
}
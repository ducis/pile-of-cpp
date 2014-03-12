#pragma once

#include <cstdlib>
#include <algorithm>
#include <vector>

template<typename T> 
class Matrix{
public:
	Matrix(size_t nRows, size_t nColumns):m_nRows(nRows),m_nColumns(nColumns),m_data(nRows*nColumns)
	{
	}
	Matrix(size_t nRows, size_t nColumns, const T *pDataColumnMajor)
		:m_nRows(nRows),m_nColumns(nColumns),m_data(pDataColumnMajor,pDataColumnMajor+nRows*nColumns)
	{
	}
	Matrix(size_t nRows, size_t nColumns, const T &value)
		:m_nRows(nRows),m_nColumns(nColumns),m_data(nRows*nColumns,value)
	{
	}
	const T &Get(size_t r, size_t c) const{
		return m_data[r+c*m_nRows];
	}
	const T &Set(size_t r, size_t c, const T &val){
		return m_data[r+c*m_nRows] = val;
	}
	typedef typename std::vector<T>::const_iterator ext_const_iterator_type;
	typename ext_const_iterator_type Begin() const { return m_data.begin(); }
	typename ext_const_iterator_type End() const { return m_data.end(); }

	typedef typename std::vector<T>::iterator ext_iterator_type;
	typename ext_iterator_type Begin() { return m_data.begin(); }
	typename ext_iterator_type End() { return m_data.end(); }

	size_t NumRows() const{ return m_nRows; }
	size_t NumColumns() const{ return m_nColumns; }
private:
	size_t m_nRows;
	size_t m_nColumns;
	std::vector<T> m_data;
};

template< typename _OS, typename T > _OS &operator<<( _OS &os, const Matrix<T> &m ){
	for(size_t r(0); r<m.NumRows(); ++r){
		for(size_t c(0); c<m.NumColumns(); ++c){
			os<<m.Get(r,c)<<os.widen('\t');
		}
		os<<os.widen('\n');
	}
	return os;
}

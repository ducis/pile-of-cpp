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
	const T &Get(size_t r, size_t c) const{
		return m_data[r+c*m_nRows];
	}
	const T &Set(size_t r, size_t c, const T &val){
		return m_data[r+c*m_nRows] = val;
	}
	typename std::vector<T>::const_iterator Begin() const { return m_data.begin(); }
	typename std::vector<T>::const_iterator End() const { return m_data.end(); }
	size_t NumRows(){ return m_nRows; }
	size_t NumColumns(){ return m_nColumns; }
private:
	size_t m_nRows;
	size_t m_nColumns;
	std::vector<T> m_data;
};

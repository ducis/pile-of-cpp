#pragma once

#include "CDxfHeader.h"
#include <fstream>
#include <algorithm>
#include <iterator>

template< class _OutputStream >
void CDxfHeader::WriteToDXF( _OutputStream &os ){
	std::wifstream is(m_path.c_str());
	std::copy(std::istreambuf_iterator<wchar_t>(is),std::istreambuf_iterator<wchar_t>(),std::ostreambuf_iterator<wchar_t>(os));
}

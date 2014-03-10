#include "CDxfHeader.impl.h"

#include <algorithm>
#include <iterator>

using namespace std;
template<> void CDxfHeader::WriteToDXF( std::wostream &os ){
	wifstream is( m_path.c_str() );
	copy(istreambuf_iterator<wchar_t>(is),istreambuf_iterator<wchar_t>(),ostream_iterator<wchar_t,wchar_t>(os));
}


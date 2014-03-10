#pragma once

#include <string>

class CDxfHeader{
	//DXF文件头.鉴于DXF文件中的一些东西是不需要改动但是必须要输出的，
	//就把它们存到另外的文件里，需要的时候直接拷过去
public:
	template< class _OutputStream > void WriteToDXF( _OutputStream &os );
	CDxfHeader(const std::wstring &path):m_path(path){}
private:
	std::wstring m_path;
};

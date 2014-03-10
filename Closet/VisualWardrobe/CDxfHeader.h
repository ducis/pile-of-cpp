#pragma once

#include <string>

class CDxfHeader{
	//DXF�ļ�ͷ.����DXF�ļ��е�һЩ�����ǲ���Ҫ�Ķ����Ǳ���Ҫ����ģ�
	//�Ͱ����Ǵ浽������ļ����Ҫ��ʱ��ֱ�ӿ���ȥ
public:
	template< class _OutputStream > void WriteToDXF( _OutputStream &os );
	CDxfHeader(const std::wstring &path):m_path(path){}
private:
	std::wstring m_path;
};

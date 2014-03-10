#include "CIdentifier.h"


const std::wstring &CIdentifier::GetString() const{
	return m_str;
}


CIdentifier::CIdentifier(const std::wstring &str):m_str(str){
}


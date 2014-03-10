#pragma once

#include <string>

class CIdentifier{//标识符，目前就是一个字符串的Wrapper
	friend bool operator == (const CIdentifier &lhs, const CIdentifier &rhs){
		return lhs.GetString() == rhs.GetString();
	}
public:
	const std::wstring &GetString() const;
	CIdentifier(const std::wstring &str);
	bool operator<(const CIdentifier &rhs) const{ return m_str<rhs.m_str; }
private:
	std::wstring m_str;
};

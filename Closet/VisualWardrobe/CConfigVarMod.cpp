#include "CConfigVarMod.h"
#include "CProgram.h"
#include "singleton.h"

template<typename T>
CConfigVarMod<T>::CConfigVarMod(const std::wstring &name, const T &val):
m_name(name),
m_cachedVal(GetSingleton<CProgram>()->GetConfigVar<T>(name)){
	GetSingleton<CProgram>()->SetConfigVar<T>(m_name,val);
}

template<typename T>
CConfigVarMod<T>::~CConfigVarMod(){
	GetSingleton<CProgram>()->SetConfigVar<T>(m_name,m_cachedVal);
}

template class CConfigVarMod<int>;
template class CConfigVarMod<float>;
template class CConfigVarMod<bool>;
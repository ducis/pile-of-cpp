#pragma once

#include "IConnectible.h"
template< class _Wrapper >
class CConnectibleWrapper: public IConnectible
{
	//用于将没有IConnectible接口的对象包装起来并提供IConnectible接口的模版类,
	//一个类可以被多个CConnectibleWrapper包装
	//不使用
public:
	virtual bool ConnectTo(const IConnectible &theOther){
		return static_cast<_Wrapper*>(this)->GetWrappedConnectible().ConnectTo(theOther);   
	}
	virtual bool DisconnectFrom(const IConnectible &theOther){	 
		return static_cast<_Wrapper*>(this)->GetWrappedConnectible().DisconnectFrom(theOther);  
	}
};

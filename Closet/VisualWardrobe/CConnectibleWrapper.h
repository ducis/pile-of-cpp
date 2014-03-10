#pragma once

#include "IConnectible.h"
template< class _Wrapper >
class CConnectibleWrapper: public IConnectible
{
	//���ڽ�û��IConnectible�ӿڵĶ����װ�������ṩIConnectible�ӿڵ�ģ����,
	//һ������Ա����CConnectibleWrapper��װ
	//��ʹ��
public:
	virtual bool ConnectTo(const IConnectible &theOther){
		return static_cast<_Wrapper*>(this)->GetWrappedConnectible().ConnectTo(theOther);   
	}
	virtual bool DisconnectFrom(const IConnectible &theOther){	 
		return static_cast<_Wrapper*>(this)->GetWrappedConnectible().DisconnectFrom(theOther);  
	}
};

#pragma once

class IConnectible{
	//���Ƕ��ڿ������ӵ�����(�������������ƴ��һ��)�����Ӻ�
	//���붯��(�����ƴ��һ��ʱ������Ҫ�޸�һЩ��ĳ���)�Ľӿ�
public:
	virtual bool ConnectTo(const IConnectible &theOther) = 0;//���ӣ�����ʱa->ConnectTo(b)��b->ConnectTo(a)��Ҫ����
	virtual bool DisconnectFrom(const IConnectible &theOther) = 0;//���룬����ʱa->DisconnectFrom(b)��b->DisconnectFrom(a)��Ҫ����
protected:
	IConnectible(){}
	~IConnectible(){}
};

#pragma once

class IConnectible{
	//这是对于可以连接的物体(如两个柜体可以拼在一起)的连接和
	//分离动作(如柜体拼在一起时可能需要修改一些板的长度)的接口
public:
	virtual bool ConnectTo(const IConnectible &theOther) = 0;//连接，连接时a->ConnectTo(b)和b->ConnectTo(a)都要调用
	virtual bool DisconnectFrom(const IConnectible &theOther) = 0;//分离，分离时a->DisconnectFrom(b)和b->DisconnectFrom(a)都要调用
protected:
	IConnectible(){}
	~IConnectible(){}
};

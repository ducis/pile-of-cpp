#pragma once
//��ʹ��
class ISimplyNotified{
public:
	virtual void OnNotifierStateChange();
	virtual void OnNotifierDeath();
};


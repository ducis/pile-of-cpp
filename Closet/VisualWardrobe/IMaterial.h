#pragma once


class IMaterial{//���ʻ���
public:
	virtual void Apply() const = 0;//��Ⱦǰ���ô˺������趨��Ӧ����Ⱦ״̬
	virtual const class CIdentifier &GetIdentifier() const = 0;
};

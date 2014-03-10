#pragma once

#include <utility>

class IHoleStyle{//���Ե�ϵĿ׵Ľӿڣ������˿��ܳ��ֵ�һ�����䴹ֱ���ཻ�Ŀ׵���Ϣ��
public:
	virtual float GetDiameter() const =0;//���׵�ֱ��
	virtual float GetDepth() const =0;//���׵����
	virtual float GetExtrude() const =0;//��������Ե��Ǹ������ϻ�Ҫ�����
	virtual const std::pair<float,IHoleStyle*> GetSecondaryHole() const =0;//���ܳ��ֵ������״�ֱ�ཻ�ĸ���,first�Ǹ����������ߵľ���(���׵��������Ӧ�����Ƿ���(0,NULL)
protected:
	~IHoleStyle(){}
};



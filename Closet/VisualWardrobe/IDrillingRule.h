#pragma once

class IDrillingRule{//�Ӱ���صĳ��ȵ�Ӧ�����ɵĿ׵�����,λ�ú���״��ӳ��
public:
	virtual void Generate(float dim) = 0;//������������趨����صĳ���,Ȼ��������漸��������ÿ׵���Ϣ
	virtual unsigned int GetHoleCount() const = 0;
	virtual class IHoleStyle *GetHoleStyle(unsigned int i) const = 0;
	virtual float GetHolePos(unsigned int i) const = 0;
};

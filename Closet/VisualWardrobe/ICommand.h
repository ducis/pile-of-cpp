#pragma once

#include <string>

#include <istream>
#include <ostream>

class ICommand{//����,����DLL������ַ�������Ľӿ�
public:
	virtual int Invoke(class CProgram *pProgram, std::wistream &arg, std::wostream &log) const = 0;
	//ִ�У�pProgram�Ǹ�������������һ��CProgram(ʵ���ϴ������Ѿ���������ֱ�ӵ��õ�GetSingleton�ˣ���������ط�ȷʵ���á��������涼�ú������)��
	//arg�ǲ�����log�������Ϣ������ֵ��Ϊ0���ǳɹ���
};

#pragma once;
#include "singleton.h"

//������������ʽ��ʵ����,��֪����Ҫ����singleton��CPP�ļ���Ű������Դ�ļ�
//һ�����singleton�ĵط�����singleton.h����,һ�������singleton������,
//��һ�����ֹ��Ϊ��DLL�����ֶ��singleton֮�������.

template <class CLASS>
CLASS *GetSingleton(){
	static CLASS object;
	return &object;
}

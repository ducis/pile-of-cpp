// GenericSimulatorDLL.cpp : ���� DLL Ӧ�ó���ĵ���������
//

#include "stdafx.h"
#include "GenericSimulatorDLL.h"


// ���ǵ���������һ��ʾ��
GENERICSIMULATORDLL_API int nGenericSimulatorDLL=0;

// ���ǵ���������һ��ʾ����
GENERICSIMULATORDLL_API int fnGenericSimulatorDLL(void)
{
	return 42;
}

// �����ѵ�����Ĺ��캯����
// �й��ඨ�����Ϣ������� GenericSimulatorDLL.h
CGenericSimulatorDLL::CGenericSimulatorDLL()
{
	return;
}

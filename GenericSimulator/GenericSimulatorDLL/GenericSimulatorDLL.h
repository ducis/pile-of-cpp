// ���� ifdef ���Ǵ���ʹ�� DLL �������򵥵�
// ��ı�׼�������� DLL �е������ļ��������������϶���� GENERICSIMULATORDLL_EXPORTS
// ���ű���ġ���ʹ�ô� DLL ��
// �κ�������Ŀ�ϲ�Ӧ����˷��š�������Դ�ļ��а������ļ����κ�������Ŀ���Ὣ
// GENERICSIMULATORDLL_API ������Ϊ�Ǵ� DLL ����ģ����� DLL ���ô˺궨���
// ������Ϊ�Ǳ������ġ�
#ifdef GENERICSIMULATORDLL_EXPORTS
#define GENERICSIMULATORDLL_API __declspec(dllexport)
#else
#define GENERICSIMULATORDLL_API __declspec(dllimport)
#endif

// �����Ǵ� GenericSimulatorDLL.dll ������
class GENERICSIMULATORDLL_API CGenericSimulatorDLL {
public:
	CGenericSimulatorDLL(void);
	// TODO: �ڴ�������ķ�����
};

extern GENERICSIMULATORDLL_API int nGenericSimulatorDLL;

GENERICSIMULATORDLL_API int fnGenericSimulatorDLL(void);

#include <windows.h>
#include <D3D9.h>
#include <D3DX9.h>
class GRunTimeDataPack;
class MyMaterialExtra{
public:
	HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR(HK_MEMORY_CLASS_SCENE_DATA,MyMaterialExtra);
	HK_DECLARE_REFLECTION(); 
public:
	GRunTimeDataPack *m_pRunTime;
	MyMaterialExtra():m_pRunTime(NULL){};
};

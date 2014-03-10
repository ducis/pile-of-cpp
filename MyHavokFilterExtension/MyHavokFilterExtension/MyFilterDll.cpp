#include "stdafx.h"
#include "MyFilterDll.h"
#include "MyMeshConverter.h"
#include "MyUVInverter.h"
#include <Common/Serialize/Util/hkBuiltinTypeRegistry.h>

//#include <Common/Serialize/Version/hkVersionRegistry.h>
#include <Common/Base/KeyCode.h>
 
// Register versioning information.
//#define HK_COMPAT_FILE <Common/Compat/hkCompatVersions.h>
//#include <Common/Compat/hkCompat_All.cxx> 
extern const hkClass MyMeshConverterClass;
extern const hkClass D3DVERTEXELEMENT9Class;
MyFilterDll::MyFilterDll(HMODULE dllModule) : hctFilterDll (dllModule) 
{

}

int MyFilterDll::getNumberOfFilters() const
{
	return 2;
}

hctFilterDescriptor* MyFilterDll::getFilterDescriptor (int index) const
{
	static hctFilterDescriptor* m_filterDescs[] = 
	{
		&g_MyMeshConverterDesc,
		&g_MyUVInverterDesc
	};

	return m_filterDescs[index];
}

/*void MyFilterDll::registerClasses (hctFilterClassRegistry& classReg) const
{
	classReg.registerClasses (hkBuiltinTypeRegistry::StaticLinkedTypeInfos, hkBuiltinTypeRegistry::StaticLinkedClasses);

	#define HK_CUSTOM_CLASS(C) \
		extern const hkTypeInfo C##TypeInfo;					\
		extern const hkClass C##Class;							\
		classReg.registerClass( &(C##TypeInfo), &(C##Class) );	

	// Our phantom class
	HK_CUSTOM_CLASS(MySimpleBuffer)
	HK_CUSTOM_CLASS(MyVertElement)
	HK_CUSTOM_CLASS(MyVertexDataStorage)
	HK_CUSTOM_CLASS(MyMaterialExtra)
//	HK_CUSTOM_CLASS(D3DVERTEXELEMENT9)
//	HK_CUSTOM_CLASS(MyPhantomShape)
}*/


void MyFilterDll::initDll( const hkMemoryInitUtil::SyncInfo& baseSystemInfo, hkError* errorStream )
{

	hctFilterDll::initDll(baseSystemInfo, errorStream);


	// HVK-3632
	//hkpWorldCinfo defaults;
	//hkpWeldingUtility::initWeldingTable(defaults.m_snapCollisionToConvexEdgeThreshold, defaults.m_snapCollisionToConcaveEdgeThreshold);
}

__declspec( dllexport ) hctFilterDll* getFilterDll (HMODULE dllModule)
{
	static MyFilterDll gFilterAssetDll (dllModule);

	return &gFilterAssetDll;
}

#define HK_CLASSES_FILE <ContentTools/Common/Filters/FilterTutorial/hctFilterTutorialClasses.h>
#include <Common/Serialize/Util/hkBuiltinTypeRegistry.cxx> 
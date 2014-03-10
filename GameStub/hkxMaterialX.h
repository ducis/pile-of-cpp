#include <Common/SceneData/Material/hkxMaterial.h>
#include <Common/SceneData/Attributes/hkxAttribute.h>
#include "GNamedVarManager.h"
class hkxMaterialX:public hkxMaterial{
public:
	void SetDeviceState();
	void RecordDeviceState();
	void ReportUnusedAttribute(const hkxAttribute *attribute,float time);
	void ParseAttribute_UseFX(const hkxAttribute *attribute,float time);
	//static void InitAttributeParsers();
	static char *AttributeNameList[];
	static void (hkxMaterialX::*AttributeParserList[])(const hkxAttribute *,float);
	static GNamedVarManager<void (hkxMaterialX::*)(const hkxAttribute *,float),&ReportUnusedAttribute,NULL,AttributeNameList,AttributeParserList> hkxMaterialX::AttributeParserManager;
};

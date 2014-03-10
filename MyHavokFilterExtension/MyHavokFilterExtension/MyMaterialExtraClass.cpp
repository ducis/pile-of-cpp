// TKBMS v1.0 -----------------------------------------------------
//
// PLATFORM   : ALL
// PRODUCT    : PHYSICS
// VISIBILITY : PUBLIC
//
// ------------------------------------------------------TKBMS v1.0

// WARNING: THIS FILE IS GENERATED. EDITS WILL BE LOST.
// Generated from 'D:/MyDemo/Common/MyMaterialExtra.h'
#include <Common/Base/hkBase.h>
#include <Common/Base/Reflection/hkClass.h>
#include <Common/Base/Reflection/hkInternalClassMember.h>
#include <Common/Base/Reflection/hkTypeInfo.h>
#include <D:/MyDemo/Common/MyMaterialExtra.h>
#define True true
#define False false



//
// Class MyMaterialExtra
//
static const hkInternalClassMember MyMaterialExtraClass_Members[] =
{
	{ "pRunTime", HK_NULL, HK_NULL, hkClassMember::TYPE_POINTER, hkClassMember::TYPE_UINT32, 0, 0, HK_OFFSET_OF(MyMaterialExtra,m_pRunTime), HK_NULL }
};
extern const hkClass MyMaterialExtraClass;
const hkClass MyMaterialExtraClass(
	"MyMaterialExtra",
	HK_NULL, // parent
	sizeof(MyMaterialExtra),
	HK_NULL,
	0, // interfaces
	HK_NULL,
	0, // enums
	reinterpret_cast<const hkClassMember*>(MyMaterialExtraClass_Members),
	HK_COUNT_OF(MyMaterialExtraClass_Members),
	HK_NULL, // defaults
	HK_NULL, // attributes
	0, // flags
	0 // version
	);
HK_REFLECTION_DEFINE_SIMPLE(MyMaterialExtra);

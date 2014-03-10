// TKBMS v1.0 -----------------------------------------------------
//
// PLATFORM   : ALL
// PRODUCT    : PHYSICS
// VISIBILITY : PUBLIC
//
// ------------------------------------------------------TKBMS v1.0

// WARNING: THIS FILE IS GENERATED. EDITS WILL BE LOST.
// Generated from 'D:/MyDemo/Common/SerializedObjects.h'
#include "DXUT.h"
#include <Common/Base/hkBase.h>
#include <Common/Base/Reflection/hkClass.h>
#include <Common/Base/Reflection/hkInternalClassMember.h>
#include <Common/Base/Reflection/hkTypeInfo.h>
#include <D:/MyDemo/Common/SerializedObjects.h>
#define True true
#define False false



//
// Class MyVertElement
//
static const hkInternalClassMember MyVertElementClass_Members[] =
{
	{ "Stream", HK_NULL, HK_NULL, hkClassMember::TYPE_UINT16, hkClassMember::TYPE_VOID, 0, 0, HK_OFFSET_OF(MyVertElement,m_Stream), HK_NULL },
	{ "Offset", HK_NULL, HK_NULL, hkClassMember::TYPE_UINT16, hkClassMember::TYPE_VOID, 0, 0, HK_OFFSET_OF(MyVertElement,m_Offset), HK_NULL },
	{ "Type", HK_NULL, HK_NULL, hkClassMember::TYPE_UINT8, hkClassMember::TYPE_VOID, 0, 0, HK_OFFSET_OF(MyVertElement,m_Type), HK_NULL },
	{ "Method", HK_NULL, HK_NULL, hkClassMember::TYPE_UINT8, hkClassMember::TYPE_VOID, 0, 0, HK_OFFSET_OF(MyVertElement,m_Method), HK_NULL },
	{ "Usage", HK_NULL, HK_NULL, hkClassMember::TYPE_UINT8, hkClassMember::TYPE_VOID, 0, 0, HK_OFFSET_OF(MyVertElement,m_Usage), HK_NULL },
	{ "UsageIndex", HK_NULL, HK_NULL, hkClassMember::TYPE_UINT8, hkClassMember::TYPE_VOID, 0, 0, HK_OFFSET_OF(MyVertElement,m_UsageIndex), HK_NULL }
};
extern const hkClass MyVertElementClass;
const hkClass MyVertElementClass(
	"MyVertElement",
	HK_NULL, // parent
	sizeof(MyVertElement),
	HK_NULL,
	0, // interfaces
	HK_NULL,
	0, // enums
	reinterpret_cast<const hkClassMember*>(MyVertElementClass_Members),
	HK_COUNT_OF(MyVertElementClass_Members),
	HK_NULL, // defaults
	HK_NULL, // attributes
	0, // flags
	0 // version
	);
HK_REFLECTION_DEFINE_SIMPLE(MyVertElement);

//
// Class MySimpleBuffer
//
static const hkInternalClassMember MySimpleBufferClass_Members[] =
{
	{ "data", HK_NULL, HK_NULL, hkClassMember::TYPE_SIMPLEARRAY, hkClassMember::TYPE_UINT8, 0, 0, HK_OFFSET_OF(MySimpleBuffer,m_data), HK_NULL }
};
extern const hkClass MySimpleBufferClass;
const hkClass MySimpleBufferClass(
	"MySimpleBuffer",
	HK_NULL, // parent
	sizeof(MySimpleBuffer),
	HK_NULL,
	0, // interfaces
	HK_NULL,
	0, // enums	
	reinterpret_cast<const hkClassMember*>(MySimpleBufferClass_Members),
	HK_COUNT_OF(MySimpleBufferClass_Members),
	HK_NULL, // defaults
	HK_NULL, // attributes
	0, // flags
	0 // version
	);
HK_REFLECTION_DEFINE_SIMPLE(MySimpleBuffer);

//
// Class MyVertexDataStorage
//
static const hkInternalClassMember MyVertexDataStorageClass_Members[] =
{
	{ "Vbs", HK_NULL, HK_NULL, hkClassMember::TYPE_SIMPLEARRAY, hkClassMember::TYPE_UINT32, 0, 0, HK_OFFSET_OF(MyVertexDataStorage,m_Vbs), HK_NULL },
	{ "Ibs", HK_NULL, HK_NULL, hkClassMember::TYPE_SIMPLEARRAY, hkClassMember::TYPE_UINT32, 0, 0, HK_OFFSET_OF(MyVertexDataStorage,m_Ibs), HK_NULL },
	{ "Dec", HK_NULL, HK_NULL, hkClassMember::TYPE_UINT32, hkClassMember::TYPE_VOID, 0, 0, HK_OFFSET_OF(MyVertexDataStorage,m_Dec), HK_NULL },
	{ "vertCount", HK_NULL, HK_NULL, hkClassMember::TYPE_INT32, hkClassMember::TYPE_VOID, 0, 0, HK_OFFSET_OF(MyVertexDataStorage,m_vertCount), HK_NULL },
	{ "buffer", &MySimpleBufferClass, HK_NULL, hkClassMember::TYPE_SIMPLEARRAY, hkClassMember::TYPE_STRUCT, 0, 0, HK_OFFSET_OF(MyVertexDataStorage,m_buffer), HK_NULL },
	{ "decl", &MyVertElementClass, HK_NULL, hkClassMember::TYPE_SIMPLEARRAY, hkClassMember::TYPE_STRUCT, 0, 0, HK_OFFSET_OF(MyVertexDataStorage,m_decl), HK_NULL }
};
extern const hkClass MyVertexDataStorageClass;
const hkClass MyVertexDataStorageClass(
	"MyVertexDataStorage",
	HK_NULL, // parent
	sizeof(MyVertexDataStorage),
	HK_NULL,
	0, // interfaces
	HK_NULL,
	0, // enums
	reinterpret_cast<const hkClassMember*>(MyVertexDataStorageClass_Members),
	HK_COUNT_OF(MyVertexDataStorageClass_Members),
	HK_NULL, // defaults
	HK_NULL, // attributes
	0, // flags
	0 // version
	);
HK_REFLECTION_DEFINE_SIMPLE(MyVertexDataStorage);

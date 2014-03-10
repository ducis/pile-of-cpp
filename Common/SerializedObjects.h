#include <windows.h>
#include <D3D9.h>
//#include <d3d9types.h>
class MyVertElement{
public:
	HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR(HK_MEMORY_CLASS_SCENE_DATA,MyVertElement);
	HK_DECLARE_REFLECTION(); 
public:
	WORD    m_Stream;     // Stream index
    WORD        m_Offset;     // Offset in the stream in bytes
    BYTE        m_Type;       // Data type
    BYTE    m_Method;     // Processing method
    BYTE    m_Usage;      // Semantics
    BYTE    m_UsageIndex; // Semantic index
};
/*class MySimpleBuffer{
public:
	HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR(HK_MEMORY_CLASS_SCENE_DATA,MySimpleBuffer);
	HK_DECLARE_REFLECTION(); 
public:
	BYTE *m_data;
	long m_numData;
};*/
class MyVertexDataStorage{
public:	
	HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR(HK_MEMORY_CLASS_SCENE_DATA,MyVertexDataStorage);
	HK_DECLARE_REFLECTION(); 
public:
	hkArray<LPDIRECT3DVERTEXBUFFER9> m_Vbs;
	hkArray<LPDIRECT3DINDEXBUFFER9> m_Ibs;
	LPDIRECT3DVERTEXDECLARATION9 m_Dec;
	long m_vertCount;
	class hkArray<hkArray<BYTE>*> m_buffers;
	hkArray<MyVertElement> *m_decl;
};

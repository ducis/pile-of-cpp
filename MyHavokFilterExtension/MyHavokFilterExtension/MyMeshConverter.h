#ifndef HAVOK_FILTER_MY_MESH_CONVERTER_H
#define HAVOK_FILTER_MY_MESH_CONVERTER_H
#include <d3d9types.h>
class MyMeshConverter : public hctFilterInterface
{
	public:

		HK_DECLARE_CLASS_ALLOCATOR(HK_MEMORY_CLASS_EXPORT);

		// Constructor - takes a pointer to the filter manager that created the filter
		MyMeshConverter(const hctFilterManagerInterface* owner);

		// Destructor
		/*virtual*/ ~MyMeshConverter();

		// The main method in any filter, it does the job
		/*virtual*/ void process( class hkRootLevelContainer& data );

};

// The filter descriptor is used by the filter manager to query information about the filter (name, category, etc)
// as well as to create individual instances.
class MyMeshConverterDescription : public hctFilterDescriptor
{
	public:
		//MyMeshConverterDescription(long t){}
		/*virtual*/ unsigned int getID() const { return 345724561; }
		/*virtual*/ FilterCategory getCategory() const { return HK_CATEGORY_USER; }
		/*virtual*/ FilterBehaviour getFilterBehaviour() const { return HK_DATA_MUTATES_INPLACE; }
		/*virtual*/ const char* getShortName() const { return "My Mesh Converter"; }
		/*virtual*/ const char* getLongName() const { return "Converts meshes"; }
		/*virtual*/ unsigned int getFilterVersion() const { return HCT_FILTER_VERSION(1,0,0); }
		/*virtual*/ hctFilterInterface* createFilter(const class hctFilterManagerInterface* owner) const { return new MyMeshConverter(owner); }

		/*virtual*/ HavokComponentMask getRequiredHavokComponents () const { return HK_COMPONENTS_COMPLETE; }
};

extern MyMeshConverterDescription g_MyMeshConverterDesc;

#endif // HAVOK_FILTER_CONVERT_TO_PHANTOM_ACTION_H

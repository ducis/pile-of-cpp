
class MyUVInverter : public hctFilterInterface
{
	public:

		HK_DECLARE_CLASS_ALLOCATOR(HK_MEMORY_CLASS_EXPORT);

		// Constructor - takes a pointer to the filter manager that created the filter
		MyUVInverter(const hctFilterManagerInterface* owner);

		// Destructor
		/*virtual*/ ~MyUVInverter();

		// The main method in any filter, it does the job
		/*virtual*/ void process( class hkRootLevelContainer& data );

};

class MyUVInverterDescription : public hctFilterDescriptor
{
	public:
		//MyMeshConverterDescription(long t){}
		/*virtual*/ unsigned int getID() const { return 4362543; }
		/*virtual*/ FilterCategory getCategory() const { return HK_CATEGORY_USER; }
		/*virtual*/ FilterBehaviour getFilterBehaviour() const { return HK_DATA_MUTATES_INPLACE; }
		/*virtual*/ const char* getShortName() const { return "My UV Inverter"; }
		/*virtual*/ const char* getLongName() const { return "Inverts v"; }
		/*virtual*/ unsigned int getFilterVersion() const { return HCT_FILTER_VERSION(1,0,0); }
		/*virtual*/ hctFilterInterface* createFilter(const class hctFilterManagerInterface* owner) const { return new MyUVInverter(owner); }

		/*virtual*/ HavokComponentMask getRequiredHavokComponents () const { return HK_COMPONENTS_COMPLETE; }
};

extern MyUVInverterDescription g_MyUVInverterDesc;

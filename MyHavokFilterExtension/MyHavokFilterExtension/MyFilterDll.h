#ifndef HAVOK_FILTER_MY_FILTER_DLL_H
#define HAVOK_FILTER_MY_FILTER_DLL_H
#include <ContentTools/Common/Filters/Common/Filter/hctFilterDll.h>
#include <ContentTools/Common/SceneExport/Utils/hctSceneExportUtils.h>
class MyFilterDll : public hctFilterDll
{
	public:

		MyFilterDll (HMODULE dllModule);

		// Must return how many types of filters are implemented in this DLL
		/*virtual*/ int getNumberOfFilters() const;

		// For each filter implemented in this DLL, we must return a filter descriptor
		/*virtual*/ hctFilterDescriptor* getFilterDescriptor (int index) const;

		/*virtual*/ void initDll ( const hkMemoryInitUtil::SyncInfo& baseSystemInfo, hkError* errorStream );

};
__declspec( dllexport ) hctFilterDll* getFilterDll (HMODULE dllModule);
#endif // HAVOK_FILTER_TUTORIAL_DLL_H
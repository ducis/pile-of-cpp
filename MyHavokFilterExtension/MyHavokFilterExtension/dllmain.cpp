// MyHavokFilterExtension.cpp : 定义 DLL 应用程序的导出函数。
//

#include "stdafx.h"


#include <Common/Base/keycode.cxx>
HINSTANCE hInstance;
static BOOL CommonControlsInitialized = FALSE;

BOOL WINAPI DllMain(HINSTANCE hinstDLL, ULONG fdwReason, LPVOID lpvReserved)
{
	switch ( fdwReason )
	{
		case DLL_PROCESS_ATTACH:
		{
			hInstance = hinstDLL;				// Hang on to this DLL's instance handle.

			if (!CommonControlsInitialized) 
			{

				CommonControlsInitialized = TRUE;
				InitCommonControls();			// Initialize Win32 controls
			}
			break;
		}

		case DLL_PROCESS_DETACH:
		{
			if (CommonControlsInitialized)
			{
				CommonControlsInitialized = FALSE;
			}
		}

		case DLL_THREAD_ATTACH:
		case DLL_THREAD_DETACH:
			break;
	}
	return TRUE;
}


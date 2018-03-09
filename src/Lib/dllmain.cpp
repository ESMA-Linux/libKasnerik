#include <windows.h>
#include "MinHook.h"

typedef FARPROC(WINAPI *GETPROCADDRESS)(HMODULE hModule, LPCSTR lpProcName);
GETPROCADDRESS pGetProcAddress = NULL;

FARPROC WINAPI myGetProcAddress(HMODULE hModule, LPCSTR lpProcName)
{
	if (HIWORD(lpProcName))
	{
		if(
			(strcmp(lpProcName, "wine_server_call")      == 0) ||
			(strcmp(lpProcName, "wine_get_version")      == 0) ||
			(strcmp(lpProcName, "wine_get_build_id")     == 0) ||
			(strcmp(lpProcName, "wine_get_host_version") == 0)  )
		{
			return NULL;
		}
	}

	return pGetProcAddress(hModule, lpProcName);
}

BOOL WINAPI DllMain(HINSTANCE hDLL, DWORD reason, LPVOID reserved)
{
	switch (reason)
	{
		case DLL_PROCESS_ATTACH:
		{
			if (MH_Initialize() != MH_OK)
			{
				return FALSE;
			}

			if (MH_CreateHook(&GetProcAddress, &myGetProcAddress, reinterpret_cast<LPVOID*>(&pGetProcAddress)) != MH_OK)
			{
				return FALSE;
			}

			if (MH_EnableHook(&GetProcAddress) != MH_OK)
			{
				return FALSE;
			}

			break;
		}
		case DLL_PROCESS_DETACH:
		{
			if (MH_DisableHook(&GetProcAddress) != MH_OK)
			{
				return FALSE;
			}

			if (MH_Uninitialize() != MH_OK)
			{
				return FALSE;
			}
			break;
		}
	}
	return TRUE;
}

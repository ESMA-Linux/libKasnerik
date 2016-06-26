#include <windows.h>

#define DLL_NAME "libKasnerik.dll"
#define BUFSIZE 1024

bool FindExeFile(wchar_t* filename)
{
	WIN32_FIND_DATA ffd;
	TCHAR currentDir[BUFSIZE] = { 0 };
	
	HANDLE hFind;
	DWORD dwRet;

	dwRet = GetCurrentDirectoryW(BUFSIZE, currentDir);
	if (dwRet == 0 || dwRet > BUFSIZE)
	{
		return false;
	}
	wcscat_s(currentDir, L"\\*");

	hFind = FindFirstFileW(currentDir, &ffd);
	if (INVALID_HANDLE_VALUE == hFind)
	{
		return false;
	}

	do {
		if (!(ffd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) && 
			(wcscmp(ffd.cFileName, L"OSMView.exe")==0 || wcscmp(ffd.cFileName, L"CityInfo.exe")==0))
		{
				wcscpy(filename, ffd.cFileName);
				return true;
		}
	} while (FindNextFileW(hFind, &ffd) != 0);

	FindClose(hFind);
	return false;
}

bool InjectLibrary(DWORD ProcessID, char* dllName)
{
	HANDLE Proc;
	LPVOID RemoteString, LoadLibAddy;

	if (!ProcessID)
	{
		return false;
	}

	Proc = OpenProcess(PROCESS_CREATE_THREAD | PROCESS_QUERY_INFORMATION | PROCESS_VM_OPERATION | PROCESS_VM_WRITE | PROCESS_VM_READ, FALSE, ProcessID);
	if (!Proc)
	{
		return false;
	}

	LoadLibAddy = (LPVOID)GetProcAddress(GetModuleHandleW(L"kernel32.dll"), "LoadLibraryA");
	RemoteString = (LPVOID)VirtualAllocEx(Proc, NULL, strlen(DLL_NAME), MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);
	WriteProcessMemory(Proc, (LPVOID)RemoteString, dllName, strlen(dllName), NULL);
	CreateRemoteThread(Proc, NULL, NULL, (LPTHREAD_START_ROUTINE)LoadLibAddy, (LPVOID)RemoteString, NULL, NULL);

	CloseHandle(Proc);
	return true;
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,LPSTR lpCmdLine, int nCmdShow)
{
	wchar_t* exeFileName = new wchar_t[BUFSIZE];
	if (!FindExeFile(exeFileName))
	{
		return 1;
	}

	STARTUPINFO			si = { 0 };
	si.cb = sizeof(STARTUPINFO);
	PROCESS_INFORMATION	pi = { 0 };

	if (CreateProcess(exeFileName, exeFileName, NULL, NULL, FALSE, CREATE_SUSPENDED, NULL, NULL, &si, &pi))
	{
		if (ResumeThread(pi.hThread) != (DWORD)-1)
		{
			InjectLibrary(pi.dwProcessId, DLL_NAME);
		}
	}

	if (pi.hThread)
	{
		CloseHandle(pi.hThread);
	}

	if (pi.hProcess)
	{
		CloseHandle(pi.hProcess);
	}

	delete[] exeFileName;

	return 0;
}

#include "Includes.hpp"

void mainThread(LPVOID lpParam) {
	Utils::DeleteClientDirectory("CatClient"); /* Delete the Previous Directory Containing the Log. */
	Utils::CreateClientDirectory("CatClient"); /* Create a New Directory where the log can be stored at. */
	Utils::DebugLogOutput("Initializing Hooks...");
	if (Hooks::InitHooks())
		Utils::DebugLogOutput("Successfully Initialized Hooks.");
	else
		Utils::DebugLogOutput("Failed to Initialize Hooks.");

	while (!GetAsyncKeyState(VK_END))
		Sleep(1);

	if (Hooks::UnInitHooks())
		Utils::DebugLogOutput("Successfully Uninitialized Hooks.");
	else
		Utils::DebugLogOutput("Failed to Uninitialize Hooks.");

	Utils::DebugLogOutput("Uninjected.");
	FreeLibraryAndExitThread(static_cast<HMODULE>(lpParam), EXIT_SUCCESS);
}

BOOL WINAPI DllMain(HMODULE hModule, DWORD dwCallReason, LPVOID lpReserved) {
	if (dwCallReason == DLL_PROCESS_ATTACH) {
		HANDLE hThread = CreateThread(nullptr, NULL, reinterpret_cast<LPTHREAD_START_ROUTINE>(mainThread), hModule, NULL, nullptr);
		if (hThread)
			CloseHandle(hThread);
	}

	return TRUE;
}


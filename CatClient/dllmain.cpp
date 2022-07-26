#include "Includes.hpp"

void mainThread(LPVOID lpParam) {
	Utils::DeleteClientDirectory("CatClient"); /* Delete the Previous Directory Containing the Log. */
	Utils::CreateClientDirectory("CatClient"); /* Create a New Directory where the log can be stored at. */

	/* SwapChain */
	Utils::DebugLogOutput("Initializing SwapChain...");
	if (Utils::InitSwapChain())
		Utils::DebugLogOutput("Successfully Initialized SwapChain.\n");
	else
		Utils::DebugLogOutput("Failed to Initialize SwapChain.\n");

	/* Hooks */
	Utils::DebugLogOutput("Initializing Hooks...");
	if (Hooks::InitHooks())
		Utils::DebugLogOutput("Successfully Initialized Hooks.\n");
	else
		Utils::DebugLogOutput("Failed to Initialize Hooks.\n");

	while (!GetAsyncKeyState(VK_END)) /* Doesn't work since the Game is Sandboxed */
		Sleep(1);

	/* ImGui */
	ImGui_ImplDX11_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();

	/* Hooks */
	if (Hooks::UnInitHooks())
		Utils::DebugLogOutput("Successfully Uninitialized Hooks.\n");
	else
		Utils::DebugLogOutput("Failed to Uninitialize Hooks.\n");

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


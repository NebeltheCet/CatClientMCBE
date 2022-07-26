#pragma once

namespace Hooks {
	inline void HookFunction(std::string hookName, LPVOID pTarget, LPVOID pDetour, LPVOID* ppOriginal) {
		if (MH_CreateHook(pTarget, pDetour, ppOriginal) != MH_STATUS::MH_OK) {
			Utils::DebugLogOutput("Failed to Hook " + hookName + ".");
			return;
		}

		Utils::DebugLogOutput("Successfully Hooked " + hookName + ".");
		return;
	}
	namespace Originals {
		typedef void(__thiscall* CInstance)(uintptr_t*, void*);
		inline CInstance oHkClientInstance;

		typedef HRESULT(__fastcall* D3D11PresentHook) (IDXGISwapChain* pSwapChain, UINT syncInterval, UINT uFLags);
		inline D3D11PresentHook oHkPresent = nullptr;

		typedef HRESULT(__stdcall* D3D11ResizeHook) (IDXGISwapChain* pSwapChain, UINT uBufCount, UINT uWidth, UINT uHeight, DXGI_FORMAT newFormat, UINT uFLags);
		inline D3D11ResizeHook oHkResize = nullptr;
	}

	namespace Functions {
		inline static bool HasInitialized = false;
		inline HRESULT __fastcall hkPresent(IDXGISwapChain* pSChain, UINT syncInterval, UINT uFlags) {
			HWND hwnd = (HWND)FindWindowA(0, "Minecraft");
			if (!HasInitialized || !pSwapChain || !pDevice) {
				pSwapChain = pSChain;
				if (SUCCEEDED(pSwapChain->GetDevice(__uuidof(ID3D11Device), (void**)&pDevice))) {
					pDevice->GetImmediateContext(&pContext);
					ID3D11Texture2D* pBackBuffer;
					pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackBuffer);
					pDevice->CreateRenderTargetView(pBackBuffer, NULL, &renderTargetView);
					pBackBuffer->Release();
					ImGui::CreateContext();
					ImGuiIO& io = ImGui::GetIO();
					io.ConfigFlags = ImGuiConfigFlags_NoMouseCursorChange;
					ImGui_ImplWin32_Init(hwnd);
					ImGui_ImplDX11_Init(pDevice, pContext);
					//ImFontConfig cfg; /* The FreeType ImGui Define is needed but that breaks the dll making it not injectable */
					//cfg.FontBuilderFlags |= ImGuiFreeTypeBuilderFlags::ImGuiFreeTypeBuilderFlags_ForceAutoHint;
					io.Fonts->AddFontFromMemoryCompressedTTF(smolPixel, smolPixelSize, 12.0f, NULL/*&cfg*/);
					Utils::DebugLogOutput("\nInitialized In Present\n");
					HasInitialized = true;
				}
				else
					return Originals::oHkPresent(pSChain, syncInterval, uFlags);
			}

			if (GetAsyncKeyState(VK_INSERT) & 1) /* Wont work since MCBE is Sandboxed */
				Menu::menuOpened = !Menu::menuOpened;

			ImGui::GetStyle().AntiAliasedFill = true;
			ImGui::GetStyle().AntiAliasedLines = true;

			ImGui_ImplDX11_NewFrame();
			ImGui_ImplWin32_NewFrame();
			ImGui::NewFrame();

			ImDrawList* pDraw = ImGui::GetForegroundDrawList();
			Render::doRenderer(pDraw);
			Menu::doMenu(pDraw);

			ImGui::Render();

			pContext->OMSetRenderTargets(1, &renderTargetView, NULL);
			ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

			return Originals::oHkPresent(pSChain, syncInterval, uFlags);
		};

		inline HRESULT __stdcall hkResize(IDXGISwapChain* pSwapChain, UINT uBufCount, UINT uWidth, UINT uHeight, DXGI_FORMAT newFormat, UINT uFLags) {
			if (renderTargetView)
				renderTargetView->Release();

			if (pContext)
				pContext->Release();

			if (pDevice)
				pDevice->Release();

			pDevice = nullptr;
			return Originals::oHkResize(pSwapChain, uBufCount, uWidth, uHeight, newFormat, uFLags);
		}

		inline void hkClientInstance(uintptr_t* cInstance, void* a2) {
			Originals::oHkClientInstance(cInstance, a2);
		}
	}
	inline bool InitHooks() {
		/* Method Pointers */
		uintptr_t AddrCInstance = Utils::FindSig("48 89 5C 24 ?? 48 89 74 24 ?? 57 48 83 EC ?? 48 8B F9 48 8B 01");
		uintptr_t** pSwapChainVTable = *reinterpret_cast<uintptr_t***>(pSwapChain);

		if (MH_Initialize() != MH_STATUS::MH_OK)
			return false;

		HookFunction("hkClientInstance", (void*)AddrCInstance, &Functions::hkClientInstance, reinterpret_cast<LPVOID*>(&Originals::oHkClientInstance));
		if (pSwapChain) {
			HookFunction("hkPresent", (void*)pSwapChainVTable[8], &Functions::hkPresent, reinterpret_cast<LPVOID*>(&Originals::oHkPresent));
			HookFunction("hkResize", (void*)pSwapChainVTable[13], &Functions::hkResize, reinterpret_cast<LPVOID*>(&Originals::oHkResize));
		}
		if (MH_EnableHook(MH_ALL_HOOKS) != MH_STATUS::MH_OK)
			return false;

		return true;
	}

	inline bool UnInitHooks() { /* Made this a bool just in case */
		if (MH_DisableHook(MH_ALL_HOOKS) != MH_STATUS::MH_OK)
			return false;

		if (MH_RemoveHook(MH_ALL_HOOKS) != MH_STATUS::MH_OK)
			return false;

		if (MH_Uninitialize() != MH_STATUS::MH_OK)
			return false;

		return true;
	}
}
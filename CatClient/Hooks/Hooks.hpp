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
		/* Game */
		typedef void(__thiscall* AVKeyItem)(uint64_t, bool);
		inline AVKeyItem oHkKeyItem;

		//typedef void(__thiscall* GameModeTick)(uint64_t*); 
		//inline GameModeTick oHkGameModeTick;

		//typedef void(__thiscall* ActorTick)(uint64_t&);
		//inline ActorTick oHkActorTick;

		/* Drawing */

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
					ImGui_ImplWin32_Init(hwnd);
					ImGui_ImplDX11_Init(pDevice, pContext);
					ImGui::GetIO().Fonts->AddFontFromMemoryCompressedTTF(DroidSansData, DroidSansSize, 16.f, NULL); /* https://cdn.discordapp.com/attachments/959288211581042738/1001671139220787200/unknown.png */
					//ImGui::GetIO().Fonts->AddFontFromMemoryCompressedTTF(smolPixel, smolPixelSize, 16.f, NULL); /* https://cdn.discordapp.com/attachments/959288211581042738/1001671651848626206/unknown.png */
					if (!HasInitialized) {
						Utils::DebugLogOutput("\nInitialized In Present\n");
						HasInitialized = true;
					}
				}
				else
					return Originals::oHkPresent(pSChain, syncInterval, uFlags);
			}

			//if (Input::IsKeyReleased(VK_INSERT))
				//Menu::menuOpened = !Menu::menuOpened;

			ImGui::GetStyle().AntiAliasedFill = true;
			ImGui::GetStyle().AntiAliasedLines = true;

			ImGui_ImplDX11_NewFrame();
			ImGui_ImplWin32_NewFrame();
			ImGui::NewFrame();

			Indicator::Render();
			Menu::doMenu();

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

		inline void hkKeyItem(uint64_t PressedKey, bool IsDown) {
			Input::KeyMap[PressedKey] = IsDown;

			Originals::oHkKeyItem(PressedKey, IsDown);
		}

		//inline void hkGameModeTick(uint64_t* gm) {
		//	Originals::oHkGameModeTick(gm);
		//}

		//inline void hkActorTick(uint64_t blockSource) {
		//	Originals::oHkActorTick(blockSource);
		//}
	}
	inline bool InitHooks() {
		/* Method Pointers */
		uint64_t AddrKeyItem = Utils::FindSig("48 ? ? 48 ? ? ? 4C 8D 05 ? ? ? ? 89");
		//uint64_t AddrGameMode = Utils::FindSig("E8 ? ? ? ? 80 BB ? ? ? ? ? 0F 84 ? ? ? ? 48 83 BB ? ? ? ? ?"); //GameMode::tick /* Unsure if the sig is right */
		//uint64_t AddrActorTick = Utils::FindSig("E8 ? ? ? ? 49 8B 06 41 8B 5E 08 "); //Actor::tick /* Unsure if the sig is right */
		uint64_t** pSwapChainVTable = *reinterpret_cast<uintptr_t***>(pSwapChain);

		if (MH_Initialize() != MH_STATUS::MH_OK)
			return false;

		HookFunction("hkKeyItem", (void*)AddrKeyItem, &Functions::hkKeyItem, reinterpret_cast<LPVOID*>(&Originals::oHkKeyItem));
		//HookFunction("hkGameModeTick", (void*)AddrGameMode, &Functions::hkGameModeTick, reinterpret_cast<LPVOID*>(&Originals::oHkGameModeTick));
		//HookFunction("hkActorTick", (void*)AddrActorTick, &Functions::hkActorTick, reinterpret_cast<LPVOID*>(&Originals::oHkActorTick));
		if (pSwapChain) {
			HookFunction("hkPresent", (void*)pSwapChainVTable[8], &Functions::hkPresent, reinterpret_cast<LPVOID*>(&Originals::oHkPresent));
			HookFunction("hkResize", (void*)pSwapChainVTable[13], &Functions::hkResize, reinterpret_cast<LPVOID*>(&Originals::oHkResize));
		}

		if (MH_EnableHook(MH_ALL_HOOKS) != MH_STATUS::MH_OK)
			return false;

		return true;
	}

	inline bool UnInitHooks() {
		if (MH_Uninitialize() != MH_STATUS::MH_OK)
			return false;

		if (MH_DisableHook(MH_ALL_HOOKS) != MH_STATUS::MH_OK)
			return false;

		return true;
	}
}
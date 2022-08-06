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

		typedef void(__fastcall* Mouse)(uint64_t, char, bool);
		inline Mouse oHkMouse;

		typedef float(__stdcall* Gamma)(void*);
		inline Gamma oHkGamma;

		typedef float(__stdcall* FOV)(void*, void*, void*);
		inline FOV oHkFOV;

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
			Menu::menuOpened = !Input::IsKeyToggled(VK_INSERT); // menu is on when starting minecraft

			ImGui::GetStyle().AntiAliasedFill = true;
			ImGui::GetStyle().AntiAliasedLines = true;

			ImGui_ImplDX11_NewFrame();
			ImGui_ImplWin32_NewFrame();
			ImGui::NewFrame();

			Indicator::Render();
			if (Menu::menuOpened) { Menu::doMenu(); }

			ImGui::Render();

			pContext->OMSetRenderTargets(1, &renderTargetView, NULL);
			ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

			return Originals::oHkPresent(pSChain, syncInterval, uFlags);
		};

		inline HRESULT __stdcall hkResize(IDXGISwapChain* pSwapChain, UINT uBufCount, UINT uWidth, UINT uHeight, DXGI_FORMAT newFormat, UINT uFlags) {
			if (renderTargetView)
				renderTargetView->Release();

			if (pContext)
				pContext->Release();

			if (pDevice)
				pDevice->Release();

			pDevice = nullptr;
			return Originals::oHkResize(pSwapChain, uBufCount, uWidth, uHeight, newFormat, uFlags);
		}

		inline float hkGamma(void* unk1) { //not implemented
			if (Variables::Fullbright)
				return 45.f; //random value i chose

			return Originals::oHkGamma(unk1);
		}

		inline float hkFOV(void* unk1, void* unk2, void* unk3) { /* I didnt care enough to look for the argument types */
			if (Variables::Zoom && Input::IsKeyDown('V'))
				return 25.f;

			return Originals::oHkFOV(unk1, unk2, unk3);
		}

		inline void hkKeyItem(uint64_t PressedKey, bool IsDown) {
			Input::KeyMap[PressedKey] = IsDown;

			Originals::oHkKeyItem(PressedKey, IsDown);
		}

		inline void hkMouse(uint64_t unk1, char mouseAction, bool IsDown) {
			if (IsDown) {
				switch (mouseAction) {
				case 1:
					Input::cachedClicks[0]++;
					break;
				case 2:
					Input::cachedClicks[1]++;
					break;
				}
			}

			Originals::oHkMouse(unk1, mouseAction, IsDown);
		}
	}
	inline bool InitHooks() {
		/* Method Pointers */
		uint64_t AddrKeyItem = Utils::FindSig("48 83 EC ? 0F B6 C1 4C 8D 05");
		uint64_t AddrMouse = Utils::FindSig("48 8B C4 48 89 58 ? 48 89 68 ? 48 89 70 ? 57 41 54 41 55 41 56 41 57 48 83 EC ? 44 0F B7 BC 24");
		uint64_t AddrFOV = Utils::FindSig("48 89 5C 24 ? 56 48 83 EC ? 0F 29 74 24 ? 44 0F 29 44 24");
		uint64_t** pSwapChainVTable = *reinterpret_cast<uintptr_t***>(pSwapChain);

		if (MH_Initialize() != MH_STATUS::MH_OK)
			return false;

		HookFunction("hkKeyItem", (void*)AddrKeyItem, &Functions::hkKeyItem, reinterpret_cast<LPVOID*>(&Originals::oHkKeyItem));
		HookFunction("hkMouse", (void*)AddrMouse, &Functions::hkMouse, reinterpret_cast<LPVOID*>(&Originals::oHkMouse));
		HookFunction("hkFOV", (void*)AddrFOV, &Functions::hkFOV, reinterpret_cast<LPVOID*>(&Originals::oHkFOV));
		if (pSwapChain) {
			HookFunction("hkPresent", (void*)pSwapChainVTable[8], &Functions::hkPresent, reinterpret_cast<LPVOID*>(&Originals::oHkPresent));
			HookFunction("hkResize", (void*)pSwapChainVTable[13], &Functions::hkResize, reinterpret_cast<LPVOID*>(&Originals::oHkResize));
		}

		if (MH_EnableHook(MH_ALL_HOOKS) != MH_STATUS::MH_OK)
			return false;

		return true;
	}

	inline bool UnInitHooks() {
		if (MH_DisableHook(MH_ALL_HOOKS) != MH_STATUS::MH_OK)
			return false;

		if (MH_Uninitialize() != MH_STATUS::MH_OK)
			return false;

		return true;
	}
}
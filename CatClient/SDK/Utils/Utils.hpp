#pragma once

namespace Utils {
	inline bool InitSwapChain() {
		HWND hwnd = (HWND)FindWindowA(0, "Minecraft");

		D3D_FEATURE_LEVEL featureLevel = D3D_FEATURE_LEVEL_11_0;
		DXGI_SWAP_CHAIN_DESC swapChainDesc;
		memset((&swapChainDesc), 0, (sizeof(swapChainDesc))); /* Set the swapChainDesc value to 0 */
		swapChainDesc.BufferCount = 1;
		swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		swapChainDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
		swapChainDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
		swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
		swapChainDesc.SampleDesc.Count = 1;
		swapChainDesc.SampleDesc.Quality = 0;
		swapChainDesc.OutputWindow = hwnd;
		swapChainDesc.Windowed = TRUE;

		HRESULT hrCreateDeviceAndSwapChain = D3D11CreateDeviceAndSwapChain(nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, 0, &featureLevel, 1, D3D11_SDK_VERSION, &swapChainDesc, &pSwapChain, &pDevice, nullptr, &pContext);
		if (FAILED(hrCreateDeviceAndSwapChain))
			return false;

		return true;
	}

	inline ImU32 RGBCol(ImColor pColor) {
		return ImGui::ColorConvertFloat4ToU32(pColor.Value);
	}

	bool doesPathExist(std::string path);
	void CreateClientDirectory(std::string path);
	void DeleteLogContent(std::string path);
	void DebugLogOutput(std::string log);
	uint64_t FindSig(const char* szSignature);
	uint64_t* findMultiLvlPtr(uint64_t baseAddr, std::vector<unsigned int> offsets);
}
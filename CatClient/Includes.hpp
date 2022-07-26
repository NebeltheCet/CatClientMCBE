#pragma once

/* Defines */
//Used for the Signature Function
#define INRANGE(x,a,b)   (x >= a && x <= b)
#define GET_BITS( x )    (INRANGE((x&(~0x20)),'A','F') ? ((x&(~0x20)) - 'A' + 0xa) : (INRANGE(x,'0','9') ? x - '0' : 0))
#define GET_BYTE( x )    (GET_BITS(x[0]) << 4 | GET_BITS(x[1]))

/* Includes */
#include <Windows.h>
#include <Psapi.h>
#include <string>
#include <filesystem>
#include <fstream>
#include <sstream>

/* DirectX11 Includes */
#include <dxgi.h>
#include <d3d11.h>
#pragma comment(lib, "d3d11.lib")

static IDXGISwapChain* pSwapChain = nullptr;
static ID3D11Device* pDevice = nullptr;
static ID3D11DeviceContext* pContext = nullptr;
static ID3D11RenderTargetView* renderTargetView = nullptr;

/* External Includes */
#include "Features/Drawing/Fonts/SmallestPixelFont.hpp"

#include "SDK/ImGui/imgui.h"
#include "SDK/ImGui/imgui_impl_win32.h"
#include "SDK/ImGui/imgui_impl_dx11.h"
#include "SDK/ImGui/freetype/imgui_freetype.h"

#include "SDK/Minhook/minhook.h"

#include "SDK/Utils/Utils.hpp"

#include "Features/Drawing/Menu.hpp"
#include "Features/Drawing/Renderer.hpp"

#include "Hooks/Hooks.hpp"
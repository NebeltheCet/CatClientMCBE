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
#include <map>

/* DirectX11 Includes */
#include <dxgi.h>
#include <d3d11.h>
#pragma comment(lib, "d3d11.lib")

static IDXGISwapChain* pSwapChain = nullptr;
static ID3D11Device* pDevice = nullptr;
static ID3D11DeviceContext* pContext = nullptr;
static ID3D11RenderTargetView* renderTargetView = nullptr;

/* External Includes */
//Fonts
#include "Features/Drawing/Fonts/SmallestPixelFont.hpp"
#include "Features/Drawing/Fonts/DroidSans.hpp"

//ImGui
#include "SDK/ImGui/imgui.h"
#include "SDK/ImGui/imgui_impl_win32.h"
#include "SDK/ImGui/imgui_impl_dx11.h"
#include "SDK/ImGui/imgui_internal.h"

//minhook
#include "SDK/Minhook/minhook.h"

//Utils
#include "SDK/Utils/InputStuff.hpp"
#include "SDK/Utils/Utils.hpp"
#include "SDK/Utils/Vector.hpp"

//Clases
#include "SDK/Classes/CGuiData.hpp"
#include "SDK/Classes/CActor.hpp"
#include "SDK/Classes/CLevelRender.hpp"
#include "SDK/Classes/CLocalPlayer.hpp"
#include "SDK/Classes/CClientInstance.hpp"

//Drawing related Stuff
#include "Features/Drawing/Renderer/Renderer.hpp"
#include "Features/Drawing/Menu.hpp"
#include "Features/Drawing/Indicator.hpp"

//Hooks
#include "Hooks/Hooks.hpp"
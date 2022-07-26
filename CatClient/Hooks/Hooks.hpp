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
	}

	namespace Functions {
		inline void hkClientInstance(uintptr_t* cInstance, void* a2) {
			Originals::oHkClientInstance(cInstance, a2);
		}
	}
	inline bool InitHooks() {
		/* Method Pointers */
		uintptr_t AddrCInstance = Utils::FindSig("48 89 5C 24 ?? 48 89 74 24 ?? 57 48 83 EC ?? 48 8B F9 48 8B 01");

		if (MH_Initialize() != MH_STATUS::MH_OK)
			return false;

		HookFunction("hkClientInstance", (void*)AddrCInstance, &Functions::hkClientInstance, reinterpret_cast<LPVOID*>(&Originals::oHkClientInstance));
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
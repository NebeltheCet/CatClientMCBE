#pragma once

namespace Input {
	inline std::map<uint64_t, bool> KeyMap;
	//Note dont reUse Input Functions...
	//they're gonna activate every Function with the Same KeyBind but other Input Types.
	//and they're gonna activate the function that is getting reused.
	inline bool IsKeyDown(uint64_t key) {
		return (KeyMap.contains(key) ? KeyMap[key] : false);
	}

	inline bool IsKeyReleased(uint64_t key) {
		if (!KeyMap.contains(key))
			return false;

		bool keyMap = KeyMap[key];
		static bool previousKeyMap = false;
		bool fuckingReturn = false;
		if (previousKeyMap && !keyMap) {
			fuckingReturn = true;
		}

		previousKeyMap = keyMap;
		return fuckingReturn;
	}

	inline bool IsKeyToggled(uint64_t key) {
		if (!KeyMap.contains(key))
			return false;

		bool keyMap = KeyMap[key];
		static bool previousKeyMap = false;
		static bool fuckingReturn = false;
		if (previousKeyMap && !keyMap) {
			fuckingReturn = !fuckingReturn;
		}

		previousKeyMap = keyMap;
		return fuckingReturn;
	}
}
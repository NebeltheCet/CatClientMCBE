#pragma once

namespace Input {
	inline std::map<uint64_t, bool> KeyMap;

	inline bool IsKeyDown(uint64_t key) {
		return KeyMap[key];
	}
}
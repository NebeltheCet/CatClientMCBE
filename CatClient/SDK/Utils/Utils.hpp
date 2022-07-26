#pragma once

namespace Utils {
	bool doesPathExist(std::string path);
	void CreateClientDirectory(std::string path);
	void DeleteClientDirectory(std::string path);
	void DebugLogOutput(std::string log);
	uintptr_t FindSig(const char* szSignature);
}
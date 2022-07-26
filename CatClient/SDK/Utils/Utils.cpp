#include "../../Includes.hpp"

bool Utils::doesPathExist(std::string path) {
	struct stat buffer;
	return (stat(path.c_str(), &buffer) == 0);
}

void Utils::CreateClientDirectory(std::string path) {
	std::string envPath = getenv("APPDATA");
	if (envPath.length() <= 0)
		return;

	std::string roamingDir = envPath + std::string("\\..\\Local\\Packages\\Microsoft.MinecraftUWP_8wekyb3d8bbwe\\RoamingState\\");
	if (!doesPathExist(roamingDir))
		return;

	std::string precisePath = std::string(roamingDir + path);
	std::filesystem::create_directories(precisePath);
}

void Utils::DeleteClientDirectory(std::string path) {
	std::string precisePath = getenv("APPDATA") + std::string("\\..\\Local\\Packages\\Microsoft.MinecraftUWP_8wekyb3d8bbwe\\RoamingState\\" + path);
	if (doesPathExist(precisePath)) {
		if (std::filesystem::is_directory(precisePath)) {
			std::filesystem::remove_all(precisePath);
		}
		else {
			std::filesystem::remove(precisePath);
		}
	}
}

void WritedatShit(std::string path, std::string log) {
	std::string precisePath = getenv("APPDATA") + std::string("\\..\\Local\\Packages\\Microsoft.MinecraftUWP_8wekyb3d8bbwe\\RoamingState\\" + path);
	if (!Utils::doesPathExist(precisePath)) {
		std::filesystem::path p(precisePath);
		std::filesystem::create_directories(p.parent_path().string());
	}

	CloseHandle(CreateFileA(precisePath.c_str(), GENERIC_WRITE | GENERIC_READ, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL));
	if (Utils::doesPathExist(precisePath) && log.length() > 0) {
		std::ofstream fileOutput;
		fileOutput.open(precisePath.c_str(), std::ios_base::app);
		fileOutput << log << std::endl;
		fileOutput.close();
	}
}

void Utils::DebugLogOutput(std::string log) {
	WritedatShit("CatClient/ClientLog.txt", log);
	return;
}

uintptr_t Utils::FindSig(const char* szSignature) {
	const char* pattern = szSignature;
	uintptr_t firstMatch = 0;
	static const uintptr_t rangeStart = (uintptr_t)GetModuleHandleA("Minecraft.Windows.exe");
	static MODULEINFO miModInfo;
	static bool init = false;
	if (!init) {
		init = true;
		GetModuleInformation(GetCurrentProcess(), (HMODULE)rangeStart, &miModInfo, sizeof(MODULEINFO));
	}
	static const uintptr_t rangeEnd = rangeStart + miModInfo.SizeOfImage;

	BYTE patByte = GET_BYTE(pattern);
	const char* oldPat = pattern;

	for (uintptr_t pCur = rangeStart; pCur < rangeEnd; pCur++)
	{
		if (!*pattern)
			return firstMatch;

		while (*(PBYTE)pattern == ' ')
			pattern++;

		if (!*pattern)
			return firstMatch;

		if (oldPat != pattern) {
			oldPat = pattern;
			if (*(PBYTE)pattern != '\?')
				patByte = GET_BYTE(pattern);
		}

		if (*(PBYTE)pattern == '\?' || *(BYTE*)pCur == patByte)
		{
			if (!firstMatch)
				firstMatch = pCur;

			if (!pattern[2])
				return firstMatch;

			pattern += 2;
		}
		else
		{
			pattern = szSignature;
			firstMatch = 0;
		}
	}
}
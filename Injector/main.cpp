#include "Includes.hpp"

DWORD SetDllPermissions(std::string wstrFilePath) { /* From UC(https://www.unknowncheats.me/forum/general-programming-and-reversing/177183-basic-intermediate-techniques-uwp-app-modding.html) */
    PACL pOldDACL = NULL, pNewDACL = NULL;
    PSECURITY_DESCRIPTOR pSD = NULL;
    EXPLICIT_ACCESS eaAccess;
    SECURITY_INFORMATION siInfo = DACL_SECURITY_INFORMATION;
    DWORD dwResult = ERROR_SUCCESS;
    PSID pSID;

    // Get a pointer to the existing DACL
    dwResult = GetNamedSecurityInfoA(wstrFilePath.c_str(), SE_FILE_OBJECT, DACL_SECURITY_INFORMATION, NULL, NULL, &pOldDACL, NULL, &pSD);
    if (dwResult != ERROR_SUCCESS)
        goto Cleanup;

    // Get the SID for ALL APPLICATION PACKAGES using its SID string
    ConvertStringSidToSidA("S-1-15-2-1", &pSID);
    if (pSID == NULL)
        goto Cleanup;

    ZeroMemory(&eaAccess, sizeof(EXPLICIT_ACCESS));
    eaAccess.grfAccessPermissions = GENERIC_READ | GENERIC_EXECUTE;
    eaAccess.grfAccessMode = SET_ACCESS;
    eaAccess.grfInheritance = SUB_CONTAINERS_AND_OBJECTS_INHERIT;
    eaAccess.Trustee.TrusteeForm = TRUSTEE_IS_SID;
    eaAccess.Trustee.TrusteeType = TRUSTEE_IS_WELL_KNOWN_GROUP;
    eaAccess.Trustee.ptstrName = (LPSTR)pSID;

    // Create a new ACL that merges the new ACE into the existing DACL
    dwResult = SetEntriesInAclA(1, &eaAccess, pOldDACL, &pNewDACL);
    if (ERROR_SUCCESS != dwResult)
        goto Cleanup;

    // Attach the new ACL as the object's DACL
    dwResult = SetNamedSecurityInfoA((LPSTR)wstrFilePath.c_str(), SE_FILE_OBJECT, siInfo, NULL, NULL, pNewDACL, NULL);
    if (ERROR_SUCCESS != dwResult)
        goto Cleanup;

Cleanup:
    if (pSD != NULL)
        LocalFree((HLOCAL)pSD);
    if (pNewDACL != NULL)
        LocalFree((HLOCAL)pNewDACL);

    return dwResult;
}

DWORD GetMCBEPID() {
    DWORD procId = 0;
    HANDLE hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if (hSnap != INVALID_HANDLE_VALUE) {
        PROCESSENTRY32 procEntry;
        procEntry.dwSize = sizeof(procEntry);
        if (Process32First(hSnap, &procEntry))  {
            while (Process32Next(hSnap, &procEntry)) {
                if (!_stricmp(procEntry.szExeFile, "Minecraft.Windows.exe")) {
                    procId = procEntry.th32ProcessID;
                    break;
                }
            }
        }
    }

    CloseHandle(hSnap);
    return procId;
}

bool Inject(const char* dllPath) {
    DWORD PID = GetMCBEPID();
    if (!PID)
        return false;

    HANDLE Proc = OpenProcess(PROCESS_ALL_ACCESS, FALSE, PID);
    if (!Proc)
        return false;

    char DllName[MAX_PATH];
    GetFullPathNameA(dllPath, MAX_PATH, DllName, NULL);
    LPVOID LoadLib = (LPVOID)GetProcAddress(GetModuleHandleA("kernel32.dll"), "LoadLibraryA");
    if (!LoadLib)
        return false;

    LPVOID RemoteString = VirtualAllocEx(Proc, NULL, strlen(DllName), MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);
    if (!RemoteString)
        return false;

    WriteProcessMemory(Proc, RemoteString, DllName, strlen(DllName), NULL);
    CreateRemoteThread(Proc, NULL, NULL, (LPTHREAD_START_ROUTINE)LoadLib, (LPVOID)RemoteString, NULL, NULL);

    CloseHandle(Proc);
    return true;
}

int main() {
startOfLoader:
    /* Variables */
    int startChoice = -1;
    std::string dllPath = "";

    std::cout << "[1] CatClient\n[2] Start Injector\n[3] Exit\nChoice => ";
    std::cin >> startChoice;
    system("cls");
    switch (startChoice) {
    case 1:
        dllPath = "CatClient.dll";
        break;
    case 2:
        std::cout << "DllPath => ";
        std::cin >> dllPath;
        system("cls");
        break;
    case 3:
        return EXIT_SUCCESS;
        break;
    default:
        if (startChoice != -1) {
            std::cout << "\nInvalid Choice\n";
            std::cout << "Back to Start...\n";
            Sleep(800);
            system("cls");
            goto startOfLoader;
        }
        break;
    }

    if (startChoice == -1) {
        return EXIT_SUCCESS;
    }

    if (dllPath.find(".dll") == std::string::npos) {
        dllPath += ".dll";
    }

    if (dllPath == "" || dllPath.size() <= 5) {
        std::cout << "\nWrong Path or Invalid Dll File.\n";
        std::cout << "Back to Start...\n";
        Sleep(800);
        system("cls");
        goto startOfLoader;
    }

    /* Change Permissions Here. */
    DWORD dwResult = SetDllPermissions(dllPath);
    if (dwResult != ERROR_SUCCESS) { /* If the Message Repeats Rebuild the Dll */
        std::cout << "Failed to Set Dll Permissions.\n";
        std::cout << "Back to Start...\n";
        Sleep(800);
        system("cls");
        goto startOfLoader;
    }

    /* Inject Here */
    if (Inject(dllPath.c_str()))
        std::cout << "Injected.\n";
    else
        std::cout << "Failed to Inject.\n";

    Sleep(800);
    system("cls");
    std::cout << "Back to Start...\n";
    Sleep(800);
    system("cls");
    goto startOfLoader;
}

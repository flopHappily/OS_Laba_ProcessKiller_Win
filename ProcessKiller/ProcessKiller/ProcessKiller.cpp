#include <windows.h>
#include <tchar.h>
#include <stdio.h>
#include <process.h>
#include <WinBase.h>
#include <TlHelp32.h>
#include <iostream>

#define SIZE 256

using namespace std;

const wchar_t* wchar(const char* c){
    const size_t cSize = strlen(c) + 1;
    wchar_t* wc = new wchar_t[cSize];
    size_t *retval = new size_t(cSize);
    rsize_t dstsz = cSize;
    mbstowcs_s(retval,wc,dstsz,c, cSize);
    return wc;
}

void killProcessByName(const WCHAR* filename)
{
    HANDLE hSnapShot = CreateToolhelp32Snapshot(TH32CS_SNAPALL, NULL);
    PROCESSENTRY32 pEntry;
    pEntry.dwSize = sizeof(pEntry);
    BOOL hRes = Process32First(hSnapShot, &pEntry);
    while (hRes)
    {
        if (wcscmp(pEntry.szExeFile,filename)==0)
        {
            HANDLE hProcess = OpenProcess(PROCESS_TERMINATE, 0,
                (DWORD)pEntry.th32ProcessID);
            if (hProcess != NULL)
            {
                TerminateProcess(hProcess, 9);
                CloseHandle(hProcess);
            }
        }
        hRes = Process32Next(hSnapShot, &pEntry);
    }
    CloseHandle(hSnapShot);
}


int main(int argc, char* argv[])
{
    for (int i = 1; i < argc; i++) {
        cout << argv[i] << endl;
        killProcessByName(wchar(argv[i]));
    }
    LPTSTR lpszVariable;
    TCHAR Var[SIZE];
    LPTCH lpvEnv;

    if (!GetEnvironmentVariable(L"PROC_TO_KILL", Var, SIZE)) {
        std::cout << "Failed to get value of PROC_TO_KILL\n";
        return 1;
    }
    else {
        char proc[SIZE + 1] = { 0 };
        size_t converted = 0;
        wcstombs_s(&converted, proc, SIZE, Var, _TRUNCATE);
        string processes(proc);
        int j(0), i = processes.find("=", 0) + 1;
        while (processes.find(",", i) != string::npos) {
            j = processes.find(",", i);
            strcpy_s(proc, processes.substr(i, j - i).c_str());
            killProcessByName(wchar(proc));
            i = j + 1;
        }
        strcpy_s(proc, processes.substr(i, string::npos).c_str());
        killProcessByName(wchar(proc));
    }
    return 0;
}
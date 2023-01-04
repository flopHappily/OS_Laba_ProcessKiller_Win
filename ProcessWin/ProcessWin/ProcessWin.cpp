#include <windows.h>
#include <stdio.h>
#include <strsafe.h>

#define BUFSIZE 4096

int main(){

    TCHAR chNewEnv[BUFSIZE];
    LPTSTR lpszCurrentVariable;
    DWORD dwFlags = 0;
    STARTUPINFO si;
    PROCESS_INFORMATION piProcessKiller;
    ZeroMemory(&si, sizeof(si));
    si.cb = sizeof(si);
    ZeroMemory(&piProcessKiller, sizeof(piProcessKiller));
    TCHAR szPath[MAX_PATH] = TEXT("ProcessKiller msedge.exe");

    lpszCurrentVariable = (LPTSTR)chNewEnv;
    if (FAILED(StringCchCopy(lpszCurrentVariable, BUFSIZE, TEXT("PROC_TO_KILL=Telegram.exe,EXCEL.EXE\0"))))
    {
        printf("String copy failed\n");
        return FALSE;
    }
    //lpszCurrentVariable += lstrlen(lpszCurrentVariable) + 1;
    //*lpszCurrentVariable = (TCHAR)0; 

    SecureZeroMemory(&si, sizeof(STARTUPINFO));
    si.cb = sizeof(STARTUPINFO);

    SetEnvironmentVariable(L"PROC_TO_KILL", chNewEnv);

    lpszCurrentVariable = (LPTSTR)chNewEnv;
    if (!CreateProcess(NULL, szPath,NULL,NULL,FALSE,CREATE_UNICODE_ENVIRONMENT,NULL, NULL, &si, &piProcessKiller))
    {
        MessageBox(NULL, TEXT("ProcessKiller failed"), TEXT("1:ProcessA"), MB_OK);
        return(0);
    };


    if (WAIT_FAILED == WaitForSingleObject(piProcessKiller.hProcess, INFINITE)){
        MessageBox(NULL, TEXT("WAIT_FAILED for ProcessB"), TEXT("2:ProcessA"), MB_OK);
    };


    CloseHandle(piProcessKiller.hProcess);
    CloseHandle(piProcessKiller.hThread);

    return 0;
}
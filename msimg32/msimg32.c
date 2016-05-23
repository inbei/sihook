#include <windows.h>

static HMODULE m_hModule = NULL;    // ԭʼģ����
//static DWORD m_dwReturn[5] = {0}; // ԭʼ�������ص�ַ

// ����ԭʼģ��
static inline BOOL WINAPI Load()
{
    TCHAR tzPath[MAX_PATH];
    TCHAR tzTemp[MAX_PATH * 2];

    GetSystemDirectory(tzPath, MAX_PATH);
    lstrcat(tzPath, TEXT("\\msimg32"));
    m_hModule = LoadLibrary(tzPath);
    if (m_hModule == NULL)
    {
        wsprintf(tzTemp, TEXT("�޷����� %s�������޷��������С�"), tzPath);
        OutputDebugString(tzTemp);
        ExitProcess(-2);
    }
    else OutputDebugString("����msimg32");
    return (m_hModule != NULL);
}

// �ͷ�ԭʼģ��
static inline VOID WINAPI Free()
{
    if (m_hModule)
    {
        FreeLibrary(m_hModule);
    }
}

// ��ȡԭʼ������ַ
static FARPROC WINAPI GetAddress(PCSTR pszProcName)
{
    FARPROC fpAddress;
    CHAR szProcName[16];
    TCHAR tzTemp[MAX_PATH];

    fpAddress = GetProcAddress(m_hModule, pszProcName);
    if (fpAddress == NULL)
    {
        if (HIWORD(pszProcName) == 0)
        {
            wsprintfA(szProcName, "%d", pszProcName);
            pszProcName = szProcName;
        }

        wsprintf(tzTemp, TEXT("�޷��ҵ����� %s�������޷��������С�"), pszProcName);
        OutputDebugString(tzTemp);
        ExitProcess(-2);
    }

    return fpAddress;
}


BOOL WINAPI DllMain(HMODULE hModule, DWORD dwReason, PVOID pvReserved)
{
    if (dwReason == DLL_PROCESS_ATTACH)
    {
        DisableThreadLibraryCalls(hModule);
        Load();

        HMODULE hMyDLL = LoadLibrary("sihook.dll");
        if (hMyDLL == NULL)
            OutputDebugString("���� sihook.dllʧ��");
        else
            OutputDebugString("���� sihook.dll�ɹ�");

		HMODULE hUtfDLL = LoadLibrary("siutf8.dll");
        if (hUtfDLL == NULL)
            OutputDebugString("���� siutf8.dllʧ��");
        else
            OutputDebugString("���� siutf8.dll�ɹ�");
		
        return TRUE;
    }
    else if (dwReason == DLL_PROCESS_DETACH)
    {
        Free();
    }
    return TRUE;
}

typedef void (__cdecl *RemoteFunc)(void);

void __cdecl vSetDdrawflag(void)
{
    RemoteFunc func = (RemoteFunc)GetAddress("vSetDdrawflag");
    func();
}
void __cdecl AlphaBlend(void)
{
    RemoteFunc func = (RemoteFunc)GetAddress("AlphaBlend");
    func();
}
void __cdecl DllInitialize(void)
{
    RemoteFunc func = (RemoteFunc)GetAddress("DllInitialize");
    func();
}
void __cdecl GradientFill(void)
{
    RemoteFunc func = (RemoteFunc)GetAddress("GradientFill");
    func();
}
void __cdecl TransparentBlt(void)
{
    RemoteFunc func = (RemoteFunc)GetAddress("TransparentBlt");
    func();
}

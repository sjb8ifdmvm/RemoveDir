#include <Windows.h>
#include <tchar.h>

void WINAPI DeleteDir(const PCWCHAR dir);

int _tmain(int argc, PCWCHAR argv[])
{
    int iArgCount = 0;
    PWCHAR* t_CommandLinePtrPtr = nullptr;
    __try
    {
        __try
        {
            t_CommandLinePtrPtr = ::CommandLineToArgvW(::GetCommandLineW(), &iArgCount);
            if (t_CommandLinePtrPtr == NULL)
                __leave;
            if (iArgCount > 2)
                __leave;
            ::DeleteDir(t_CommandLinePtrPtr[1]);
        }
        __finally
        {
            if (t_CommandLinePtrPtr != NULL)
                ::LocalFree(t_CommandLinePtrPtr);
            t_CommandLinePtrPtr;
        }
    }
    __except (1)
    {

    }
    return 0;
}

void WINAPI DeleteDir(const PCWCHAR p_PathPtr)
{
    size_t t_szAllocLen = 0;
    PWCHAR t_FileNamePtr = NULL;
    PWCHAR t_PosPtr = NULL;
    ::WIN32_FIND_DATAW t_FindFileData;
    HANDLE t_hFindHandle = INVALID_HANDLE_VALUE;

    __try
    {
        if ((p_PathPtr == NULL) || (p_PathPtr[0] == NULL))
            __leave;
        ::ZeroMemory(&t_FindFileData, sizeof(::WIN32_FIND_DATAW));

        __try
        {
            t_szAllocLen = wcslen(p_PathPtr) + MAX_PATH + 1;//EX: size = 263 (題目(2) + 260 + 1)
            t_FileNamePtr = new WCHAR[t_szAllocLen];
            wcscpy_s(t_FileNamePtr, t_szAllocLen, p_PathPtr);

            t_PosPtr = wcsrchr(t_FileNamePtr, L'\\');//字串中尋找 '\'字元所在位置
            if (t_PosPtr != NULL && t_PosPtr[1] == NULL)
                t_PosPtr[0] = NULL;

            wcscat_s(t_FileNamePtr, t_szAllocLen, L"\\*.*");//EX:題目\*.* 
            t_hFindHandle = ::FindFirstFileW(t_FileNamePtr, &t_FindFileData);

            if (t_hFindHandle == INVALID_HANDLE_VALUE)//文件查找失敗
                __leave;

            wcscpy_s(t_FileNamePtr, t_szAllocLen, p_PathPtr);//EX:題目
            t_PosPtr = wcsrchr(t_FileNamePtr, L'\\');//字串中尋找 '\'字元所在位置
            if (t_PosPtr != NULL && t_PosPtr[1] == NULL)
                t_PosPtr[1] = NULL;

            do
            {
                if (t_FindFileData.cFileName[0] == NULL)
                    break;
                if (t_FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
                {
                    if (!wcscmp(t_FindFileData.cFileName, L"."))
                        continue;
                    if (!wcscmp(t_FindFileData.cFileName, L".."))
                        continue;
                    swprintf_s(t_FileNamePtr, t_szAllocLen, L"%s\\%s", p_PathPtr, t_FindFileData.cFileName);
                    ::DeleteDir(t_FileNamePtr);
                }
                else
                {
                    swprintf_s(t_FileNamePtr, t_szAllocLen, L"%s\\%s", p_PathPtr, t_FindFileData.cFileName);
                    ::DeleteFileW(t_FileNamePtr);
                }
            } while (::FindNextFileW(t_hFindHandle, &t_FindFileData));
            ::RemoveDirectoryW(p_PathPtr);
        }
        __finally
        {
            if (t_FileNamePtr != NULL)
                delete[] t_FileNamePtr;
            t_FileNamePtr = NULL;
            if (t_hFindHandle != INVALID_HANDLE_VALUE)
                ::FindClose(t_hFindHandle);
            t_hFindHandle = INVALID_HANDLE_VALUE;
        }
    }
    __except (1)
    {

    }
}

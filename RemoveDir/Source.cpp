#include <Windows.h>
#include <tchar.h> 
#include <strsafe.h>


void DeleteDir(TCHAR* dir);
void DeleteDir(TCHAR* dir, unsigned int deleteParent);

int _tmain(int argc, TCHAR* argv[])
{
    if (argc != 2)
    {
        _tprintf(TEXT("\nUsage: %s <folder>\n"), argv[0]);
        return 1;
    }

    DeleteDir(argv[1]);

    return 0;
}

void DeleteDir(TCHAR* dir)
{
    DeleteDir(dir, 1);
}

void DeleteDir(TCHAR* dir, unsigned int deleteParent)
{
    TCHAR szDir[MAX_PATH]{ 0 };
    StringCchCopy(szDir, MAX_PATH, dir);
    StringCchCat(szDir, MAX_PATH, L"//*.*");

    WIN32_FIND_DATAW wFindFileData;
    HANDLE hFind = FindFirstFileW(szDir, &wFindFileData);

    if (hFind == INVALID_HANDLE_VALUE)
        return;

    do
    {
        if (wFindFileData.cFileName[0] == L'.')//忽略系統目錄
            continue;

        TCHAR szFile[MAX_PATH];
        StringCchCopy(szFile, MAX_PATH, dir);
        StringCchCat(szFile, MAX_PATH, L"//");
        StringCchCat(szFile, MAX_PATH, wFindFileData.cFileName);

        if (wFindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) //如果找到的是目錄，則進入目錄進行遞迴
        {
            DeleteDir(szFile, ++deleteParent);
            RemoveDirectoryW(szFile);
            deleteParent--;
        }
        else //找到的是檔案
        {
            DeleteFileW(szFile);
        }
    } while (FindNextFileW(hFind, &wFindFileData));
    if (deleteParent > 0)
    {
        RemoveDirectoryW(dir);
    }
    FindClose(hFind);
}

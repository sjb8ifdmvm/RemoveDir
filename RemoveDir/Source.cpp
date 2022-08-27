#include <Windows.h>
#include <tchar.h> 
#include <strsafe.h>

#ifdef DEBUG
#include <locale>
#endif // DEBUG

void DeleteDir(TCHAR* dir);
void DeleteDir(TCHAR* dir, unsigned int deleteParent);

int _tmain(int argc, TCHAR* argv[])
{
    if (argc != 2)
    {
        _tprintf(TEXT("\nUsage: %s <folder>\n"), argv[0]);
        return 1;
    }
#ifdef DEBUG
    setlocale(LC_CTYPE, "");
#endif // DEBUG

    DeleteDir(argv[1]);

    return 0;
}

void DeleteDir(TCHAR* dir)
{
    DeleteDir(dir, 1);
}

void DeleteDir(TCHAR* dir, unsigned int DeleteRootPath)
{
    TCHAR szDir[MAX_PATH]{ 0 }; 
    StringCchCopy(szDir, MAX_PATH, dir);
    StringCchCat(szDir, MAX_PATH, L"\\*.*"); //EX: dir\*.*

    WIN32_FIND_DATAW wFindFileData;
    HANDLE hFind = FindFirstFileW(szDir, &wFindFileData);

    if (hFind == INVALID_HANDLE_VALUE)
        return;

    do
    {
        if (wFindFileData.cFileName[0] == L'.')//忽略系統目錄
            continue;

        TCHAR szFile[MAX_PATH];
        StringCchCopy(szFile, MAX_PATH, dir);//EX: dir\*.*
        StringCchCat(szFile, MAX_PATH, L"\\");//EX: dir\*.*\ 
        StringCchCat(szFile, MAX_PATH, wFindFileData.cFileName);//EX: dir\*.*\file

        if (wFindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) //如果找到的是目錄
        {
            DeleteDir(szFile, ++DeleteRootPath); //進入目錄進行遞迴
            RemoveDirectoryW(szFile);//資料夾內檔案清除後移除目錄
            DeleteRootPath--;
        }
        else //找到的是檔案
        {
            DeleteFileW(szFile);//清除
        }
    } while (FindNextFileW(hFind, &wFindFileData));

    //移除目標根目錄
    if (DeleteRootPath > 0)
    {
        RemoveDirectoryW(dir);
    }

    FindClose(hFind);//關閉檔案句柄
}

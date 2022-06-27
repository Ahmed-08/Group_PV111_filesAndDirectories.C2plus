#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <direct.h>
#include <io.h>
#include <Windows.h>

// функция копирует данные из одной папки в другую
void copyDirectory(char* oldPath, char* newPath)
{
    // сохраняем размер пути старой папки
    size_t sizeOldPath = strlen(oldPath);

    // если новой папки не существует
    // то создаем новую папку
    _finddata_t* fileinfo = new _finddata_t;
    long d = _findfirst(newPath, fileinfo);
    if (d == -1)
    {
        _mkdir(newPath);
    }
    // находим первый файл в папке
    // с расширением .txt
    strcat(oldPath, "*.txt");
    long fileInOldPath = _findfirst(oldPath, fileinfo);
    oldPath[sizeOldPath] = '\0';

    // сохраняем длину нового пути
    strcat(newPath, "//");
    size_t sizeNewdPath = strlen(newPath);
    d = fileInOldPath;
    
    // находим файл с максимальным размером
    // и записываем размер в maxSizeFile
    int maxSizeFile{0};
    while (d != -1)
    {
        d = _findnext(fileInOldPath, fileinfo);
        if(fileinfo->size > maxSizeFile)
            maxSizeFile = fileinfo->size;
    }
    oldPath[sizeOldPath] = '\0';
    strcat(oldPath, "*.txt");
    fileInOldPath = _findfirst(oldPath, fileinfo);
    
    // выделяем память на maxSizeFile элементов
    char* buffer = new char[maxSizeFile]{ 0 };
    d = fileInOldPath;
    while (d != -1)
    {
        oldPath[sizeOldPath] = '\0';
        strcat(oldPath, fileinfo->name);
        
        newPath[sizeNewdPath] = '\0';
        strcat(newPath, fileinfo->name);
        if(!fileinfo->size)
        {
            FILE* file = fopen(newPath, "w");
            fclose(file);
            d = _findnext(fileInOldPath, fileinfo);
            continue; 
        }

        FILE* file = fopen(oldPath, "r");
        if (!file)
        {
            std::cout << "ERROR!";
            return;
        }
        else
        {
            fread(buffer, sizeof(char), fileinfo->size, file);
            fclose(file);
        }

        file = fopen(newPath, "w");
        if (!file)
        {
            std::cout << "ERROR!";
            return;
        }
        else
        {
            fwrite(buffer, sizeof(char), strlen(buffer), file);
            fclose(file);
        }
        
        d = _findnext(fileInOldPath, fileinfo);
    }

    _findclose(fileInOldPath);
    delete[] buffer;
    delete fileinfo;

}

void moveFile(char* oldPath, char* newPath)
{
    size_t sizeOldPath = strlen(oldPath);
    size_t sizeNewPath = strlen(newPath);
    strcat(oldPath, "*.txt");
    _finddata_t* fileinfo = new _finddata_t;
    long fileInOldPath = _findfirst(oldPath, fileinfo);
    long d = fileInOldPath;
    while (d != -1)
    {
        oldPath[sizeOldPath] = '\0';
        newPath[sizeNewPath] = '\0';
        strcat(oldPath, fileinfo->name);
        strcat(newPath, fileinfo->name);
        rename(oldPath, newPath);

        d = _findnext(fileInOldPath, fileinfo);
    }
    _findclose(fileInOldPath);
    delete fileinfo;
}

void deleteDirectory(char* path)
{
    size_t sizePath = strlen(path);
    strcat(path, "*.txt");
    _finddata_t* fileinfo = new _finddata_t;
    long fileInOldPath = _findfirst(path, fileinfo);
    long d = fileInOldPath;
    
    while (d != -1)
    {
        path[sizePath] = '\0';
        strcat(path, fileinfo->name);
        remove(path);
        d = _findnext(fileInOldPath, fileinfo);
    }
    path[sizePath] = '\0';
    _rmdir(path);
    delete fileinfo;
}

int main()
{
    char* s1 = new char[100]{ 0 };
    char* s2 = new char[100]{ 0 };
    strcat(s1, "G://C++//projects//VBD11//workWithFiles//");
    strcat(s2, "G://C++//projects//VBD11//workWithFiles//newDirectory");
    
    copyDirectory(s1, s2);
    strcpy(s2, "G://C++//projects//VBD11//workWithFiles//newDirectory//");

    deleteDirectory(s2);
    _mkdir(s2);
    
    strcpy(s1, "G://C++//projects//VBD11//workWithFiles//");
    strcpy(s2, "G://C++//projects//VBD11//workWithFiles//newDirectory//");
    moveFile(s1, s2);
    strcpy(s2, "G://C++//projects//VBD11//workWithFiles//newDirectory//");
    deleteDirectory(s2);
    
    delete[] s1;
    delete[] s2;
    return 0;
}
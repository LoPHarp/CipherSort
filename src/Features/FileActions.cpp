#include "FileActions.h"

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <windows.h>
#include <filesystem>

using namespace std;
namespace fs = std::filesystem;

/*
string ResolvePath(const string& originalPath)
{
    fs::path p(originalPath);
    string dir = p.parent_path().string();
    string filename = p.filename().string();

    if (filename.find("Sorted_") == 0) return originalPath;

    string sortedName = "Sorted_" + filename;
    fs::path sortedPath = p.parent_path() / sortedName;

    if (fs::exists(sortedPath))
    {
        ConsoleClear();
        cout << "Sorted version found!" << endl;
        cout << "1. Use Original: " << filename << endl;
        cout << "2. Use Sorted:   " << sortedName << endl;

        int choice = InputDigitalValue();
        if (choice == 2) return sortedPath.string();
    }

    return originalPath;
}

void ActionViewContent(const string& path)
{
    string targetPath = ResolvePath(path);

    fs::path p(path);
    string filename = p.filename().string();

    if (filename.find("Crypted_") == 0)
    {
        printError("File is encrypted! Decrypt it first.");
        return;
    }

    ifstream in(targetPath);
    if (!in.is_open())
    {
        printError("Cannot open file for reading!");
        return;
    }

    vector<string> content;
    string line;
    while (getline(in, line))
    {
        content.push_back(line);
        }
    in.close();

    if (content.empty())
    {
        printError("File is empty.");
        return;
    }

    ConsoleClear();
    cout << "Viewing: " << filename << endl;
    PrintData(content);
}
*/
void ActionOpenFolder(const string& path)
{
    string params = "/select,\"" + path + "\"";
    ShellExecuteA(NULL, "open", "explorer.exe", params.c_str(), NULL, SW_SHOW);
}

void ActionOpenFile(const string& path)
{
    ShellExecuteA(NULL, "open", path.c_str(), NULL, NULL, SW_SHOW);
}

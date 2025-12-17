#include "FileActions.h"
#include "View.h" 
#include "UserInput.h" 

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <windows.h>
#include <filesystem>

using namespace std;
namespace fs = std::filesystem;

void ActionViewContent(const string& path)
{
    fs::path p(path);
    string filename = p.filename().string();

    if (filename.find("Crypted_") == 0)
    {
        printError("File is encrypted! Decrypt it first.");
        return;
    }

    ifstream in(path);
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

    PrintData(content);
}

void ActionOpenFolder(const string& path)
{
    string params = "/select,\"" + path + "\"";
    ShellExecuteA(NULL, "open", "explorer.exe", params.c_str(), NULL, SW_SHOW);
}

void ActionOpenFile(const string& path)
{
    ShellExecuteA(NULL, "open", path.c_str(), NULL, NULL, SW_SHOW);
}
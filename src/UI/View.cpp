#include "View.h"
#include "Config.h"

#include <iostream>
#include <fstream>
#include <string>
#include <windows.h>
#include <vector>
#include <filesystem>

using namespace std;

void ConsoleClear()
{
	cout << "\x1B[2J\x1B[H";
}

void Countdown(int seconds)
{
	for (int i = seconds; i > 0; i--)
	{
		cout << i << " " << flush;
		Sleep(1000);
	}
}

void ViewMainMenu()
{
	ConsoleClear();
	cout << "1 Set File" << endl;
	cout << "2 Sorte File" << endl;
	cout << "3 Cipher File" << endl;
	cout << "--- File Actions ---" << endl;
	cout << "4 View Content (Console)" << endl;
	cout << "5 Open File Folder" << endl;
	cout << "6 Open File" << endl;
}

void ViewsetFolderMenu()
{
	ConsoleClear();
	cout << "1 Enter the full path manually" << endl;
	cout << "2 Navigate step by step by selecting folders/files" << endl;
	cout << "3 Select using Windows Dialog (Standard)" << endl;
	cout << "4 Return(Exit)" << endl;
}

void ViewSortedMenu()
{
	ConsoleClear();
	cout << "1 Sort by the first 5 characters" << endl;
	cout << "2 Sort by first N chars" << endl;
	cout << "3 Sort by user-specified characters" << endl;
	cout << "4 Standard alphabetical sort" << endl;
	cout << "5 Return(Exit)" << endl;
}

void ViewFinalSortedMenu(const string& fileName)
{
	ConsoleClear();

	cout << "1 Save sorted file (as Sorted_" << fileName << ")" << endl;
	cout << "2 Save and Open sorted file" << endl;
	cout << "3 Print sorted file" << endl;
	cout << "4 Return(Exit)" << endl;
}

void ViewFileSystemElements(const vector<string>& items, const string& currentPath)
{
	ConsoleClear();
	cout << "> \"" << currentPath << "\"" << endl;
	cout << "r Return(Exit)" << endl;
	if (!currentPath.empty())
	{
		cout << "s Save folder path and leave" << endl;
		cout << "b Back at 1 step" << endl;
		cout << "d Go to Drives (Root)" << endl;
	}
	cout << "-------------------------------------------" << endl;
	int count = 1;
	for (string line : items)
	{
		cout << count << " " << line << endl;
		count++;
	}
}

void ViewPath(int choise)
{
	ifstream in(FileToSavePath);
	if (!in)
		cerr << "Failed to open file to reading new path!" << endl;
	string FolderPath, FilePath;
	getline(in, FolderPath);
	getline(in, FilePath);
	in.close();

	switch (choise)
	{
	case 1:
		cout << "New Folder: \"" << FolderPath << "\"" << endl;
		break;
	case 2:
		cout << "New File: \"" << FilePath << "\"" << endl;
		break;
	case 3:
		cout << "New Folder: \"" << FolderPath << "\"" << endl;
		cout << "New File:   \"" << FilePath << "\"" << endl;
		break;
	}
}

void printError(string mes)
{
	cerr << "Error: " << mes << endl;
	Countdown();
}

void PrintData(const vector<string>& data)
{
	ConsoleClear();
	for (const auto& line : data)
		cout << line << endl;
	cout << endl;
	system("pause");
}

void PrintData(const vector<Group>& groups)
{
	ConsoleClear();
	for (const auto& group : groups)
	{
		for (const auto& line : group.Lines)
			cout << "  " << line << endl;
		cout << endl;
	}
	cout << endl;
	system("pause");
}
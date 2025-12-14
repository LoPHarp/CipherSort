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
	cout << "1 Set Folder" << endl;
	cout << "2 Set File" << endl;
}

void ViewsetFolderMenu()
{
	ConsoleClear();
	cout << "1 Enter the full path manually" << endl;
	cout << "2 Navigate step by step by selecting folders/files" << endl;
	cout << "3 Select using Windows Dialog (Standard)" << endl;
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
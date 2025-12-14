#include "UserInput.h"

#include <windows.h>
#include <commdlg.h>
#include <iostream>
#include <string>
#include <cctype>
#include <algorithm>

using namespace std;

string InputString()
{
	string path = "";
	cout << ": ";
	getline(cin, path);
	return path;
}

int InputDigitalValue()
{
	while (true)
	{
		string input = InputString();
		if (input.empty()) continue;

		bool isNumber = true;
		for (char c : input)
		{
			if (!isdigit(c))
			{
				isNumber = false;
				break;
			}
		}

		if (isNumber)
			return stoi(input);
		else
		{
			cout << "Only digital value!" << endl;
		}
	}
}

int CheckInputType(const string& input)
{
	if (input.empty())
		return 0;

	bool isNumber = all_of(input.begin(), input.end(), ::isdigit);
	if (isNumber)
		return 1;
	else if (input.length() == 1)
		return 2;
	else
		return 0;
}

string OpenWindowsFileDialog()
{
	char filename[MAX_PATH];

	OPENFILENAMEA ofn;
	ZeroMemory(&filename, sizeof(filename));
	ZeroMemory(&ofn, sizeof(ofn));

	ofn.lStructSize = sizeof(ofn);
	ofn.hwndOwner = NULL; 
	ofn.lpstrFile = filename;
	ofn.nMaxFile = MAX_PATH;
	ofn.lpstrFilter = "All Files\0*.*\0Text Files\0*.txt\0"; 
	ofn.nFilterIndex = 1;
	ofn.lpstrTitle = "Select a File"; 
	ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_NOCHANGEDIR;

	if (GetOpenFileNameA(&ofn))
	{
		return string(filename);
	}

	return "";
}
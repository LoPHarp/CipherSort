#include "FileSorter.h"
#include "FileSelector.h"
#include "View.h"
#include "Userinput.h"

#include <filesystem>
#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <algorithm>

using namespace std;
namespace fs = filesystem;

bool CompareGroups(const Group& a, const Group& b)
{
	return a.Key < b.Key;
}

void SaveData(const string& path, const vector<string>& data)
{
	ofstream out(path);
	if (!out.is_open()) {
		printError("Cannot save file!");
		return;
	}
	for (const auto& line : data)
		out << line << endl;
	out.close();
	cout << "File saved successfully to: " << path << endl;
	Countdown(2);
}

void SaveData(const string& path, const vector<Group>& groups)
{
	ofstream out(path);
	if (!out.is_open()) {
		printError("Cannot save file!");
		return;
	}
	for (const auto& group : groups)
	{
		for (const auto& line : group.Lines)
			out << line << endl;
		out << endl;
	}
	out.close();
	cout << "File saved successfully to: " << path << endl;
	Countdown(2);
}

void FileSort(const string& path)
{
	if (path.empty())
	{
		printError("Internal Error: FileSort received empty path!");
		return;
	}

	ifstream in(path);
	if (!in.is_open())
	{
		printError("Cannot open file!");
		return;
	}
	string line;
	vector<string> NotSortedData;
	while (getline(in, line))
		if (!line.empty())
			NotSortedData.push_back(line);
	in.close();

	if (NotSortedData.empty())
	{
		printError("File is empty!");
		return;
	}

	ViewSortedMenu();
	int choice = InputDigitalValue();

	vector<string> simpleResult; 
	vector<Group> groupResult; 
	bool isSimpleSort = (choice == 4);

	if (isSimpleSort)
	{
		simpleResult = NotSortedData;
		sort(simpleResult.begin(), simpleResult.end());
	}
	else
	{
		int N = 5;
		string UserKey;
		if (choice == 2)
		{
			cout << "N";
			N = InputDigitalValue();
		}
		else if (choice == 3)
		{
			cout << "Key";
			UserKey = InputString();
		}

		for (const string& curentLine : NotSortedData)
		{
			string key;
			if (choice == 1 || choice == 2)
			{
				if (curentLine.length() >= N)
					key = curentLine.substr(0, N);
				else
					key = curentLine;
			}
			else if (choice == 3)
			{
				if (curentLine.find(UserKey) != 0)
					continue;
				key = UserKey;
			}

			bool found = false;
			for (auto& group : groupResult)
			{
				if (group.Key == key)
				{
					group.Lines.push_back(curentLine);
					found = true;
					break;
				}
			}

			if (!found)
			{
				Group NewGroup;
				NewGroup.Key = key;
				NewGroup.Lines.push_back(curentLine);
				groupResult.push_back(NewGroup);
			}
		}
		sort(groupResult.begin(), groupResult.end(), CompareGroups);
	}
	
	fs::path p(path);
	string fileName = p.filename().string();
	fs::path newPath = p.parent_path() / ("Sorted_" + fileName);
	string SavePath = newPath.string();
	while (true)
	{
		ViewFinalSortedMenu(fileName);
		int action = InputDigitalValue();

		if (action == 1 || action == 2)
		{
			if (isSimpleSort)
				SaveData(SavePath, simpleResult);
			else
				SaveData(SavePath, groupResult);
			if (action == 2)
			{
				string command = "notepad \"" + SavePath + "\"";
				system(command.c_str());
			}
		}
		else if (action == 3)
		{
			if (isSimpleSort)
				PrintData(simpleResult);
			else
				PrintData(groupResult);
		}
		else if (action == 4)
		{
			break;
		}
	}
}
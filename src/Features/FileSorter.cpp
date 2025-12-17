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
#include <map>

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
		if (group.Key == "DUPLICATES")
			out << "\n=== DUPLICATES (Extracted copies) ===" << endl;

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

	map<string, int> lineCounts;
	string line;
	vector<string> originalOrder;

	while (getline(in, line))
	{
		if (!line.empty()) {
			lineCounts[line]++;
		}
	}
	in.close();

	if (lineCounts.empty()) { printError("File is empty!"); return; }

	vector<string> heavyDuplicates;
	for (const auto& pair : lineCounts)
	{
		if (pair.second >= 2) 
		{
			heavyDuplicates.push_back(pair.first);
		}
	}

	int dupMode = 0;
	if (!heavyDuplicates.empty())
	{
		ConsoleClear();
		cout << "Found " << heavyDuplicates.size() << " lines that repeat 2 or more times." << endl;
		cout << "1. Keep all (Do nothing)" << endl;
		cout << "2. Deduplicate (Keep 1 copy in main list, delete others)" << endl;
		cout << "3. Move to separate group at the END (1 copy only)" << endl;

		int dChoice = InputDigitalValue();
		if (dChoice == 2) dupMode = 1;
		else if (dChoice == 3) dupMode = 2;
	}

	vector<string> NotSortedData;
	vector<string> EndGroupData;

	for (const auto& pair : lineCounts)
	{
		string txt = pair.first;
		int count = pair.second;

		if (dupMode == 0) 
		{
			for (int i = 0; i < count; i++)
				NotSortedData.push_back(txt);
		}
		else if (dupMode == 1) 
		{
			NotSortedData.push_back(txt);
		}
		else if (dupMode == 2) 
		{
			if (count >= 2)
			{
				NotSortedData.push_back(txt);
				EndGroupData.push_back(txt);
			}
			else
			{
				NotSortedData.push_back(txt);
			}
		}
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

		if (!EndGroupData.empty())
		{
			simpleResult.push_back(""); 
			simpleResult.push_back("=== DUPLICATES ===");
			for (const auto& d : EndGroupData) simpleResult.push_back(d);
		}
	}
	else
	{
		int N = 5;
		string UserKey;
		if (choice == 2) { cout << "N"; N = InputDigitalValue(); }
		else if (choice == 3) { cout << "Key"; UserKey = InputString(); }

		for (const string& curentLine : NotSortedData)
		{
			string key;
			if (choice == 1 || choice == 2)
			{
				if (curentLine.length() >= N) key = curentLine.substr(0, N);
				else key = curentLine;
			}
			else if (choice == 3)
			{
				if (curentLine.find(UserKey) != 0) continue;
				key = UserKey;
			}

			bool found = false;
			for (auto& group : groupResult)
			{
				if (group.Key == key) {
					group.Lines.push_back(curentLine);
					found = true;
					break;
				}
			}

			if (!found) {
				Group NewGroup;
				NewGroup.Key = key;
				NewGroup.Lines.push_back(curentLine);
				groupResult.push_back(NewGroup);
			}
		}
		sort(groupResult.begin(), groupResult.end(), CompareGroups);

		if (!EndGroupData.empty())
		{
			Group dupGroup;
			dupGroup.Key = "DUPLICATES";
			dupGroup.Lines = EndGroupData;
			groupResult.push_back(dupGroup);
		}
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
			if (isSimpleSort) SaveData(SavePath, simpleResult);
			else SaveData(SavePath, groupResult);

			if (action == 2)
			{
				string command = "notepad \"" + SavePath + "\"";
				system(command.c_str());
			}
		}
		else if (action == 3)
		{
			if (isSimpleSort) PrintData(simpleResult);
			else PrintData(groupResult);
		}
		else if (action == 4) break;
	}
}
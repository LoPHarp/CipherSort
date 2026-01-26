/*
#include "LinkExecutor.h"
#include "View.h"
#include "UserInput.h" 

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <windows.h>
#include <cstdlib>
#include <filesystem>
#include <sstream>

using namespace std;
namespace fs = filesystem;

void ActionOpenLinks(const string& path)
{
	if (path.empty()) { printError("Path is empty!"); return; }

	ifstream in(path);
	if (!in.is_open()) { printError("Cannot open file!"); return; }

	vector<string> lines;
	string line;
	while (getline(in, line))
	{
		lines.push_back(line);
	}
	in.close();

	if (lines.empty()) {
		printError("File is empty.");
		return;
	}

	while (true)
	{
		ConsoleClear();
		cout << "--- File Content (" << fs::path(path).filename().string() << ") ---" << endl;

		for (size_t i = 0; i < lines.size(); i++)
		{
			cout << (i + 1) << ". " << lines[i] << endl;
		}

		cout << "----------------------------------------" << endl;
		cout << "Enter line number (e.g., '5') OR range (e.g., '1 5')." << endl;
		cout << "Enter '0' to Return." << endl;

		string input = InputString();

		if (input.empty()) continue;

		stringstream ss(input);
		int start = 0, end = 0;

		ss >> start; 

		if (start == 0) return;

		if (!(ss >> end))
		{
			end = start;
		}

		if (start < 1 || end >(int)lines.size() || start > end)
		{
			printError("Invalid range! Check line numbers.");
			continue;
		}

		int openedCount = 0;
		cout << "Processing lines " << start << " to " << end << "..." << endl;

		for (int i = start - 1; i < end; i++)
		{
			string content = lines[i];

			bool looksLikeLink = (content.find("http") != string::npos) || (content.find("www") != string::npos);

			if (looksLikeLink)
			{
				ShellExecuteA(NULL, "open", content.c_str(), NULL, NULL, SW_SHOWNORMAL);

				openedCount++;
				Sleep(100); 
			}
		}

		cout << "Done! Opened " << openedCount << " links." << endl;
		Countdown(3);
	}
}
*/

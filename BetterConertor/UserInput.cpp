#include "UserInput.h"

#include <iostream>
#include <string>
#include <cctype>
#include <algorithm>

using namespace std;

int InputDigitalValue()
{
	while (true)
	{
		cout << ": ";
		string choise;
		cin >> choise;

		bool isNumber = true;
		for (char c : choise)
			if (!isdigit(c))
			{
				isNumber = false;
				break;
			}

		if (isNumber)
			return stoi(choise);
		else
		{
			cout << "Only digital value!" << endl;
			continue;
		}
	}
}

string InputString()
{
	string path = "";
	cout << ": ";
	getline(cin, path);
	return path;
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
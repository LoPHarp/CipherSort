#include "UserInput.h"

#include <iostream>
#include <string>
#include <cctype>

using namespace std;

void ConsoleClear()
{
	cout << "\x1B[2J\x1B[H";
}

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
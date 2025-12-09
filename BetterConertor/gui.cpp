#include "gui.h"
#include "file.h"

#include <string>
#include <cctype>

using namespace std;

void menu()
{
	while (true)
	{
		cout << "1. Set File" << endl;
		cout << ": " << endl;

		string choise;
		cin >> choise;

		bool isNumber = true;
		for (char c : choise)
			if (!isdigit(c))
			{
				isNumber = false;
				break;
			}

		int number = 0;
		if (isNumber)
			number = stoi(choise);
		else
		{
			cout << "Only digital value!\n\n" << endl;
			continue;
		}

		switch (number)
		{
			case 1:
				setFile();
				break;

		}
			

	}
}
#include "gui.h"
#include "file.h"
#include "UserInput.h"

#include <iostream>
#include <string>

using namespace std;

void menu()
{
	while (true)
	{
		ConsoleClear();

		cout << "1. Set File" << endl;

		int choise = InputDigitalValue();

		switch (choise)
		{
			case 1:
				setFolder();
				break;

		}
			

	}
}
#include "gui.h"
#include "file.h"
#include "UserInput.h"
#include "View.h"

#include <iostream>
#include <string>

using namespace std;

void menu()
{
	while (true)
	{
		ViewMainMenu();
		int choise = InputDigitalValue();

		switch (choise)
		{
			case 1:
				setFolder();
				break;
			case 2:
				break;
		}
			

	}
}
#include "gui.h"
#include "FileSelector.h"
#include "FileSorter.h"
#include "FileCipher.h"
#include "FileActions.h"
#include "UserInput.h"
#include "View.h"
#include "LinkExecutor.h"

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
				FileSelect();
				break;
			case 2:
			{
				string currentPath = GetFilePath();
				if (!currentPath.empty())
				{
					FileSort(currentPath);
				}
				else
				{
					printError("File is not selected or does not exist!");
				}
				break;
			}
			case 3:
			{
				string currentPath = GetFilePath();
				if (!currentPath.empty())
				{
					FileCipher(currentPath);
				}
				else
				{
					printError("File is not selected or does not exist!");
				}
				break;
			}
			case 4:
			{
				string currentPath = GetFilePath();
				if (!currentPath.empty()) ActionViewContent(currentPath);
				else printError("File is not selected!");
				break;
			}
			case 5:
			{
				string currentPath = GetFilePath();
				if (!currentPath.empty()) ActionOpenFolder(currentPath);
				else printError("File is not selected!");
				break;
			}
			case 6:
			{
				string currentPath = GetFilePath();
				if (!currentPath.empty()) ActionOpenFile(currentPath);
				else printError("File is not selected!");
				break;
			}
			case 7:
			{
				string currentPath = GetFilePath();
				if (!currentPath.empty()) ActionOpenLinks(currentPath);
				else printError("File is not selected!");
				break;
			}
		}
			

	}
}
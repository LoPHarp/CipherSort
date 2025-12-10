#include "file.h"
#include "UserInput.h"

#include <unistd.h>
#include <iostream>
#include <fstream>
#include <string>
#include <windows.h>
#include <vector>
#include <filesystem>

using namespace std;
namespace fs = filesystem;

string FolderPath = "";
string FilePath = "";

void setFolder()
{
	int choise = 0;
	while (true)
	{
		ConsoleClear();
		cout << "1 Enter the full path manually" << endl;
		cout << "2 Navigate step by step by selecting folders/files" << endl;
		choise = InputDigitalValue();
		if (choise > 0 && choise < 3)
			break;
	}

	switch (choise)
	{
		case 1:
			while (true)
			{
				ConsoleClear();
				cout << "Enter the path to a folder or file:" << endl;
				string input = InputString();

				fs::path p(input);

				if (fs::is_directory(p))
				{
					cout << "Such a path does not exist!" << endl;
					continue;
				}

				if (fs::is_directory(p))
				{
					FolderPath = p.string();
					break;
				}
				else if (fs::is_regular_file(p))
				{
					FilePath = p.string();
					FolderPath = p.parent_path().string();
					break;
				}
				else 
				{
					std::cout << "This is neither a regular file nor a folder." << std::endl;
				}
				
				if (!FilePath.empty() || !FolderPath.empty())
				{
					if(!FolderPath.empty())
						cout << "Folder: " << FolderPath << std::endl;
					if (!FilePath.empty())
						cout << "File: " << FilePath << endl;
					for (int i = 3; i > 0; i--)
					{
						cout << i << " ";
						sleep(3); //ß ÎÑÒÀÍÎÂÈËÑß ÍÀ ÒÎÌ ×ÒÎÁÛ ÑÄÅËÀÒÜ ÇÀÄÅÐÆÊÓ, ÒÈÏÎ ÒÀÉÌÅÐ Ñ ÂÛÂÎÄÎÌ ÇÀÏÈÑÀÍÍÎÃÎ ÔÀÉËÀ È ÏÀÏÊÈ
					}
				}
			}
			break;

		case 2:
			break;
	}
	while (true)
	{
		ConsoleClear();
		if (FolderPath.empty())
		{
			DWORD mask = GetLogicalDrives();
			cout << "\"\"" << endl;

			int num = 1;
			vector<string> files;
			for (char drive = 'A'; drive < 'Z'; drive++)
				if (mask & (1 << (drive - 'A')))
				{
					cout << num << " " << drive << ":\\" << endl;
					num++;
					files.push_back(string(1, drive) + ":\\");
				}

			FolderPath = files[InputDigitalValue()];
		}
		else
		{

		}
	}
}
#include "file.h"
#include "UserInput.h"
#include "View.h"
#include "Config.h"

#include <iostream>
#include <fstream>
#include <string>
#include <windows.h>
#include <vector>
#include <filesystem>

using namespace std;
namespace fs = filesystem;

class ChoiseFileOrFolder
{
private:
	string FolderPath, FilePath;

	void SetFolderPath(const string path)
	{
		ifstream in(FileToSavePath);
		if (in.is_open()) 
		{
			getline(in, FolderPath);
			getline(in, FilePath);
			in.close();
		}

		FolderPath = path;

		ofstream out(FileToSavePath);
		if (out.is_open()) 
		{
			out << FolderPath << endl;
			out << FilePath << endl;
			out.flush();
			out.close();
		}
		else 
		{
			printError("Error: Cannot open file for writing! Check permissions.");
		}
	}
	void SetFilePath(const string path)
	{
		ifstream in(FileToSavePath);
		if (in.is_open()) 
		{
			getline(in, FolderPath);
			getline(in, FilePath);
			in.close();
		}

		fs::path p(path);
        FilePath = p.string();
        FolderPath = p.parent_path().string();

		ofstream out(FileToSavePath);
		if (out.is_open()) 
		{
			out << FolderPath << endl;
			out << FilePath << endl;
			out.flush(); 
			out.close();
		}
		else 
		{
			printError("Error: Cannot open file for writing! Check permissions.");
		}
	}
public:
	string getFolderPath() const { return FolderPath; }
	string getFilePath() const { return FilePath; }

	void SetFolderOrFilePath(const string path)
	{
		fs::path p(path);

		if (fs::is_directory(p))
		{
			SetFolderPath(path);
			ViewPath(1);
			return;
		}
		else if (fs::is_regular_file(p))
		{
			SetFilePath(path);
			ViewPath(3);
			return;
		}
		else
		{
			printError("This is neither a regular file nor a folder.");
			return;
		}
	}

	bool PathIsExist(const string path) const
	{
		fs::path p(path);
		if (!fs::exists(p))
		{
			cout << "Such a path does not exist!" << endl;
			return false;
		}
		return true;
	}
};

void setFolder()
{
	int choise = 0;
	while (true)
	{
		ViewsetFolderMenu();
		choise = InputDigitalValue();
		if (choise > 0 && choise < 3)
			break;
	}

	cin.ignore();

	ChoiseFileOrFolder FF;
	switch (choise)
	{
		case 1:
		{
			while (true)
			{
				ConsoleClear();
				cout << "Enter the path to a folder or file:" << endl;
				string input = InputString();

				if (FF.PathIsExist(input))
				{
					FF.SetFolderOrFilePath(input);
					break;
				}
				else
					continue;
			}
			break;
		}
		case 2: 
		{
			bool start = true;
			string FolderOrFilePath;
			while (true)
			{
				if (start)
				{
					DWORD mask = GetLogicalDrives();
					vector<string> files;

					int count = 1;
					for (char drive = 'A'; drive < 'Z'; drive++)
						if (mask & (1 << (drive - 'A')))
						{
							files.push_back(string(1, drive) + ":\\");
							count++;
						}
					ViewFileSystemElements(files);
					string input = InputString();
					switch (CheckInputType(input))
					{
					case 0:
					{
						printError("Uncorrect input.");
						Countdown();
						break;
					}
					case 1:
					{
						int choise = stoi(input);
						if (choise <= files.size() && choise >= 0)
						{
							FolderOrFilePath = files[choise - 1];
							start = false;
						}
						else
						{
							printError("This disk is not available.");
							Countdown();
						}
						break;
					}
					case 2:
					{
						char choise = input[0];
						if (choise == 'r')
						{
							return;
						}
						else
						{
							printError("Not correct input.");
							Countdown();
						}
						break;
					}
					}
				}
				else
				{
					vector<string> files, forView;
					try 
					{
						int count = 1;
						for (const auto& entry : fs::directory_iterator(FolderOrFilePath))
						{
							forView.push_back(entry.path().filename().string());
							count++;
							files.push_back(entry.path().string());
						}
					}
					catch (const fs::filesystem_error& e) {
						cerr << "Acces error: " << e.what() << endl;
					}

					ViewFileSystemElements(forView, FolderOrFilePath);
					string input = InputString();
					switch (CheckInputType(input))
					{
					case 0:
					{
						printError("Uncorrect input.");
						Countdown();
						break;
					}
					case 1:
					{
						int choise = stoi(input);
						if (choise <= files.size() && choise >= 0)
						{
							FolderOrFilePath = files[choise - 1];
						}
						else
						{
							printError("This disk is not available.");
							Countdown();
						}
						break;
					}
					case 2:
					{
						char choise = input[0];
						if (choise == 'r')
						{
							return;
						}
						else if (choise == 's')
						{
							if (FF.PathIsExist(FolderOrFilePath))
							{
								FF.SetFolderOrFilePath(FolderOrFilePath);
								return;
							}
							else
							{
								printError("Such a path does not exist!");
								return;
							}
						}
						else
						{
							printError("Not correct input.");
							Countdown();
						}
						break;
					}
					}
				}
			}
			break;
		}
	}
}
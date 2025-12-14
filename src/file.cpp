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
			Countdown(5);
		}
		else if (fs::is_regular_file(p))
		{
			SetFilePath(path);
			ViewPath(3);
			Countdown(5);
		}
		else
		{
			printError("This is neither a regular file nor a folder.");
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

bool IsFileSelected()
{
	ifstream in(FileToSavePath);
	if (!in) 
	{
		printError("File and folder not selected.");
		return false;
	}

	string folder, file;
	getline(in, folder);
	getline(in, file); 
	in.close();

	if (!file.empty() && fs::exists(file) && fs::is_regular_file(file))
	{
		return true;
	}

	printError("The file is not selected or does not exist.");
	return false;
}

void setFolder()
{
	int choice = 0;
	while (true)
	{
		ViewsetFolderMenu();
		choice = InputDigitalValue();
		if (choice > 0 && choice < 5)
			break;
	}

	ChoiseFileOrFolder FF;
	switch (choice)
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
						break;
					}
					case 1:
					{
						int intInput = stoi(input);
						if (intInput <= files.size() && intInput >= 0)
						{
							FolderOrFilePath = files[intInput - 1];
							start = false;
						}
						else
						{
							printError("This disk is not available.");
						}
						break;
					}
					case 2:
					{
						char charInput = input[0];
						if (charInput == 'r')
						{
							return;
						}
						else
						{
							printError("Not correct input.");
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
						break;
					}
					case 1:
					{
						int intInput = stoi(input);
						if (intInput <= files.size() && intInput >= 0)
						{
							string selectedPath = files[intInput - 1];
							fs::path p(selectedPath);

							if (fs::is_directory(p))
							{
								FolderOrFilePath = selectedPath;
							}
							else
							{
								FF.SetFolderOrFilePath(selectedPath);
								return;
							}
						}
						else
						{
							printError("This disk is not available.");
						}
						break;
					}
					case 2:
					{
						char charInput = input[0];
						if (charInput == 'r')
						{
							return;
						}
						else if (charInput == 'b')
						{
							fs::path p(FolderOrFilePath);
							if (p.has_parent_path() && p != p.root_path())
							{
								FolderOrFilePath = p.parent_path().string();
							}
							else
							{
								start = true; 
							}
						}
						else if (charInput == 's')
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
						}
						break;
					}
					}
				}
			}
			break;
		}
		case 3:
		{
			ConsoleClear();
			cout << "Opening Windows Dialog..." << endl;

			string selectedPath = OpenWindowsFileDialog();

			if (selectedPath.empty())
			{
				printError("File selection canceled.");
			}
			else
			{
				if (FF.PathIsExist(selectedPath))
				{
					FF.SetFolderOrFilePath(selectedPath);
				}
			}
			break;
		}
		case 4:
			return;
	}
}
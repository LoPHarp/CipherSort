#include "FileActions_Console.h"
#include "View.h"
#include "Userinput.h"
#include "FileCipher.h"
#include "FileSorter.h"

#include <iostream>
#include <filesystem>
#include <chrono>

using namespace std;
namespace fs = std::filesystem;

void ActionCipherFile_Console(const string& path)
{
	if (path.empty())
	{
		printError("File is not selected!");
		return;
	}

	CipherMode mode;
	while (true)
	{
		ConsoleClear();
		cout << "Mode:\n0 Return\n1 Encrypt\n2 Decrypt" << endl;
		int choice = InputDigitalValue();
		switch (choice)
		{
		case 0:
			return;
		case 1:
			mode = CipherMode::Encrypt;
			break;
		case 2:
			mode = CipherMode::Decrypt;
			break;
		default:
			continue;
		}
		break;
	}
	ConsoleClear();

	string outPath = path;
	if (mode == CipherMode::Encrypt)
	{
		outPath = path + ".crp";
	}
	else
	{
		if (path.length() > 4 && path.substr(path.length() - 4) == ".crp")
			outPath = path.substr(0, path.length() - 4);
	}

	string tempPath = path + ".tmp";

	cout << "Enter Key: ";
	string key = InputString();
	ConsoleClear();

	CipherResult res = ProcessFile(key, mode, path, tempPath, false, CipherMethod::RC4);

	if (res == CipherResult::Success)
	{
		if (fs::exists(path))
			fs::remove(path);

		fs::rename(tempPath, outPath);

		cout << "Success! File updated: " << outPath << endl;
	}
	else
	{
		if (fs::exists(tempPath))
			fs::remove(tempPath);

		printError("Encryption/Decryption failed! Check your key or file format.");
	}
}

void ActionSortFile_Console(const string& path)
{
	if (path.empty())
	{
		printError("File is not selected!");
		return;
	}

	SortConfig config;

	ConsoleClear();
	ViewSortedMenu();
	int	choice = InputDigitalValue();

	ConsoleClear();
	switch (choice)
	{
	case 1:
		config.method = SortMethod::Prefix;
		config.nChars = 5;
		break;
	case 2:
		config.method = SortMethod::Prefix;
		cout << "Enter number of characters: ";
		config.nChars = InputDigitalValue();
		break;
	case 3:
		config.method = SortMethod::Alphabetical;
		break;
	case 4:
		config.method = SortMethod::WholeLine;
		break;
	case 5:
		config.method = SortMethod::PrefixAlphabetical;
		cout << "Enter number of characters: ";
		config.nChars = InputDigitalValue();
		break;
	default:
		return;
	}

	ConsoleClear();
	ViewDuplicateMenu();
	int dupChoice = InputDigitalValue();
	switch (dupChoice)
	{
	case 1: config.dupMode = DuplicateMode::KeepAll; break;
	case 2: config.dupMode = DuplicateMode::Deduplicate; break;
	case 3: config.dupMode = DuplicateMode::MoveToEnd; break;
	default: return;
	}

	ConsoleClear();
	cout << "Include group headers in file?\n1 Yes\n2 No\n";
	config.includeGroupNames = (InputDigitalValue() == 1);

	vector<Group> sortRes;
	
	auto start = chrono::steady_clock::now();
	SortResult res = ProcessorSorting(config, path, sortRes);
	auto end = chrono::steady_clock::now();
	auto diff = chrono::duration_cast<chrono::milliseconds>(end - start);

	choice = 0;
	if (res == SortResult::Success)
	{
		ConsoleClear();
		cout << "Sorted in: " << diff.count() << " ms" << endl;
		Countdown();
		ConsoleClear();

		while (true)
		{
			ViewFinalSortedMenu();
			choice = InputDigitalValue();
			ConsoleClear();

			fs::path p(path);
			string outPath = (p.parent_path() / ("Sorted_" + p.filename().string())).string();
			SortResult SaveResult = SortResult::Success;
			switch (choice)
			{
			case 1:
			{
				SaveResult = SaveResultToFile(outPath, sortRes, config.includeGroupNames);
				break;
			}
			case 2:
			{
				SaveResult = SaveResultToFile(outPath, sortRes, config.includeGroupNames);
				if (SaveResult == SortResult::Success)
				{
					string command = "notepad \"" + outPath + "\"";
					system(command.c_str());
				}
				break;
			}
			case 3:
			{
				PrintData(sortRes, config.includeGroupNames);
				break;
			}
			case 4:
				return;
			default:
				continue;
			}
			if (SaveResult == SortResult::Success && choice != 3)
			{
				cout << "File saved to " << outPath << endl;
				Countdown();
			}
			else if(SaveResult != SortResult::Success)
				printError("Save sorted file failed!");
		}	
	}
	else
	{
		string errorMsg = "Unknown error";
		if (res == SortResult::OpenError) errorMsg = "Cannot open input file";
		else if (res == SortResult::EmptyFile) errorMsg = "File is empty";
		else if (res == SortResult::SaveError) errorMsg = "Cannot save output file";

		printError("Sorting failed: " + errorMsg);
	}
}

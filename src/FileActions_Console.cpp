#include "FileActions_Console.h"
#include "View.h"
#include "Userinput.h"
#include "FileCipher.h"
#include "FileSorter.h"

#include <iostream>
#include <filesystem>

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

	int choice = 0;
	while (true)
	{
		ConsoleClear();
		ViewSortedMenu();
		choice = InputDigitalValue();
		if (choice == 6)
			return;
		if (choice > 0 && choice < 6)
			break;
	}

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
		cout << "Enter number of characters for grouping: ";
		config.nChars = InputDigitalValue();
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
	cout << "Duplicates Mode:\n1 Keep All\n2 Deduplicate\n3 Move to End\n4 Return" << endl;
	int dupChoice = InputDigitalValue();
	switch (dupChoice)
	{
	case 1: config.dupMode = DuplicateMode::KeepAll; break;
	case 2: config.dupMode = DuplicateMode::Deduplicate; break;
	case 3: config.dupMode = DuplicateMode::MoveToEnd; break;
	default: return;
	}

	ConsoleClear();
	cout << "Include group headers in file?\n1 Yes\n2 No ";
	config.includeGroupNames = (InputDigitalValue() == 1);

	fs::path p(path);
	string outPath = (p.parent_path() / ("Sorted_" + p.filename().string())).string();

	ConsoleClear();
	cout << "Sorting in progress..." << endl;
	SortResult res = ProcessorSorting(config, path, outPath);

	if (res == SortResult::Success)
	{
		cout << "Success! Sorted file saved " << outPath << endl;
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

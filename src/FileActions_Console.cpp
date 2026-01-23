#include "FileActions_Console.h"
#include "View.h"
#include "Userinput.h"
#include "FileCipher.h"

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

	string tempPath = path + ".tmp";

	ConsoleClear();
	cout << "Mode:\n1 Encrypt\n2 Decrypt" << endl;
	int choice = InputDigitalValue();
	CipherMode mode = (choice == 1) ? CipherMode::Encrypt : CipherMode::Decrypt;

	ConsoleClear();
	cout << "Enter Key: ";
	string key = InputString();

	ConsoleClear();

	CipherResult res = ProcessFile(key, mode, path, tempPath, false, CipherMethod::RC4);

	if (res == CipherResult::Success)
	{
		if (fs::exists(path))
			fs::remove(path);

		fs::rename(tempPath, path);

		cout << "Success! File updated." << endl;
	}
	else
	{
		if (fs::exists(tempPath))
			fs::remove(tempPath);

		printError("Encryption/Decryption failed!");
	}
}
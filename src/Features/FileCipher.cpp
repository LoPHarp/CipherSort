#include "FileCipher.h"
#include "FileSelector.h"
#include "UserInput.h"
#include "View.h"
#include "Config.h"

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <filesystem>

using namespace std;
namespace fs = std::filesystem;

const string FILE_SIGNATURE = "CHECK_PASS_OK";

void FileCipher(const string& path)
{
	if (path.empty()) 
	{
		printError("Internal Error: FileCipher received empty path!");
		return;
	}

	ConsoleClear();
	cout << "Enter encryption key (password): ";
	string key = InputString();

	if (key.empty())
	{
		printError("Key cannot be empty! Operation canceled.");
		return;
	}

	ifstream in(path, ios::binary);
	if (!in.is_open()) 
	{
		printError("Cannot open file!");
		return;
	}

	vector<char> buffer((istreambuf_iterator<char>(in)), istreambuf_iterator<char>());
	in.close();

	if (buffer.empty()) 
	{
		printError("File is empty!");
		return;
	}

	fs::path p(path);
	string filename = p.filename().string();
	bool isDecryptionMode = (filename.find("Crypted_") == 0);

	if (isDecryptionMode)
	{
		cout << "Mode: Decryption..." << endl;

		if (buffer.size() < FILE_SIGNATURE.length())
		{
			printError("File is corrupted (too small) or not encrypted by this program.");
			return;
		}

		vector<char> checkBuffer;
		for (size_t i = 0; i < FILE_SIGNATURE.length(); i++)
			checkBuffer.push_back(buffer[i]);

		for (size_t i = 0; i < checkBuffer.size(); i++)
		{
			checkBuffer[i] = checkBuffer[i] ^ key[i % key.length()];
		}

		string decryptedSignature(checkBuffer.begin(), checkBuffer.end());
		if (decryptedSignature != FILE_SIGNATURE)
		{
			printError("WRONG PASSWORD! Operation aborted. File remains safe.");
			return;
		}

		cout << "Password correct! Decrypting..." << endl;

		for (size_t i = 0; i < buffer.size(); i++)
		{
			buffer[i] = buffer[i] ^ key[i % key.length()];
		}

		buffer.erase(buffer.begin(), buffer.begin() + FILE_SIGNATURE.length());

		string newName = filename.substr(8);
		fs::path newPath = p.parent_path() / newName;

		ofstream out(newPath, ios::binary);
		if (!out.is_open()) { printError("Cannot save file!"); return; }
		out.write(buffer.data(), buffer.size());
		out.close();

		try {
			if (fs::exists(path)) fs::remove(path);
			ofstream configOut(FileToSavePath);
			if (configOut.is_open()) {
				configOut << p.parent_path().string() << endl;
				configOut << newPath.string() << endl;
				configOut.close();
			}
		}
		catch (...) {}

		cout << "Success! File decrypted." << endl;
		Countdown(3);
	}
	else
	{
		cout << "Mode: Encryption..." << endl;
		buffer.insert(buffer.begin(), FILE_SIGNATURE.begin(), FILE_SIGNATURE.end());

		for (size_t i = 0; i < buffer.size(); i++)
		{
			buffer[i] = buffer[i] ^ key[i % key.length()];
		}

		string newName = "Crypted_" + filename;
		fs::path newPath = p.parent_path() / newName;

		ofstream out(newPath, ios::binary);
		if (!out.is_open()) { printError("Cannot save file!"); return; }
		out.write(buffer.data(), buffer.size());
		out.close();

		try {
			if (fs::exists(path)) fs::remove(path);
			ofstream configOut(FileToSavePath);
			if (configOut.is_open()) {
				configOut << p.parent_path().string() << endl;
				configOut << newPath.string() << endl;
				configOut.close();
			}
		}
		catch (...) {}

		cout << "Success! File encrypted." << endl;
		Countdown(3);
	}
}
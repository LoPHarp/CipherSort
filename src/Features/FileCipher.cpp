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

const string FILE_SIGNATURE = "CPSORT";

CipherResult XOR(const string& key, ifstream& in, ofstream& out)
{
	const size_t BUFFER_SIZE = 65536;
	vector<char> buffer(BUFFER_SIZE);
	size_t totalProcessed = 0;

	while (in.read(buffer.data(), BUFFER_SIZE) || in.gcount() > 0)
	{
		size_t bytesRead = in.gcount();

		for (size_t i = 0; i < bytesRead; i++)
			buffer[i] ^= key[(totalProcessed + i) % key.length()];

		out.write(buffer.data(), bytesRead);
		totalProcessed += bytesRead;
	}

	return CipherResult::Success;
}

CipherResult ProcessFile(const string& key, CipherMode mode, const string& inpath, const string& outpath, bool DeleteInputFile, CipherMethod method)
{
	ifstream in(inpath, ios::binary);
	if (!in.is_open())
		return CipherResult::FileNotFound;

	in.seekg(0, ios::end);
	if (in.tellg() == 0)
		return CipherResult::EmptyFile;
	in.seekg(0, ios::beg);

	ofstream out(outpath, ios::binary);
	if (!out.is_open())
		return CipherResult::SaveError;

	if (mode == CipherMode::Encrypt)
	{
		string HeaderSignature = FILE_SIGNATURE + (method == CipherMethod::XOR ? 'X' : 'R');

		for (size_t i = 0; i < HeaderSignature.size(); i++)
			HeaderSignature[i] ^= key[i % key.length()];

		out.write(HeaderSignature.data(), HeaderSignature.length());
	}
	else
	{
		char headData[7];
		in.read(headData, 7);

		for (int i = 0; i < 7; i++)
		{
			headData[i] ^= key[i % key.length()];
		}
	
		string CheckSig(headData, 6);
		if (!(CheckSig == FILE_SIGNATURE))
			return CipherResult::WrongPassword;

		if (headData[6] == 'X')
			method = CipherMethod::XOR;
		else if (headData[6] == 'R')
			method = CipherMethod::RC4;
		else
			return CipherResult::UndefinedMethod;
	}

	if (method == CipherMethod::XOR)
	{
		CipherResult res = XOR(key, in, out);

		in.close();
		out.close();

		if (res == CipherResult::Success)
		{
			if (DeleteInputFile)
				if (fs::exists(inpath))
					fs::remove(inpath);
		}
		else
			return CipherResult::EncryptionError;
	}
	else if (method == CipherMethod::RC4)
	{

	}
	else
		return CipherResult::UndefinedMethod;

	return CipherResult::Success;
}
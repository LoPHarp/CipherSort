#pragma once
#include <string>
#include <iostream>

enum class CipherMode
{
	Encrypt,
	Decrypt
};

enum class CipherMethod
{
	Undefined,
	RC4,
	XOR
};

enum class CipherResult
{
	Success = 0,
	FileNotFound,
	EmptyFile,
	WrongPassword,
	SaveError,
	UndefinedMethod,
	EncryptionError
};

CipherResult ProcessFile(const string& key, CipherMode mode, const std::string& inpath, const std::string& outpath, bool DeleteInputFile = false, CipherMethod method = CipherMethod::Undefined);

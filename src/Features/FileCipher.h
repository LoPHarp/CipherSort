#pragma once
#include <string>

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

CipherResult ProcessFile(const std::string& key, const std::string& inpath, const std::string& outpath, CipherMethod method = CipherMethod::Undefined);

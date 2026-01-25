#pragma once
#include <string>
#include <vector>

enum class DuplicateMode 
{ 
	KeepAll,
	Deduplicate,
	MoveToEnd 
};

enum class SortMethod
{
	Prefix,
	WholeLine,
	Alphabetical,
	PrefixAlphabetical
};

enum class SortResult
{
	Success = 0,
	OpenError,
	EmptyFile,
	SaveError,
	UndefinedMethod
};

struct SortConfig
{
	DuplicateMode dupMode;
	SortMethod method;
	int nChars = 5;
	std::string searchKey = "";
	bool includeGroupNames = false;
};

SortResult ProcessorSorting(const SortConfig& config, const std::string& inPath, const std::string& outPath);
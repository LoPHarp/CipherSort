#pragma once
#include <string>
#include <vector>

struct Group
{
	std::string Key = "";
	std::vector<std::string> Lines;
};

void FileSort(const std::string& path);
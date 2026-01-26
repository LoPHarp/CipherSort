#include "FileSorter.h"

#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <map>

using namespace std;

string Trim(const string& inpStr)
{
	size_t first = inpStr.find_first_not_of(" \t\r\n");
	size_t last = inpStr.find_last_not_of(" \t\r\n");
	return inpStr.substr(first, (last - first + 1));
}

void AddToGroup(vector<Group>& groups, const string& key, const string& line)
{
	for (auto& group : groups)
	{
		if (group.Key == key)
		{
			group.Lines.push_back(line);
			return;
		}
	}

	Group newGroup;
	newGroup.Key = key;
	newGroup.Lines.push_back(line);
	groups.push_back(newGroup);
}

bool CompareNoCase(const string& a, const string& b)
{
	for (size_t i = 0; i < a.length() && i < b.length(); ++i)
	{
		char charA = tolower(a[i]);
		char charB = tolower(b[i]);

		if (charA != charB)
			return charA < charB;
	}

	return a.length() < b.length();
}

bool CompareAlphabeticalTrimmed(const string& a, const string& b)
{
	size_t i = a.find_first_not_of(" \t\r\n");
	size_t j = b.find_first_not_of(" \t\r\n");

	while (i < a.size() && j < b.size())
	{
		char charA = tolower(a[i]);
		char charB = tolower(b[j]);

		if (charA != charB)
			return charA < charB;

		i++;
		j++;
	}

	return (a.length() - i) < (b.length() - j);
}

vector<Group> SortByPrefix(const vector<string>& data, int n)
{
	vector<Group> result;
	for (const string& line : data)
	{
		size_t first = line.find_first_not_of(" \t\r\n");
		string cleanStart = line.substr(first);

		string key = (cleanStart.length() > n) ? cleanStart.substr(0, n) : cleanStart;
		AddToGroup(result, key, line);
	}
	return result;
}

vector<Group> SortByWholeLine(const vector<string>& data)
{
	vector<Group> result;

	for (const auto& line : data)
	{
		string key = line;
		AddToGroup(result, key, line);
	}

	return result;
}

vector<Group> SortAlphabetically(const vector<string>& data)
{
	vector<Group> result;
	Group singleGroup;
	singleGroup.Key = "ALPHABETICAL";
	singleGroup.Lines = data;

	sort(singleGroup.Lines.begin(), singleGroup.Lines.end(), CompareAlphabeticalTrimmed);

	result.push_back(singleGroup);
	return result;
}

vector<Group> SortHybrid(const vector<string>& data, int n)
{
	vector<Group> result = SortByPrefix(data, n);

	for (auto& group : result)
		sort(group.Lines.begin(), group.Lines.end(), CompareAlphabeticalTrimmed);

	return result;
}

SortResult SaveResultToFile(const string& path, const vector<Group>& groups, bool showHeaders)
{
	ofstream out(path);
	if (!out.is_open())
		return SortResult::SaveError;

	for (const auto& group : groups)
	{
		if (showHeaders && !group.Key.empty() && group.Key != "ALPHABETICAL")
			out << "======= [\"" << group.Key << "\"] =======" << "\n";

		for (const auto& line : group.Lines)
			out << line << "\n";
	}
	out << "\n";

	out.close();
	return SortResult::Success;
}

SortResult ProcessorSorting(const SortConfig& config, const string& inPath, vector<Group>& outRes)
{
	ifstream in(inPath);
	if (!in.is_open())
		return SortResult::OpenError;

	outRes.clear();

	map<string, int> lineCounts;

	string str;
	while (getline(in, str))
	{
		if (!str.empty() && str.find_first_not_of(" \t\r\n") != string::npos)
		{
			if (config.keepRowFormatting == false)
				str = Trim(str);
			lineCounts[str]++;
		}
	}
	in.close();

	if (lineCounts.empty())
		return SortResult::EmptyFile;

	vector<string> dataPool;
	vector<string> dupData;

	for (map<string, int>::iterator it = lineCounts.begin(); it != lineCounts.end(); ++it)
	{
		const string& text = it->first;
		int count = it->second;

		switch (config.dupMode)
		{
		case DuplicateMode::KeepAll:
		{
			dataPool.insert(dataPool.end(), count, text);
			break;
		}
		case DuplicateMode::Deduplicate:
			dataPool.push_back(text);
			break;
		case DuplicateMode::MoveToEnd:
		{
			dataPool.push_back(text);
			if (count > 1)
				dupData.push_back(text);
			break;
		}
		}
	}

	vector<Group> result;
	switch (config.method)
	{
	case SortMethod::Prefix:
		result = SortByPrefix(dataPool, config.nChars);
		break;
	case SortMethod::Alphabetical:
		result = SortAlphabetically(dataPool);
		break;
	case SortMethod::PrefixAlphabetical:
		result = SortHybrid(dataPool, config.nChars);
		break;
	case SortMethod::WholeLine:
		result = SortByWholeLine(dataPool);
		break;
	default:
		return SortResult::UndefinedMethod;
	}

	sort(result.begin(), result.end(), [](const Group& a, const Group& b)
	{
		return CompareNoCase(a.Key, b.Key);
	});

	outRes = move(result);

	return SortResult::Success;
}
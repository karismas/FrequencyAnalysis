#include <vector>

#include "utils.h"

std::map<std::string, double> normalizeDict(std::map<std::string, double> dict)
{
	std::map<std::string, double> normalizedDict;

	double total = 0;
	std::map<std::string, double>::iterator it = dict.begin();
	while (it != dict.end())
	{
		total += it->second;
		++it;
	}

	it = dict.begin();
	while (it != dict.end())
	{
		normalizedDict[it->first] = (it->second / total);
		++it;
	}

	return normalizedDict;
}

std::vector<char> getAlphabet()
{
	std::vector<char> alphabet;

	for (int i = 97; i < 123; ++i)
	{
		alphabet.emplace_back(char(i));
	}

	return alphabet;
}

std::vector<char> getAlphabetWithoutSH()
{
	std::vector<char> alphabet;

	for (int i = 97; i < 104; ++i)
	{
		alphabet.emplace_back(char(i));
	}

	for (int i = 105; i < 115; ++i)
	{
		alphabet.emplace_back(char(i));
	}

	for (int i = 116; i < 123; ++i)
	{
		alphabet.emplace_back(char(i));
	}

	return alphabet;
}

#include <vector>

#include "utils.h"

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

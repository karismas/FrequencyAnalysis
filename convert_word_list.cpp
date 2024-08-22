#include <sstream>
#include <fstream>
#include <string>
#include <iostream>
#include <map>

#include "utils.h"

std::map<std::string, int> getWordCount(std::string filename) 
{ 
	std::map<std::string, int> wordCount;

    std::ifstream file(filename);

    std::string line; 
    while (std::getline(file, line))
    {
		std::istringstream stream(line);
		std::string first;
		std::string second;
		std::getline(stream, first, ' ');
		std::getline(stream, second, ' ');
		wordCount[first] = std::stoi(second);
    }

	return wordCount;
}

std::map<std::string, double> getWordFrequency(std::map<std::string, int> wordCount)
{
	std::map<std::string, double> wordFrequency;

	for (auto const& [word, count] : wordCount)
	{
		wordFrequency[word] = (double)count;
	}

	return normalizeDict(wordFrequency);
}

std::map<std::string, double> getLetterFrequency(std::map<std::string, double> wordFrequency)
{
	std::map<std::string, double> letterFrequency;

	std::map<std::string, double>::iterator it = wordFrequency.begin();
	while (it != wordFrequency.end())
	{
		std::string word = it->first;
		for (int i = 0; i < word.length(); ++i)
		{
			std::string s{word[char(i)]};
			letterFrequency[s] += it->second;
		}

		++it;
	}

	return normalizeDict(letterFrequency);
}

std::map<std::string, double> getDoubleLetterFrequency(std::map<std::string, double> wordFrequency)
{
	std::map<std::string, double> doubleLetterFrequency;

	std::map<std::string, double>::iterator it = wordFrequency.begin();
	while (it != wordFrequency.end())
	{
		std::string word = it->first;
		for (int i = 0; i < word.length(); ++i)
		{
			if (i + 1 >= word.length()) break;

			std::string s1{char(word[i])};
			std::string s2{char(word[i + 1])};
			doubleLetterFrequency[s1 + s2] += it->second;
		}

		++it;
	}

	return normalizeDict(doubleLetterFrequency);
}

void printDict(std::map<std::string, double> dict)
{
	std::map<std::string, double>::iterator it = dict.begin();
	while (it != dict.end())
	{
		std::cout << it->first << ": " << it->second << "%\n";
		++it;
	}
}

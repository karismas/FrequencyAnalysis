#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>
#include <array>

#include "convert_word_list.h"
#include "utils.h"
#include "keyboard.h"

int numberPrompt(std::string question)
{
	std::string inp = "";
    while (true)
	{
		std::cout << "\n" << question << "\n";
		std::cout << ">>> ";
		std::cin >> inp;

		try
		{
			std::stoi(inp);
		}
		catch (std::invalid_argument)
		{
			std::cout << "\nThat input is not usable. Please try again.";
			continue;
		}

		break;
	}

    return std::stoi(inp);
}

std::string prompt(std::string question, std::vector<std::string> optionPrompts)
{
	std::string inp = "";
	int choice = -1;
	while (true)
	{
		std::cout << "\n" << question << "\n";

		std::cout << "Enter a number 1-" << optionPrompts.size() << ".\n";
		for (int i = 0; i < optionPrompts.size(); ++i)
		{
			std::cout << "[" << i + 1 << "] " << optionPrompts[i] << "\n";
		}

		std::cout << "\n>>> ";
		std::cin >> inp;

		try
		{
			choice = std::stoi(inp);
		}
		catch (std::invalid_argument)
		{
			std::cout << "\nThat input is not usable. Please try again.";
			continue;
		}

		if (choice < 1 || choice > optionPrompts.size())
		{
			std::cout << "\nThat is not one of the options. Please try again.";
			continue;
		}

        break;
	}

	--choice;

	return optionPrompts[choice];
}

int main()
{
	srand(time(NULL));

	std::string choice;
	std::vector<std::string> prompts = {"Yes", "No"};

	int numOrganisms;
	int numGenerations;
	int numPrint;

	choice = prompt("Would you like to continue from a previous run?\n", prompts);
	if (choice == prompts[0])
	{
		// Load previous run file
		// numOrganisms = ...
		// numGenerations = ...
		// numPrint = ...
		// ...
	}

	else
	{
		numOrganisms = numberPrompt("How many organisms would you like?\n");
		numGenerations = numberPrompt("How many generations would you like?\n");
		numPrint = numberPrompt("How many generations would you like to complete before printing the best organism's information?\n");
	}

	std::map<std::string, int> wordCount = getWordCount("WordCount.txt");
	std::map<std::string, double> wordFrequency = getWordFrequency(wordCount);
	std::map<std::string, double> letterFrequency = getLetterFrequency(wordFrequency);
	std::map<std::string, double> doubleLetterFrequency = getDoubleLetterFrequency(wordFrequency);

	Keyboard::populate(1000);
	Keyboard* bestKeyboard;
	double bestScore = (*Keyboard::keyboards)[0]->getFitnessScore(&letterFrequency, &doubleLetterFrequency);
	for (int i = 0; i < 250; ++i)
	{
		Keyboard::breedGeneration(&letterFrequency, &doubleLetterFrequency);
		double bestGenerationScore = (*Keyboard::keyboards)[0]->getFitnessScore(&letterFrequency, &doubleLetterFrequency);
		if (bestGenerationScore < bestScore)
		{
			bestKeyboard = (*Keyboard::keyboards)[0];
			bestScore = bestGenerationScore;
		}
		
		(*Keyboard::keyboards)[0]->print();
		std::cout << bestGenerationScore << "\n";
		(*Keyboard::keyboards)[0]->printFingerUsage(&letterFrequency);
		(*Keyboard::keyboards)[0]->printFingerVerticals(&doubleLetterFrequency);
	}

	std::cout << "\n\nTHE BEST\n";
	bestKeyboard->print();
	std::cout << bestScore << "\n";

	delete bestKeyboard;

	// Repeat
}

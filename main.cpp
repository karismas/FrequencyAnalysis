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

void bestKeyboardPrinter(int generation, Keyboard* keyboard, double score, std::map<std::string, double>* letterFrequency, std::map<std::string, double>* doubleLetterFrequency)
{
	std::cout << "================================================================================\n";

	std::cout << "                                 GENERATION " << generation << "\n\n";

	(*Keyboard::keyboards)[0]->printLayout();

	std::cout << "\n                                     SCORE" << "\n";
	std::cout << "                                    " << score << "\n";

	(*Keyboard::keyboards)[0]->printFingerUsage(letterFrequency);
	(*Keyboard::keyboards)[0]->printFingerVerticals(doubleLetterFrequency);
	std::cout << "================================================================================\n";
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
		std::cout << "This has not been implemented yet...\n";
		return 0;
	}

	else
	{
		numOrganisms = numberPrompt("How many organisms would you like? (must be an even number)\n");
		numGenerations = numberPrompt("How many generations would you like?\n");
		numPrint = numberPrompt("How many generations would you like to complete before printing the best organism's information?\n");
	}

	std::map<std::string, int> wordCount = getWordCount("WordCount.txt");
	std::map<std::string, double> wordFrequency = getWordFrequency(wordCount);
	std::map<std::string, double> letterFrequency = getLetterFrequency(wordFrequency);
	std::map<std::string, double> doubleLetterFrequency = getDoubleLetterFrequency(wordFrequency);

	Keyboard::populate(numOrganisms);

	Keyboard* bestKeyboard;
	int bestGeneration;
	double bestScore = (*Keyboard::keyboards)[0]->getFitnessScore(&letterFrequency, &doubleLetterFrequency);

	for (int i = 1; i <= numGenerations; ++i)
	{
		Keyboard::breedGeneration(&letterFrequency, &doubleLetterFrequency);

		double bestGenerationScore = (*Keyboard::keyboards)[0]->getFitnessScore(&letterFrequency, &doubleLetterFrequency);
		if (bestGenerationScore < bestScore)
		{
			bestKeyboard = (*Keyboard::keyboards)[0];
			bestScore = bestGenerationScore;
			bestGeneration = i;
		}

		if (i % numPrint == 0)
		{
			std::cout << "\n\n";
			bestKeyboardPrinter(i, (*Keyboard::keyboards)[0], bestGenerationScore, &letterFrequency, &doubleLetterFrequency);
		}
	}

	std::cout << "\n\n\n*********************************** MOST FIT ***********************************\n";
	bestKeyboardPrinter(bestGeneration, bestKeyboard, bestScore, &letterFrequency, &doubleLetterFrequency);
	std::cout << "********************************************************************************\n\n";

	Keyboard::printStatistics();

	delete Keyboard::temp;
	for (int i = 0; i < Keyboard::keyboards->size(); ++i)
	{
		delete (*Keyboard::keyboards)[i];
		(*Keyboard::keyboards)[i] = nullptr;
	}
	delete Keyboard::keyboards;
}

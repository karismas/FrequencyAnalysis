#include <algorithm>
#include <iostream>
#include <array>
#include <string>

#include "keyboard.h"
#include "utils.h"

std::vector<Keyboard*>* Keyboard::keyboards = new std::vector<Keyboard*>();
std::vector<Keyboard*>* Keyboard::temp = new std::vector<Keyboard*>();

Keyboard::Keyboard(std::array<char, 24> inLayout)
{
	std::vector<char> alphabet = getAlphabetWithoutSH();
	alphabet.erase(std::remove_if(alphabet.begin(), alphabet.end(), [&inLayout](char c) 
	{
		bool isInLayout = false;
		for (int i = 0; i < inLayout.size(); ++i)
	    {
			if (inLayout[i] == c) { isInLayout = true; break;}
		}
		return isInLayout;
	}), alphabet.end());
	Randomizer::shuffleVector(&alphabet);

	std::vector<char>::iterator it = alphabet.begin();
	for (int i = 0; i < inLayout.size(); ++i)
	{
		if (inLayout[i] >= 97 && inLayout[i] < 123)
		{
			layout[i] = inLayout[i];
		}

		else
		{
			layout[i] = *it;
			++it;
		}
	}
}

double Keyboard::getFitnessScore(std::map<std::string, double>* letterFrequency, std::map<std::string, double>* doubleLetterFrequency)
{
	double lpu = 8 * getLeftPinkyUsage(letterFrequency);
	double lru = 4 * getLeftRingUsage(letterFrequency);
	double lmu = 1 * getLeftMiddleUsage(letterFrequency);
	double liu = 2 * getLeftIndexUsage(letterFrequency);

	double rpu = 8 * getRightPinkyUsage(letterFrequency);
	double rru = 4 * getRightRingUsage(letterFrequency);
	double rmu = 1 * getRightMiddleUsage(letterFrequency);
	double riu = 2 * getRightIndexUsage(letterFrequency);

	double lpv = 8 * getLeftPinkyVerticals(doubleLetterFrequency);
	double lrv = 4 * getLeftRingVerticals(doubleLetterFrequency);
	double lmv = 1 * getLeftMiddleVerticals(doubleLetterFrequency);
	double liv = 2 * getLeftIndexVerticals(doubleLetterFrequency);
             
	double rpv = 8 * getRightPinkyVerticals(doubleLetterFrequency);
	double rrv = 4 * getRightRingVerticals(doubleLetterFrequency);
	double rmv = 1 * getRightMiddleVerticals(doubleLetterFrequency);
	double riv = 2 * getRightIndexVerticals(doubleLetterFrequency);

	// 15 55 25 5

	// std::cout << "\n" << (lpu + lru + lmu + liu + rpu + rru + rmu + riu) << "\n";
	// std::cout << "\n" << 10 * (lpv + lrv + lmv + liv + rpv + rrv + rmv + riv) << "\n";
	return (10 * (lpv + lrv + lmv + liv + rpv + rrv + rmv + riv) + (lpu + lru + lmu + liu + rpu + rru + rmu + riu));
}

void Keyboard::populate(int numOrgansims)
{
	std::array<char, 24> constraints = {};

	constraints[8]  = 'd';
	constraints[9]  = 'n';
	constraints[10] = 'e';
	constraints[11] = 'a';
	constraints[12] = 'o';
	constraints[13] = 't';
	constraints[14] = 'i';
	constraints[15] = 'r';
	
	for (int i = 0; i < numOrgansims; ++i)
	{
		keyboards->emplace_back(new Keyboard(constraints));
	}
}

void Keyboard::breedGeneration(std::map<std::string, double>* letterFrequency, std::map<std::string, double>* doubleLetterFrequency)
{
	for (int i = 0; i < keyboards->size(); i += 2)
	{
		Keyboard* k1 = (*keyboards)[i];
		Keyboard* k2 = (*keyboards)[i + 1];
		breedOrganisms(k1, k2, letterFrequency, doubleLetterFrequency);
	}

	keyboards = temp;
	temp = new std::vector<Keyboard*>();
}

void Keyboard::breedOrganisms(Keyboard* k1, Keyboard* k2, std::map<std::string, double>* letterFrequency, std::map<std::string, double>* doubleLetterFrequency)
{
	int split = rand() % 24;

	std::vector<char> c1{};
	std::vector<char> c2{};

	for (int i = 0; i < split; ++i)
	{
		c1.emplace_back(k1->layout[i]);
		c2.emplace_back(k2->layout[i]);
	}

	for (int i = 0; i < c1.size(); ++i)
	{
		k1->swapCharacter(i, c2[i]);
		k2->swapCharacter(i, c1[i]);
	}

	temp->insert(
		std::upper_bound(temp->begin(), temp->end(), k1,
		[&](Keyboard* k1, Keyboard* other)
		{
			return k1->getFitnessScore(letterFrequency, doubleLetterFrequency) < other->getFitnessScore(letterFrequency, doubleLetterFrequency);
		}), k1
	);

	temp->insert(
		std::upper_bound(temp->begin(), temp->end(), k2,
		[&](Keyboard* k2, Keyboard* other)
		{
			return k2->getFitnessScore(letterFrequency, doubleLetterFrequency) < other->getFitnessScore(letterFrequency, doubleLetterFrequency);
		}), k2
	);
}

void Keyboard::swapCharacter(int position, char newChar)
{
	char oldChar = this->layout[position];

	int newCharCurrPosition = 0;
	for (int i = 0; i < 24; ++i)
	{
		if (layout[i] == newChar)
		{
			newCharCurrPosition = i;
			break;
		}
	}

	layout[position] = newChar;
	layout[newCharCurrPosition] = oldChar;
}

double Keyboard::getFingerUsage(std::map<std::string, double>* letterFrequency, int indices[])
{
	double usage = 0.f;

	std::string first = std::string{layout[indices[0]]};
	std::string second = std::string{layout[indices[2]]};

	usage += (*letterFrequency)[first];
	usage += (*letterFrequency)[second];

	return usage;
}

double Keyboard::getLeftPinkyUsage(std::map<std::string, double>* letterFrequency)
{
	return getFingerUsage(letterFrequency, leftPinkyIndices);
}

double Keyboard::getLeftRingUsage(std::map<std::string, double>* letterFrequency)
{
	return getFingerUsage(letterFrequency, leftRingIndices);
}

double Keyboard::getLeftMiddleUsage(std::map<std::string, double>* letterFrequency)
{
	return getFingerUsage(letterFrequency, leftMiddleIndices);
}

double Keyboard::getLeftIndexUsage(std::map<std::string, double>* letterFrequency)
{
	return getFingerUsage(letterFrequency, leftIndexIndices);
}

double Keyboard::getRightPinkyUsage(std::map<std::string, double>* letterFrequency)
{
	return getFingerUsage(letterFrequency, rightPinkyIndices);
}

double Keyboard::getRightRingUsage(std::map<std::string, double>* letterFrequency)
{
	return getFingerUsage(letterFrequency, rightRingIndices);
}

double Keyboard::getRightMiddleUsage(std::map<std::string, double>* letterFrequency)
{
	return getFingerUsage(letterFrequency, rightMiddleIndices);
}

double Keyboard::getRightIndexUsage(std::map<std::string, double>* letterFrequency)
{
	return getFingerUsage(letterFrequency, rightIndexIndices);
}

double Keyboard::getFingerVerticals(std::map<std::string, double>* doubleLetterFrequency, int indices[])
{
	double usage = 0.f;

	for (int i = 0; i < 3; ++i)
	{
		for (int j = 0; j < 3; ++j)
		{
			if (i == j) continue;

			std::string first = std::string{layout[indices[i]]};
			std::string second = std::string{layout[indices[j]]};
			usage += (*doubleLetterFrequency)[first + second];
		}
	}

	return usage;
}

double Keyboard::getLeftPinkyVerticals(std::map<std::string, double>* doubleLetterFrequency)
{
	return getFingerVerticals(doubleLetterFrequency, leftPinkyIndices);
}

double Keyboard::getLeftRingVerticals(std::map<std::string, double>* doubleLetterFrequency)
{
	return getFingerVerticals(doubleLetterFrequency, leftRingIndices);
}

double Keyboard::getLeftMiddleVerticals(std::map<std::string, double>* doubleLetterFrequency)
{
	return getFingerVerticals(doubleLetterFrequency, leftMiddleIndices);
}

double Keyboard::getLeftIndexVerticals(std::map<std::string, double>* doubleLetterFrequency)
{
	return getFingerVerticals(doubleLetterFrequency, leftIndexIndices);
}

double Keyboard::getRightPinkyVerticals(std::map<std::string, double>* doubleLetterFrequency)
{
	return getFingerVerticals(doubleLetterFrequency, rightPinkyIndices);
}

double Keyboard::getRightRingVerticals(std::map<std::string, double>* doubleLetterFrequency)
{
	return getFingerVerticals(doubleLetterFrequency, rightRingIndices);
}

double Keyboard::getRightMiddleVerticals(std::map<std::string, double>* doubleLetterFrequency)
{
	return getFingerVerticals(doubleLetterFrequency, rightMiddleIndices);
}

double Keyboard::getRightIndexVerticals(std::map<std::string, double>* doubleLetterFrequency)
{
	return getFingerVerticals(doubleLetterFrequency, rightIndexIndices);
}

void Keyboard::print()
{
	std::string out = "";
	for (int i = 0; i < (sizeof(layout) / sizeof(char)); ++i)
	{
		if (i % 4 == 0)
		{
			if (i % 8 == 0)
			{
				out += "\n";
			}
			
			else
			{
				out += " ";
			}
		}

		std::string charString{layout[i]};
		out += charString;
	}
	
	std::cout << out << "\n";
}

void Keyboard::printFingerUsage(std::map<std::string, double>* letterFrequency)
{
	double lpu = getLeftPinkyUsage(letterFrequency);
	double lru = getLeftRingUsage(letterFrequency);
	double lmu = getLeftMiddleUsage(letterFrequency);
	double liu = getLeftIndexUsage(letterFrequency);

	double rpu = getRightPinkyUsage(letterFrequency);
	double rru = getRightRingUsage(letterFrequency);
	double rmu = getRightMiddleUsage(letterFrequency);
	double riu = getRightIndexUsage(letterFrequency);

	double total = lpu + lru + lmu + liu + rpu + rru + rmu + riu;
	lpu *= (100 / total);
	lru *= (100 / total);
	lmu *= (100 / total);
	liu *= (100 / total);
	rpu *= (100 / total);
	rru *= (100 / total);
	rmu *= (100 / total);
	riu *= (100 / total);

	std::cout << "\nUSAGE\n";
	std::cout << lpu << "%  "
			  << lru << "%  "
			  << lmu << "%  "
			  << liu << "%    "
			  << riu << "%  "
			  << rmu << "%  "
			  << rru << "%  "
			  << rpu << "%\n\n";
}

void Keyboard::printFingerVerticals(std::map<std::string, double>* doubleLetterFrequency)
{
	double lpv = 8 * getLeftPinkyVerticals(doubleLetterFrequency);
	double lrv = 4 * getLeftRingVerticals(doubleLetterFrequency);
	double lmv = 1 * getLeftMiddleVerticals(doubleLetterFrequency);
	double liv = 2 * getLeftIndexVerticals(doubleLetterFrequency);
             
	double rpv = 8 * getRightPinkyVerticals(doubleLetterFrequency);
	double rrv = 4 * getRightRingVerticals(doubleLetterFrequency);
	double rmv = 1 * getRightMiddleVerticals(doubleLetterFrequency);
	double riv = 2 * getRightIndexVerticals(doubleLetterFrequency);

	double total = lpv + lrv + lmv + liv + rpv + rrv + rmv + riv;
	lpv *= (100 / total);
	lrv *= (100 / total);
	lmv *= (100 / total);
	liv *= (100 / total);
	rpv *= (100 / total);
	rrv *= (100 / total);
	rmv *= (100 / total);
	riv *= (100 / total);

	std::cout << "\nVERTICALS\n";
	std::cout << lpv << "%  "
			  << lrv << "%  "
			  << lmv << "%  "
			  << liv << "%    "
			  << riv << "%  "
			  << rmv << "%  "
			  << rrv << "%  "
			  << rpv << "%\n\n";
}

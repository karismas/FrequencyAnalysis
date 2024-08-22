#include <algorithm>
#include <iostream>
#include <array>
#include <sstream>
#include <string>

#include "keyboard.h"
#include "utils.h"

std::vector<Keyboard*>* Keyboard::keyboards = new std::vector<Keyboard*>();
std::vector<Keyboard*>* Keyboard::temp = new std::vector<Keyboard*>();

std::map<char, int> Keyboard::ltp{};
std::map<char, int> Keyboard::ltr{};
std::map<char, int> Keyboard::ltm{};
std::map<char, int> Keyboard::lti{};
std::map<char, int> Keyboard::lmp{};
std::map<char, int> Keyboard::lmr{};
std::map<char, int> Keyboard::lmm{};
std::map<char, int> Keyboard::lmi{};
std::map<char, int> Keyboard::lbp{};
std::map<char, int> Keyboard::lbr{};
std::map<char, int> Keyboard::lbm{};
std::map<char, int> Keyboard::lbi{};

std::map<char, int> Keyboard::rtp{};
std::map<char, int> Keyboard::rtr{};
std::map<char, int> Keyboard::rtm{};
std::map<char, int> Keyboard::rti{};
std::map<char, int> Keyboard::rmp{};
std::map<char, int> Keyboard::rmr{};
std::map<char, int> Keyboard::rmm{};
std::map<char, int> Keyboard::rmi{};
std::map<char, int> Keyboard::rbp{};
std::map<char, int> Keyboard::rbr{};
std::map<char, int> Keyboard::rbm{};
std::map<char, int> Keyboard::rbi{};

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

void Keyboard::trackStatistics()
{
	std::array<char, 24> best = (*Keyboard::keyboards)[0]->layout;

	ltp[best[0]] += 1;
	ltr[best[1]] += 1;
	ltm[best[2]] += 1;
	lti[best[3]] += 1;

	rti[best[4]] += 1;
	rtm[best[5]] += 1;
	rtr[best[6]] += 1;
	rtp[best[7]] += 1;

	lmp[best[8]] += 1;
	lmr[best[9]] += 1;
	lmm[best[10]] += 1;
	lmi[best[11]] += 1;

	rmi[best[12]] += 1;
	rmm[best[13]] += 1;
	rmr[best[14]] += 1;
	rmp[best[15]] += 1;

	lbp[best[16]] += 1;
	lbr[best[17]] += 1;
	lbm[best[18]] += 1;
	lbi[best[19]] += 1;

	rbi[best[20]] += 1;
	rbm[best[21]] += 1;
	rbr[best[22]] += 1;
	rbp[best[23]] += 1;
}

double Keyboard::getFitnessScore(std::map<std::string, double>* letterFrequency, std::map<std::string, double>* doubleLetterFrequency)
{
	double lpu = getLeftPinkyUsage(letterFrequency);
	double lru = getLeftRingUsage(letterFrequency);
	double lmu = getLeftMiddleUsage(letterFrequency);
	double liu = getLeftIndexUsage(letterFrequency);

	double rpu = getRightPinkyUsage(letterFrequency);
	double rru = getRightRingUsage(letterFrequency);
	double rmu = getRightMiddleUsage(letterFrequency);
	double riu = getRightIndexUsage(letterFrequency);

	double lpv = getLeftPinkyVerticals(doubleLetterFrequency);
	double lrv = getLeftRingVerticals(doubleLetterFrequency);
	double lmv = getLeftMiddleVerticals(doubleLetterFrequency);
	double liv = getLeftIndexVerticals(doubleLetterFrequency);
             
	double rpv = getRightPinkyVerticals(doubleLetterFrequency);
	double rrv = getRightRingVerticals(doubleLetterFrequency);
	double rmv = getRightMiddleVerticals(doubleLetterFrequency);
	double riv = getRightIndexVerticals(doubleLetterFrequency);

	std::map<std::string, double> leftFingerUsage{};
	leftFingerUsage["lpu"] = lpu;
	leftFingerUsage["lru"] = lru;
	leftFingerUsage["lmu"] = lmu;
	leftFingerUsage["liu"] = liu;

	std::map<std::string, double> rightFingerUsage{};
	rightFingerUsage["rpu"] = rpu;
	rightFingerUsage["rru"] = rru;
	rightFingerUsage["rmu"] = rmu;
	rightFingerUsage["riu"] = riu;

	std::map<std::string, double> leftFingerVerticals{};
	leftFingerVerticals["lpv"] = lpv;
	leftFingerVerticals["lrv"] = lrv;
	leftFingerVerticals["lmv"] = lmv;
	leftFingerVerticals["liv"] = liv;

	std::map<std::string, double> rightFingerVerticals{};
	rightFingerVerticals["rpv"] = rpv;
	rightFingerVerticals["rrv"] = rrv;
	rightFingerVerticals["rmv"] = rmv;
	rightFingerVerticals["riv"] = riv;

	leftFingerUsage = normalizeMap(leftFingerUsage);
	rightFingerUsage = normalizeMap(rightFingerUsage);
	leftFingerVerticals = normalizeMap(leftFingerVerticals);
	rightFingerVerticals = normalizeMap(rightFingerVerticals);

	double lpuScore = abs(leftFingerUsage["lpu"] - 0.05);
	double lruScore = abs(leftFingerUsage["lru"] - 0.25);
	double lmuScore = abs(leftFingerUsage["lmu"] - 0.55);
	double liuScore = abs(leftFingerUsage["liu"] - 0.15);

	double rpuScore = abs(rightFingerUsage["rpu"] - 0.05);
	double rruScore = abs(rightFingerUsage["rru"] - 0.25);
	double rmuScore = abs(rightFingerUsage["rmu"] - 0.55);
	double riuScore = abs(rightFingerUsage["riu"] - 0.15);

	double lpvScore = abs(leftFingerVerticals["lpv"] - 0.05);
	double lrvScore = abs(leftFingerVerticals["lrv"] - 0.25);
	double lmvScore = abs(leftFingerVerticals["lmv"] - 0.55);
	double livScore = abs(leftFingerVerticals["liv"] - 0.15);
                                             
	double rpvScore = abs(rightFingerVerticals["rpv"] - 0.05);
	double rrvScore = abs(rightFingerVerticals["rrv"] - 0.25);
	double rmvScore = abs(rightFingerVerticals["rmv"] - 0.55);
	double rivScore = abs(rightFingerVerticals["riv"] - 0.15);

	double usageScore = lpuScore + lruScore + lmuScore + liuScore + rpuScore + rruScore + rmuScore + riuScore;
	double verticalsScore = lpvScore + lrvScore + lmvScore + livScore + rpvScore + rrvScore + rmvScore + rivScore;

	// 15 55 25 5

	return usageScore + (2 * verticalsScore);
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

	trackStatistics();
}

void Keyboard::breedOrganisms(Keyboard* k1, Keyboard* k2, std::map<std::string, double>* letterFrequency, std::map<std::string, double>* doubleLetterFrequency)
{
	int split = rand() % 24;

	std::vector<char> c1{};
	std::vector<char> c2{};

	int choice = rand() % 2;
	int start = (choice == 0) ? 0 : split;
	int end = (choice == 0) ? split : 24;
	for (int i = start; i < end; ++i)
	{
		c1.emplace_back(k1->layout[i]);
		c2.emplace_back(k2->layout[i]);
	}

	for (int i = start; i < end; ++i)
	{
		k1->swapCharacter(i, c2[i - start]);
		k2->swapCharacter(i, c1[i - start]);
	}

	temp->insert(
		std::upper_bound(temp->begin(), temp->end(), k1,
		[&](Keyboard* first, Keyboard* second)
		{
			return first->getFitnessScore(letterFrequency, doubleLetterFrequency) < second->getFitnessScore(letterFrequency, doubleLetterFrequency);
		}), k1
	);

	temp->insert(
		std::upper_bound(temp->begin(), temp->end(), k2,
		[&](Keyboard* first, Keyboard* second)
		{
			return first->getFitnessScore(letterFrequency, doubleLetterFrequency) < second->getFitnessScore(letterFrequency, doubleLetterFrequency);
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

void Keyboard::printLayout()
{
	std::string out = "                                    LAYOUT";
	for (int i = 0; i < (sizeof(layout) / sizeof(char)); ++i)
	{
		if (i % 4 == 0)
		{
			if (i % 8 == 0)
			{
				out += "\n                                   ";
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

	double leftTotal = lpu + lru + lmu + liu;
	double rightTotal = rpu + rru + rmu + riu;
	lpu *= (100 / leftTotal);
	lru *= (100 / leftTotal);
	lmu *= (100 / leftTotal);
	liu *= (100 / leftTotal);
	rpu *= (100 / rightTotal);
	rru *= (100 / rightTotal);
	rmu *= (100 / rightTotal);
	riu *= (100 / rightTotal);

	std::cout << "\n                                     USAGE\n";
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
	double lpv = getLeftPinkyVerticals(doubleLetterFrequency);
	double lrv = getLeftRingVerticals(doubleLetterFrequency);
	double lmv = getLeftMiddleVerticals(doubleLetterFrequency);
	double liv = getLeftIndexVerticals(doubleLetterFrequency);
             
	double rpv = getRightPinkyVerticals(doubleLetterFrequency);
	double rrv = getRightRingVerticals(doubleLetterFrequency);
	double rmv = getRightMiddleVerticals(doubleLetterFrequency);
	double riv = getRightIndexVerticals(doubleLetterFrequency);

	double leftTotal = lpv + lrv + lmv + liv;
	double rightTotal = rpv + rrv + rmv + riv;
	lpv *= (100 / leftTotal);
	lrv *= (100 / leftTotal);
	lmv *= (100 / leftTotal);
	liv *= (100 / leftTotal);
	rpv *= (100 / rightTotal);
	rrv *= (100 / rightTotal);
	rmv *= (100 / rightTotal);
	riv *= (100 / rightTotal);

	std::cout << "\n                                   VERTICALS\n";
	std::cout << lpv << "%  "
			  << lrv << "%  "
			  << lmv << "%  "
			  << liv << "%    "
			  << riv << "%  "
			  << rmv << "%  "
			  << rrv << "%  "
			  << rpv << "%\n\n";
}

std::vector<std::map<char, double>> Keyboard::getTopThree(std::map<char, double> map)
{
	std::vector<std::map<char, double>> inOrder{};
	std::vector<std::map<char, double>> out{};

	std::map<char, double>::iterator it = map.begin();
	while (it != map.end())
	{
		std::map<char, double> temp{};
		temp[it->first] = it->second;

		inOrder.insert(
			std::upper_bound(inOrder.begin(), inOrder.end(), temp,
			[](std::map<char, double> first, std::map<char, double> second)
			{
				std::map<char, double>::iterator firstIt = first.begin();
				std::map<char, double>::iterator secondIt = second.begin();
				return firstIt->second > secondIt->second;
			}), temp
		);

		++it;
	}
	
	int threeOrLess = (inOrder.size() < 3) ? inOrder.size() : 3;
	for (int i = 0; i < threeOrLess; ++i)
	{
		out.emplace_back(inOrder[i]);
	}

	for (int i = out.size(); i < 3; ++i)
	{
		std::map<char, double> temp{};
		temp['_'] = 0.f;
		out.emplace_back(temp);
	}

	return out;
}

void Keyboard::printStatistics()
{
	std::vector<std::map<char, double>> nltpT3 = getTopThree(normalizeMap(ltp));
	std::vector<std::map<char, double>> nltrT3 = getTopThree(normalizeMap(ltr));
	std::vector<std::map<char, double>> nltmT3 = getTopThree(normalizeMap(ltm));
	std::vector<std::map<char, double>> nltiT3 = getTopThree(normalizeMap(lti));
	std::vector<std::map<char, double>> nlmpT3 = getTopThree(normalizeMap(lmp));
	std::vector<std::map<char, double>> nlmrT3 = getTopThree(normalizeMap(lmr));
	std::vector<std::map<char, double>> nlmmT3 = getTopThree(normalizeMap(lmm));
	std::vector<std::map<char, double>> nlmiT3 = getTopThree(normalizeMap(lmi));
	std::vector<std::map<char, double>> nlbpT3 = getTopThree(normalizeMap(lbp));
	std::vector<std::map<char, double>> nlbrT3 = getTopThree(normalizeMap(lbr));
	std::vector<std::map<char, double>> nlbmT3 = getTopThree(normalizeMap(lbm));
	std::vector<std::map<char, double>> nlbiT3 = getTopThree(normalizeMap(lbi));

	std::vector<std::map<char, double>> nrtpT3 = getTopThree(normalizeMap(rtp));
	std::vector<std::map<char, double>> nrtrT3 = getTopThree(normalizeMap(rtr));
	std::vector<std::map<char, double>> nrtmT3 = getTopThree(normalizeMap(rtm));
	std::vector<std::map<char, double>> nrtiT3 = getTopThree(normalizeMap(rti));
	std::vector<std::map<char, double>> nrmpT3 = getTopThree(normalizeMap(rmp));
	std::vector<std::map<char, double>> nrmrT3 = getTopThree(normalizeMap(rmr));
	std::vector<std::map<char, double>> nrmmT3 = getTopThree(normalizeMap(rmm));
	std::vector<std::map<char, double>> nrmiT3 = getTopThree(normalizeMap(rmi));
	std::vector<std::map<char, double>> nrbpT3 = getTopThree(normalizeMap(rbp));
	std::vector<std::map<char, double>> nrbrT3 = getTopThree(normalizeMap(rbr));
	std::vector<std::map<char, double>> nrbmT3 = getTopThree(normalizeMap(rbm));
	std::vector<std::map<char, double>> nrbiT3 = getTopThree(normalizeMap(rbi));

	std::ostringstream out;
	out << "\n                                        STATISTICS\n";
	out << "  TLP        TLR        TLM        TLI             TRI        TRM        TRR        TRP\n";
	for (int i = 0; i < 3; ++i)
	{
		std::map<char, double>::iterator lpIt = nltpT3[i].begin();
		std::map<char, double>::iterator lrIt = nltrT3[i].begin();
		std::map<char, double>::iterator lmIt = nltmT3[i].begin();
		std::map<char, double>::iterator liIt = nltiT3[i].begin();

		std::map<char, double>::iterator rpIt = nrtpT3[i].begin();
		std::map<char, double>::iterator rrIt = nrtrT3[i].begin();
		std::map<char, double>::iterator rmIt = nrtmT3[i].begin();
		std::map<char, double>::iterator riIt = nrtiT3[i].begin();

		out << lpIt->first << ": " << (lpIt->second * 100) << "%    ";
		out << lrIt->first << ": " << (lrIt->second * 100) << "%    ";
		out << lmIt->first << ": " << (lmIt->second * 100) << "%    ";
		out << liIt->first << ": " << (liIt->second * 100) << "%         ";

		out << riIt->first << ": " << (riIt->second * 100) << "%    ";
		out << rmIt->first << ": " << (rmIt->second * 100) << "%    ";
		out << rrIt->first << ": " << (rrIt->second * 100) << "%    ";
		out << rpIt->first << ": " << (rpIt->second * 100) << "%\n";
	}

	out << "\n  MLP        MLR        MLM        MLI             MRI        MRM        MRR        MRP\n";
	for (int i = 0; i < 3; ++i)
	{
		std::map<char, double>::iterator lpIt = nlmpT3[i].begin();
		std::map<char, double>::iterator lrIt = nlmrT3[i].begin();
		std::map<char, double>::iterator lmIt = nlmmT3[i].begin();
		std::map<char, double>::iterator liIt = nlmiT3[i].begin();

		std::map<char, double>::iterator rpIt = nrmpT3[i].begin();
		std::map<char, double>::iterator rrIt = nrmrT3[i].begin();
		std::map<char, double>::iterator rmIt = nrmmT3[i].begin();
		std::map<char, double>::iterator riIt = nrmiT3[i].begin();

		out << lpIt->first << ": " << (lpIt->second * 100) << "%    ";
		out << lrIt->first << ": " << (lrIt->second * 100) << "%    ";
		out << lmIt->first << ": " << (lmIt->second * 100) << "%    ";
		out << liIt->first << ": " << (liIt->second * 100) << "%         ";

		out << riIt->first << ": " << (riIt->second * 100) << "%    ";
		out << rmIt->first << ": " << (rmIt->second * 100) << "%    ";
		out << rrIt->first << ": " << (rrIt->second * 100) << "%    ";
		out << rpIt->first << ": " << (rpIt->second * 100) << "%\n";
	}

	out << "\n  BLP        BLR        BLM        BLI             BRI        BRM        BRR        BRP\n";
	for (int i = 0; i < 3; ++i)
	{
		std::map<char, double>::iterator lpIt = nlbpT3[i].begin();
		std::map<char, double>::iterator lrIt = nlbrT3[i].begin();
		std::map<char, double>::iterator lmIt = nlbmT3[i].begin();
		std::map<char, double>::iterator liIt = nlbiT3[i].begin();

		std::map<char, double>::iterator rpIt = nrbpT3[i].begin();
		std::map<char, double>::iterator rrIt = nrbrT3[i].begin();
		std::map<char, double>::iterator rmIt = nrbmT3[i].begin();
		std::map<char, double>::iterator riIt = nrbiT3[i].begin();

		out << lpIt->first << ": " << (lpIt->second * 100) << "%    ";
		out << lrIt->first << ": " << (lrIt->second * 100) << "%    ";
		out << lmIt->first << ": " << (lmIt->second * 100) << "%    ";
		out << liIt->first << ": " << (liIt->second * 100) << "%         ";

		out << riIt->first << ": " << (riIt->second * 100) << "%    ";
		out << rmIt->first << ": " << (rmIt->second * 100) << "%    ";
		out << rrIt->first << ": " << (rrIt->second * 100) << "%    ";
		out << rpIt->first << ": " << (rpIt->second * 100) << "%\n";
	}

	std::cout << out.str() << "\n";
}

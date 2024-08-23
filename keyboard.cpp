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


	std::map<std::string, double> totalFingerUsage{};
	totalFingerUsage["lpu"] = lpu;
	totalFingerUsage["lru"] = lru;
	totalFingerUsage["lmu"] = lmu;
	totalFingerUsage["liu"] = liu;
	totalFingerUsage["rpu"] = rpu;
	totalFingerUsage["rru"] = rru;
	totalFingerUsage["rmu"] = rmu;
	totalFingerUsage["riu"] = riu;

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

	totalFingerUsage = normalizeMap(totalFingerUsage);
	leftFingerUsage = normalizeMap(leftFingerUsage);
	rightFingerUsage = normalizeMap(rightFingerUsage);
	leftFingerVerticals = normalizeMap(leftFingerVerticals);
	rightFingerVerticals = normalizeMap(rightFingerVerticals);

	double leftScore = abs((totalFingerUsage["lpu"] + totalFingerUsage["lru"] + totalFingerUsage["lmu"] + totalFingerUsage["liu"]) - 0.5);
	double rightScore = abs((totalFingerUsage["rpu"] + totalFingerUsage["rru"] + totalFingerUsage["rmu"] + totalFingerUsage["riu"]) - 0.5);

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

	double splitScore = leftScore + rightScore;
	double usageScore = lpuScore + lruScore + lmuScore + liuScore + rpuScore + rruScore + rmuScore + riuScore;
	double verticalsScore = lpvScore + lrvScore + lmvScore + livScore + rpvScore + rrvScore + rmvScore + rivScore;

	return splitScore + usageScore + verticalsScore;
}

void Keyboard::populate(int numOrgansims)
{
	std::array<char, 24> constraints = {};

	constraints = { 'z', 'g', 'l', 'w', 'y', 'u', 'f', 'q', 'd', 'n', 'e', 'a', 'o', 't', 'i', 'r', 'k', 'b', 'm', 'x', 'v', 'c', 'p', 'j' };
	// constraints = { 'z', 'g', 'l', 'w', 'y', 'u', 'f', 'q', 'd', 'n', 'e', 'a', 'o', 't', 'i', 'r', 'p', 'b', 'm', 'k', 'v', 'c', 'x', 'j' };
	//  z, k      g, b        l, m        w, x           y, v       u, c       f, p         q, j

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
	float slpu = lpu * (100 / leftTotal);
	float slru = lru * (100 / leftTotal);
	float slmu = lmu * (100 / leftTotal);
	float sliu = liu * (100 / leftTotal);

	float srpu = rpu * (100 / rightTotal);
	float srru = rru * (100 / rightTotal);
	float srmu = rmu * (100 / rightTotal);
	float sriu = riu * (100 / rightTotal);

	float total = leftTotal + rightTotal;
	float tlpu = lpu * (100 / total);
	float tlru = lru * (100 / total);
	float tlmu = lmu * (100 / total);
	float tliu = liu * (100 / total);
          
	float trpu = rpu * (100 / total);
	float trru = rru * (100 / total);
	float trmu = rmu * (100 / total);
	float triu = riu * (100 / total);

	std::cout << "\n                                     USAGE\n";
	std::cout << tlpu << "%  "
			  << tlru << "%  "
			  << tlmu << "%  "
			  << tliu << "%    "
			  << triu << "%  "
			  << trmu << "%  "
			  << trru << "%  "
			  << trpu << "%\n\n";

	std::cout << "\n                                SEPARATE USAGE\n";
	std::cout << slpu << "%  "
			  << slru << "%  "
			  << slmu << "%  "
			  << sliu << "%    "
			  << sriu << "%  "
			  << srmu << "%  "
			  << srru << "%  "
			  << srpu << "%\n\n";
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


// *********************************** MOST FIT ***********************************
// ================================================================================
//
//                                     LAYOUT
//                                    zglw yufq
//                                    dnea otir
//                                    kbmx vcpj
// 
//                                      SCORE
//                                     0.578454
// 
//                                      USAGE
// 2.87932%  13.3271%  25.527%  7.94326%    10.3978%  22.2411%  16.6483%  1.03619%
// 
// 
//                                 SEPARATE USAGE
// 5.79611%  26.8277%  51.3863%  15.9899%    20.6619%  44.1963%  33.0827%  2.05906%
// 
// 
//                                    VERTICALS
// 0.039778%  25.3634%  60.2351%  14.3617%    18.3554%  54.2241%  27.2871%  0.133388%
// 
// ================================================================================
// ********************************************************************************


// 10000 Organisms, 100 Generations
//                                         STATISTICS
//   TLP        TLR        TLM        TLI             TRI        TRM        TRR        TRP
// k: 15%     g: 54%     l: 31%      w: 16%         y: 15%     u: 49%     f: 25%     j: 26%
// z: 13%     b: 10%     m: 30%      x: 14%         j: 14%     c: 31%     w: 15%     q: 17%
// q: 12%     p: 10%     c: 15%      y: 12%         v: 12%     l: 16%     b: 12%     z: 11%
// 
//   BLP        BLR        BLM        BLI             BRI        BRM        BRR        BRP
// z: 17%     g: 34%     m: 33%      p: 13%         y: 17%     u: 39%     f: 25%     q: 23%
// x: 16%     w: 13%     l: 26%      z: 13%         b: 14%     c: 33%     p: 20%     k: 18%
// k: 14%     b: 8%      c: 18%      j: 12%         v: 13%     l: 26%     b: 13%     z: 16%
//
//  z, k      g, b?        l, m        w?, x?           y, v       u, c       f, p?         q, j



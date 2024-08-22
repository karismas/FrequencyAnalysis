#include <iostream>
#include <vector>
#include <random>
#include <map>
#include <algorithm>
#include <sstream>

std::vector<char> getAlphabet();
std::vector<char> getAlphabetWithoutSH();
std::map<std::string, double> normalizeDict(std::map<std::string, double> dict);

template <typename T>
void printVector(std::vector<T>* vec)
{
	std::ostringstream out;
	out << "{ ";

	for (int i = 0; i < (*vec).size(); ++i)
	{
		out << (*vec)[i];

		if (i != (*vec).size() - 1)
			out << ", ";
	}
	out << " }";
	std::cout << out.str() << "\n";
}

template <typename A, typename B>
void printMap(std::map<A, B>* map)
{
	std::ostringstream out;
	out << "{ ";

	auto it = map->begin();
	while (it != map->end())
	{
		out << it->first << ": " << it->second;
		if (++it != map->end())
			out << ", ";
	}
	out << " }";
	std::cout << out.str() << "\n";
}

class Randomizer
{
	private:
	static inline std::random_device rd = std::random_device{};
	static inline std::default_random_engine rng = std::default_random_engine{ rd() };

	public:
	template <typename T>
	static void shuffleVector(std::vector<T>* vec)
	{
		std::shuffle(std::begin(*vec), std::end(*vec), rng);
	}
};

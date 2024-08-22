#include <map>
#include <string>
#include <array>

class Keyboard
{
	private:

	std::array<char, 24> layout;

	int leftPinkyIndices[3] = { 0, 8, 16 };
	int leftRingIndices[3] = { 1, 9, 17 };
	int leftMiddleIndices[3] = { 2, 10, 18 };
	int leftIndexIndices[3] = { 3, 11, 19 };
	int rightPinkyIndices[3] = { 7, 15, 23 };
	int rightRingIndices[3] = { 6, 14, 22 };
	int rightMiddleIndices[3] = { 5, 13, 21 };
	int rightIndexIndices[3] = { 4, 12, 20 };

	double getFingerUsage(std::map<std::string, double>* letterFrequency, int indices[]);
	double getFingerVerticals(std::map<std::string, double>* letterFrequency, int indices[]);

	public:
	void swapCharacter(int position, char newChar);


	static std::vector<Keyboard*>* keyboards;
	static std::vector<Keyboard*>* temp;

	Keyboard(std::array<char, 24> inLayout);
	// ~Keyboard();
	
	static void populate(int numOrgansims);
	static void breedGeneration(std::map<std::string, double>* letterFrequency, std::map<std::string, double>* doubleLetterFrequency);
	static void breedOrganisms(Keyboard* k1, Keyboard* k2, std::map<std::string, double>* letterFrequency, std::map<std::string, double>* doubleLetterFrequency);

	double getFitnessScore(std::map<std::string, double>* letterFrequency, std::map<std::string, double>* doubleLetterFrequency);

	double getLeftPinkyUsage(std::map<std::string, double>* letterFrequency);
	double getLeftRingUsage(std::map<std::string, double>* letterFrequency);
	double getLeftMiddleUsage(std::map<std::string, double>* letterFrequency);
	double getLeftIndexUsage(std::map<std::string, double>* letterFrequency);
	double getRightPinkyUsage(std::map<std::string, double>* letterFrequency);
	double getRightRingUsage(std::map<std::string, double>* letterFrequency);
	double getRightMiddleUsage(std::map<std::string, double>* letterFrequency);
	double getRightIndexUsage(std::map<std::string, double>* letterFrequency);

	double getLeftPinkyVerticals(std::map<std::string, double>* doubleLetterFrequency);
	double getLeftRingVerticals(std::map<std::string, double>* doubleLetterFrequency);
	double getLeftMiddleVerticals(std::map<std::string, double>* doubleLetterFrequency);
	double getLeftIndexVerticals(std::map<std::string, double>* doubleLetterFrequency);
	double getRightPinkyVerticals(std::map<std::string, double>* doubleLetterFrequency);
	double getRightRingVerticals(std::map<std::string, double>* doubleLetterFrequency);
	double getRightMiddleVerticals(std::map<std::string, double>* doubleLetterFrequency);
	double getRightIndexVerticals(std::map<std::string, double>* doubleLetterFrequency);

	void print();
	void printFingerUsage(std::map<std::string, double>* letterFrequency);
	void printFingerVerticals(std::map<std::string, double>* doubleLetterFrequency);
};

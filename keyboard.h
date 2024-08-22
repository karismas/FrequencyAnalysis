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

	void swapCharacter(int position, char newChar);

	static void trackStatistics();
	static std::vector<std::map<char, double>> getTopThree(std::map<char, double> map);
	static std::map<char, int> ltp;
	static std::map<char, int> ltr;
	static std::map<char, int> ltm;
	static std::map<char, int> lti;
	static std::map<char, int> lmp;
	static std::map<char, int> lmr;
	static std::map<char, int> lmm;
	static std::map<char, int> lmi;
	static std::map<char, int> lbp;
	static std::map<char, int> lbr;
	static std::map<char, int> lbm;
	static std::map<char, int> lbi;

	static std::map<char, int> rtp;
	static std::map<char, int> rtr;
	static std::map<char, int> rtm;
	static std::map<char, int> rti;
	static std::map<char, int> rmp;
	static std::map<char, int> rmr;
	static std::map<char, int> rmm;
	static std::map<char, int> rmi;
	static std::map<char, int> rbp;
	static std::map<char, int> rbr;
	static std::map<char, int> rbm;
	static std::map<char, int> rbi;

	public:

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

	void printLayout();
	void printFingerUsage(std::map<std::string, double>* letterFrequency);
	void printFingerVerticals(std::map<std::string, double>* doubleLetterFrequency);
	static void printStatistics();
};

#include <map>
#include <string>

std::map<std::string, int> getWordCount(std::string filename);
std::map<std::string, double> getWordFrequency(std::map<std::string, int> wordCount);
std::map<std::string, double> getLetterFrequency(std::map<std::string, double> wordFrequency);
std::map<std::string, double> getDoubleLetterFrequency(std::map<std::string, double> wordFrequency);
std::map<std::string, double> normalizeDict(std::map<std::string, double> dict);
void printDict(std::map<std::string, double> dict);

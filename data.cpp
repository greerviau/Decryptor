#include "data.h"

using namespace std;

Data::Data() {
    attackMode = -1;
    hashType = -1;
    wordIndex = -1;
    testIndex = 0;
}

void Data::setAttackMode(int a) { attackMode = a; }
int Data::getAttackMode() { return attackMode; }

void Data::setHashType(int h) { hashType = h; }
int Data::getHashType() { return hashType; }

void Data::addHash(string hash) { hashes.insert(hash); }
void Data::addHashes(list<string> hash) {
    for(auto const& h: hash)
        hashes.insert(h);
    }
bool Data::containsHash(string hash) {
    if(hashes.find(hash) != hashes.end())
        return true;
    return false;
}
int Data::getHashSize() { return hashes.size(); }

void Data::addWord(string word) { wordlist.push_front(word); }
void Data::addWords(list<string> words) {
    for(auto const& w: words)
        wordlist.push_front(w);
}
string Data::getWord(int i) { 
    std::list<std::string>::iterator it = wordlist.begin();
    std::advance(it, i);
    return *it;
}
list<string> Data::getWordList() { return wordlist; }
int Data::getWordSize() { return wordlist.size(); }

void Data::addCrack(string crack) { cracked.push_front(crack); }
void Data::addCracks(list<string> cracks) {
    for(auto const& c: cracks)
        cracked.push_front(c);
}
string Data::getCrack(int i) {
    std::list<std::string>::iterator it = cracked.begin();
    std::advance(it, i);
    return *it;
}
list<string> Data::getCracked() { return cracked; }
int Data::getCrackSize() { return cracked.size(); }

int Data::getWordIndex() { return wordIndex; }
int Data::getTestIndex() { return testIndex; }

string Data::nextWord() {
    wordIndex++;
    std::list<std::string>::iterator it = wordlist.begin();
    std::advance(it, wordIndex);
    return *it;
}

bool Data::isNextWord() { return (getWordIndex() <= getWordSize()-2); }

string Data::nextTest() {
    if(test[testIndex] == 0) {
        test[testIndex] = 47;
    }
    test[testIndex]++;
    if(test[testIndex] == 58)
        test[testIndex] = 65;
    else if(test[testIndex] == 91)
        test[testIndex] = 97;
    testIndex = 0;
    while(test[testIndex] > 122) {
        test[testIndex] = 48;
        testIndex++;
        if(testIndex >= (sizeof(test)/sizeof(int)))
            break;
    }
    string testString = "";
    for (int i = 0; i < (sizeof(test)/sizeof(int)); i++) {
        if(test[i] != 0)
            testString += test[i];
    }
    return testString;
}

bool Data::isNextTest() { return getTestIndex() < (sizeof(test)/sizeof(char)); }


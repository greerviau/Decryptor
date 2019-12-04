#include "data.h"

using namespace std;

Data::Data() {
    attackMode = -1;
    hashType = -1;
    dictIndex = 0;
    testIndex = 0;
}

Data::~Data() {
    pthread_mutex_destroy(&lock);
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

void Data::addWord(string word) { dictionary.push_back(word); }
void Data::addWords(list<string> words) {
    for(auto const& w: words)
        dictionary.push_back(w);
}
string Data::getWord(int i) { 
    string word = dictionary[i];
    return word;
}
vector<string> Data::getWordList() { return dictionary; }
int Data::getWordSize() { return dictionary.size(); }

void Data::addCrack(string crack) { 
    pthread_mutex_lock(&lock);
    cracked.push_back(crack); 
    cout << crack << endl;
    pthread_mutex_unlock(&lock);
}
void Data::addCracks(list<string> cracks) {
    pthread_mutex_lock(&lock);
    for(auto const& c: cracks)
        cracked.push_back(c);
    pthread_mutex_unlock(&lock);
}
string Data::getCrack(int i) {
    std::list<string>::iterator it = cracked.begin();
    std::advance(it,i);
    return *it;
}
list<string> Data::getCracked() { return cracked; }
int Data::getCrackSize() { return cracked.size(); }

int Data::getDictIndex() { return dictIndex; }
int Data::getTestIndex() { return testIndex; }

string Data::nextWord() {
    pthread_mutex_lock(&lock);
    string word = dictionary[dictIndex];
    dictIndex++;
    pthread_mutex_unlock(&lock); 
    return word;
}

bool Data::isNextWord() { return (getDictIndex() < getWordSize()); }

string Data::nextTest() {
    pthread_mutex_lock(&lock);
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
    pthread_mutex_unlock(&lock);
    return testString;
}

bool Data::isNextTest() { return getTestIndex() < (sizeof(test)/sizeof(int)); }


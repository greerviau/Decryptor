#include "data.h"

using namespace std;

Data::Data() {
    attack_mode = -1;
    hash_type = -1;
    dict_index = 0;
    test_index = 0;
}

Data::~Data() {
    pthread_mutex_destroy(&lock);
}

void Data::setAttackMode(int a) { attack_mode = a; }
int Data::getAttackMode() { return attack_mode; }

void Data::setHashType(int h) { hash_type = h; }
int Data::getHashType() { return hash_type; }

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
vector<string> Data::getDictionary() { return dictionary; }
int Data::getDictSize() { return dictionary.size(); }

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

int Data::getDictIndex() { return dict_index; }
int Data::getTestIndex() { return test_index; }

string Data::nextWord() {
    pthread_mutex_lock(&lock);
    string word = dictionary[dict_index];
    dict_index++;
    pthread_mutex_unlock(&lock); 
    return word;
}

bool Data::isNextWord() { return (getDictIndex() < getDictSize()); }

string Data::nextTest() {
    pthread_mutex_lock(&lock);
    if(test[test_index] == 0) {
        test[test_index] = 47;
    }
    test[test_index]++;
    if(test[test_index] == 58)
        test[test_index] = 65;
    else if(test[test_index] == 91)
        test[test_index] = 97;
    test_index = 0;
    while(test[test_index] > 122) {
        test[test_index] = 48;
        test_index++;
        if(test_index >= (sizeof(test)/sizeof(int)))
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


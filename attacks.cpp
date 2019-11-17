#include "attacks.h"

using namespace std;

Attacks::Attacks(Data* data) {
    dataP = data;
}

void Attacks::run() {
    switch(dataP->getAttackMode()) {
        case 0:
            wordListAttack();
            break;
        case 1:
            bruteForce();
            break;
        default:
            cout << "--< Invalid Attack Mode >--" << endl;
            system("exit");
    }
}

void Attacks::wordListAttack() {
    try {
        while(dataP->isNextWord()) {
            string word = dataP->nextWord();
            string hashcheck = getHash(word);
            if(dataP->containsHash(hashcheck)) {
                dataP->addCrack(hashcheck+":"+word);
                cout << hashcheck << ":" << word << endl;
            }
        }
    } catch (exception& e) {
        cout << "Exception in wordListAttack()" << endl;
    }
}

void Attacks::bruteForce() {
    try {
        while(dataP->isNextTest()) {
            string testS = dataP->nextTest();
            string hashcheck = getHash(testS);
            if(dataP->containsHash(hashcheck)) {
                dataP->addCrack(hashcheck+":"+testS);
                cout << hashcheck << ":" << testS << endl;
            }
        }
    } catch (exception& e) {
        cout << "Exception in bruteForce()" << endl;
    }
}

string Attacks::getHash(string word) {
    try {
        switch(dataP->getHashType()) {
            case 0:
                return md5(word);
            case 1:
                return sha1(word);
            case 2:
                return sha256(word);
            case 3:
                return sha3(word);
            default:
                cout << "--< Invalid Hashing Algorithm >--" << endl;
                system("exit");
        }
    } catch (exception& e) {
        cout << "Exception in getHash()" << endl;
        return NULL;
    }
}


#pragma once

#include <iostream>
#include <cstdlib>
#include <list> 
#include <set>
#include <iterator>
#include <sstream>
#include <string>

#ifndef DATA_H
#define DATA_H

namespace std {
    class Data {
        public:
            Data();
            void setAttackMode(int a);
            int getAttackMode();

            void setHashType(int h);
            int getHashType();

            void addHash(string hash);
            void addHashes(list<string> hash);
            bool containsHash(string hash);
            int getHashSize();

            void addWord(string word);
            void addWords(list<string> words);
            string getWord(int i);
            list<string> getWordList();
            int getWordSize();

            void addCrack(string crack);
            void addCracks(list<string> cracks);
            string getCrack(int i);
            list<string> getCracked();
            int getCrackSize();

            int getWordIndex();
            int getTestIndex();

            string nextWord();

            bool isNextWord();

            string nextTest();

            bool isNextTest();


        private:
            int attackMode;
            int hashType;
            int wordIndex;
            int testIndex;
            int test[8] = {0,0,0,0,0,0,0,0};

            set<string> hashes;
            list<string> wordlist;
            list<string> cracked;
    };
}

#endif
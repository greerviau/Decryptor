#pragma once

#include <iostream>
#include <cstdlib>
#include <list> 
#include <vector>
#include <iterator>
#include <sstream>
#include <string>
#define HAVE_STRUCT_TIMESPEC
#include <pthread.h>

#include "data.h"
#include "hash_library/md5.h"
#include "hash_library/sha1.h"
#include "hash_library/sha3.h"
#include "hash_library/sha256.h"

#ifndef ATTACKS_H
#define ATTACKS_H

namespace std {
    class Attacks {
        public:
            Attacks(Data* data);
            void run();
            void dictionaryAttack();
            void bruteForce();
            string getHash(string word);

        private:
            Data* dataP;

            list<string> cracked;

            MD5 md5;
            SHA1 sha1;
            SHA3 sha3;
            SHA256 sha256;

    };
}

#endif
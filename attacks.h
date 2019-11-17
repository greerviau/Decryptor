#pragma once

#include <iostream>
#include <cstdlib>
#include <list> 
#include <iterator>
#include <sstream>
#include <string>

#include "data.h"
#include "hash-library/md5.h"
#include "hash-library/sha1.h"
#include "hash-library/sha3.h"
#include "hash-library/sha256.h"

#ifndef ATTACKS_H
#define ATTACKS_H

namespace std {
    class Attacks {
        public:
            Attacks(Data* data);
            void run();
            void wordListAttack();
            void bruteForce();
            string getHash(string word);

        private:
            Data* dataP;
            MD5 md5;
            SHA1 sha1;
            SHA3 sha3;
            SHA256 sha256;

    };
}

#endif
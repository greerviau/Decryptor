#include <iostream>
#include <cstdlib>
#include <iomanip>
#include <fstream>
#include <list> 
#include <iterator>
#include <sstream>
#include <string>

#include "data.h"
#include "attacks.h"

using namespace std;

void helpMenu() {
    cout << "\n\n- [ Decryptor Help Menu ] -" << endl;
    cout << "\nAccess Help Menu : --help" << endl;
    cout << "\nFormat : decryptor.exe [options]" << endl;
    cout << "\n- [ Options ] -\n" << endl;
    cout << " Options | Type | Description                                | Example" << endl;
    cout << "===============================================================================" << endl;
    cout << " -a      | Num  | Attack Mode                                | -a1" << endl;
    cout << " -m      | Num  | Hash Algorithm                             | -m0" << endl;
    cout << " -h      | File | Hash File                                  | -hC:/hash.txt" << endl;
    cout << " -w      | File | Wordlist File                              | -wC:/wordlist.txt" << endl;
    cout << " -o      | File | Output File                                | -oC:/output.txt" << endl;
    cout << " -t      | Num  | Number Of Threads (Default MAX)            | -t4" << endl;
    cout << "\n- [ Attack Modes ] -\n" << endl;
    cout << " # | Method" << endl;
    cout << "================" << endl;
    cout << " 0 | Wordlist" << endl;
    cout << " 1 | Brute Force" << endl;
    cout << "\n- [ Hash Algorithms ] -\n" << endl;
    cout << " # | Hash" << endl;
    cout << "===========" << endl;
    cout << " 0 | MD5" << endl;
    cout << " 1 | SHA-1" << endl;
    cout << " 2 | SHA-256" << endl;
    cout << " 3 | SHA-3" << endl;
    cout << "\n- [ Format Examples ] -\n" << endl;
    cout << " Attack Mode | Hash Algorithm | Example" << endl;
    cout << "=====================================================================================================" << endl;
    cout << " Wordlist    | MD5            | decryptor.exe -a 0 -m 0 -h ./examples/md5.hash -w example.dict" << endl;
    cout << " Brute Force | SHA256         | decryptor.exe -a 1 -m 2 -h ./examples/sha256.hash" << endl;
}

template<class E>
void showList(std::list <E> const &list) 
{ 
    for(auto const& i: list) { 
        std::cout<<"el " <<i<<endl;
    }
} 

list<string> readFile(string file) {
    string x;
    ifstream inFile;
    list<string> inlist;
    
    inFile.open(file);
    if (!inFile) {
        cout << "Unable to open file [ " << file << " ]";
        exit(1); // terminate with error
    }

    while (inFile >> x) {
        inlist.push_back(x);
    }
    
    inFile.close();
    return inlist;
}

template<class E>
void writeFile(string file, list<E> outlist) {
    ofstream outFile;
    
    outFile.open(file);
    if (!outFile) {
        cout << "Unable to open file";
        exit(1); // terminate with error
    }

    for(auto const& i: outlist) { 
        outFile << i << endl;
    }
    
    outFile.close();
}

void cla(int argc, char** argv, string* hashFile, string* wordFile, string* outFile, int* threads, Data* data) { 
    const int hashTC = (int)'-'+(int)'m';
    const int hashC = (int)'-'+(int)'h';
    const int wordC = (int)'-'+(int)'w';
    const int outC = (int)'-'+(int)'o';
    const int threadC = (int)'-'+(int)'t';
    const int attackC = (int)'-'+(int)'a';
    const int helpC = (int)'-'+(int)'-'+(int)'h'+(int)'e'+(int)'l'+(int)'p';

    int count = 1;
    for (char **pargv = argv+1; *pargv != argv[argc]; pargv+=2) {

        int command = 0;
        for (char *ptr = *pargv; *ptr != '\0'; ptr++) {
            command += (int) *ptr;
        }
        count++;
        string param = "";
        if(count < argc) {
            for (char *ptr = *(pargv+1); *ptr != '\0'; ptr++) {
                param += *ptr;
            }
        }
        switch(command) {
            case hashTC:
                data->setHashType(std::stoi(param));
                break;
            case attackC:
                data->setAttackMode(std::stoi(param));
                break;
            case hashC:
                *hashFile = param;
                break;
            case wordC:
                *wordFile = param;
                break;
            case outC:
                *outFile = param;
                break;
            case threadC:
                *threads = std::stoi(param);
                break;
            case helpC:
                helpMenu();
                break;
            default:
                break;
        }
    }
}

int main(int argc, char** argv) {
    string hashFile = "";
    string wordFile = "";
    string outFile = "";
    int threads = 0;
    Data data;

    if(argc <= 1) {
        cla(argc, argv, &hashFile, &wordFile, &outFile, &threads, &data);
    } else {

    }

    if(hashFile == "") {
        cout << "--< Hash File Required >--" << endl;
        system("exit");
    }
    
    if(data.getAttackMode() == 0 && wordFile == "") {
        cout << "--< WordFile Required for Wordlist Attack >--" << endl;
        system("exit");
    }
    
    cout << "HashFile.....: " << hashFile << endl;
    cout << "WordFile.....: " << wordFile << endl;
    cout << "OutFile......: " << outFile << endl;
    cout << "AttackType...: " << data.getAttackMode() << endl;
    cout << "Threads......: " << threads << endl;

    data.addHashes(readFile(hashFile));
    if(data.getAttackMode() == 0) {
        data.addWords(readFile(wordFile));
    }

    Data* dataptr;
    dataptr = &data;

    Attacks atk(&data);
    atk.run();

    cout << endl << endl;
    cout << "> Status............: Exhausted" << endl;
    cout << "> Cracked...........: " << data.getCrackSize() << "/" << data.getHashSize() << endl;

    if(outFile != "") {
        writeFile(outFile, data.getCracked());
    }

    return 0;
}
#include <iostream>
#include <cstdlib>
#include <iomanip>
#include <fstream>
#include <list> 
#include <iterator>
#include <sstream>
#include <string>
#include <cstring>
#include <vector>
#include <chrono>
#include <ctime>  
#define HAVE_STRUCT_TIMESPEC
#include <pthread.h>

#include "data.h"
#include "attacks.h"

using namespace std;

int err = 0;
const int MAX_THREADS = 8; 

auto start_time = chrono::system_clock::now();

string hash_file_path = "";
string dict_file_path = "";
string out_file_path = "";
int threads = MAX_THREADS;
Data data;

void helpMenu() {
    cout << "\n\n- [ Decryptor Help Menu ] -" << endl;
    cout << "\nAccess Help Menu : --help" << endl;
    cout << "\nFormat : dcrypt.exe [options]" << endl;
    cout << "\n- [ Options ] -\n" << endl;
    cout << " Options | Type | Description                                | Example" << endl;
    cout << "===============================================================================" << endl;
    cout << " -a      | Num  | Attack Mode                                | -a 1" << endl;
    cout << " -m      | Num  | Hash Algorithm                             | -m 0" << endl;
    cout << " -h      | File | Hash File                                  | -h C:/hash.hash" << endl;
    cout << " -d      | File | Dictionary File                            | -d C:/dictionary.dict" << endl;
    cout << " -o      | File | Output File                                | -o C:/output.txt" << endl;
    cout << " -t      | Num  | Number Of Threads (Default MAX)            | -t 4" << endl;
    cout << "\n- [ Attack Modes ] -\n" << endl;
    cout << " # | Method" << endl;
    cout << "================" << endl;
    cout << " 0 | Dictionary" << endl;
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
    cout << " Dictionary  | MD5            | dcrypt.exe -a 0 -m 0 -h ./examples/md5.hash -d example.dict" << endl;
    cout << " Brute Force | SHA256         | dcrypt.exe -a 1 -m 2 -h ./examples/sha256.hash" << endl;
}

void displayData() {
    try {
        cout << endl << endl;
        string attacks[2] = {"Dictionary", "BruteForce"};
        string hashTypes[4] = {"MD5", "SHA1", "SHA256", "SHA3"};
        cout << "> Attack-Mode..........: " << attacks[data.getAttackMode()] << endl;
        cout << "> Hash-Type............: " << hashTypes[data.getHashType()] << endl;
        cout << "> Hash-File............: " << hash_file_path << endl;
        if(out_file_path != "")
            cout << "> OutFile..............: " << out_file_path << endl;
        if(data.getAttackMode() == 0) {
            cout << "> Dictionary...........: " << dict_file_path << endl;
            cout << "> Dictionary-Size......: " << data.getDictSize() << endl;
        } else
            cout << "> Mask.................: [A-Z][a-z][0-9]" << endl;
        cout << "> Threads..............: " << threads << endl;
        time_t start = std::chrono::system_clock::to_time_t(start_time);
        cout << "> Start-Time...........: " << ctime(&start) << endl;
        cout << "> Status...............: Running" << endl;
        cout << "> Cracked..............: " << data.getCrackSize() << "/" << data.getHashSize() << endl;
    } catch (exception& e) {
        cout << "--< Invalid Hash Algorithm >--" <<endl;
        exit(1);
    }
}

template<class E>
void showList(std::list <E> const &list) {
    for(auto const& i: list)
        std::cout<<i<<endl;
} 

list<string> readFile(string path) {
    string x;
    ifstream inFile;
    list<string> inList;
    
    inFile.open(path);
    if (!inFile) {
        cout << "Unable to open file [ " << path << " ]";
        exit(1); // terminate with error
    }

    while (inFile >> x)
        inList.push_back(x);
    
    inFile.close();
    return inList;
}

template<class E>
void writeFile(string path, list<E> outList) {
    ofstream out_file_path;
    
    out_file_path.open(path);
    if (!out_file_path) {
        cout << "Unable to open file [ " << path << " ]";
        exit(1); // terminate with error
    }

    for(auto const& line: outList)
        out_file_path << line << endl;
    
    out_file_path.close();
}

void cli() {
    cout << "Hash Type\n0 = MD5\n1 = SHA1\n2 = SHA256\n3 = SHA3\nEnter Hash Type: ";
    string hsht;
    getline(cin, hsht);
    if(hsht != "")
        data.setHashType(std::stoi(hsht));
    cout << "Path to Hash File: ";
    getline(cin, hash_file_path);
    cout << endl << "Attack Mode\n0 = Dictionary\n1 = BruteForce\nEnter Attack Mode: ";
    string atm;
    getline(cin, atm);
    if(atm != "")
        data.setAttackMode(std::stoi(atm));
    if(data.getAttackMode() == 0) {
        cout << "Path to Dictionary: ";
        getline(cin,dict_file_path);
    }
    cout << "Path to OutFile (blank for no file): ";
    getline(cin,out_file_path);
    cout << "Number of Threads (blank for max): ";
    string t;
    getline(cin,t);
    if(t != "")
        threads = std::stoi(t);

}

void cla(int argc, char** argv) { 
    const int hashTC = (int)'-'+(int)'m';
    const int hashC = (int)'-'+(int)'h';
    const int wordC = (int)'-'+(int)'d';
    const int outC = (int)'-'+(int)'o';
    const int threadC = (int)'-'+(int)'t';
    const int attackC = (int)'-'+(int)'a';
    const int helpC = (int)'-'+(int)'-'+(int)'h'+(int)'e'+(int)'l'+(int)'p';

    int count = 1;
    for (char **pargv = argv+1; *pargv != argv[argc]; pargv+=2) {

        int command = 0;
        for (char *ptr = *pargv; *ptr != '\0'; ptr++)
            command += (int) *ptr;
        
        count++;
        string param = "";
        if(count < argc) {
            for (char *ptr = *(pargv+1); *ptr != '\0'; ptr++) {
                param += *ptr;
            }
        }
        switch(command) {
            case hashTC:
                data.setHashType(std::stoi(param));
                break;
            case attackC:
                data.setAttackMode(std::stoi(param));
                break;
            case hashC:
                hash_file_path = param;
                break;
            case wordC:
                dict_file_path = param;
                break;
            case outC:
                out_file_path = param;
                break;
            case threadC:
                threads = std::stoi(param);
                break;
            case helpC:
                helpMenu();
                exit(1);
            default:
                break;
        }
    }
}

void *execute(void *obj) {
    ((Attacks *)obj)->run();
    return NULL;
}

int main(int argc, char** argv) {
    if(argc > 1)
        cla(argc, argv);
    else
        cli();

    if(data.getHashType() == -1) {
        cout << "--< Hash Type Not Specified >--" << endl;
        exit(1);
    } else if(data.getHashType() > 3) {
        cout << "--< Invalid Hash Type >--" << endl;
        exit(1);
    } else if(data.getAttackMode() == -1) {
        cout << "--< Attack Mode Not Specified >--" << endl;
        exit(1);
    } else if(data.getAttackMode() > 1) {
        cout << "--< Invalid Attack Mode >--" << endl;
        exit(1);
    } else if(hash_file_path == "") {
        cout << "--< Hash File Required >--" << endl;
        exit(1);
    } else if(data.getAttackMode() == 0 && dict_file_path == "") {
        cout << "--< Dictionary Required for Dictionary Attack >--" << endl;
        exit(1);
    } else if(threads > MAX_THREADS) {
        cout << "--< Number of Threads Exceeds MAX THREADS >--" << endl;
        exit(1);
    } else if(threads < 0) {
        cout << "--< Must Use At Least 1 Thread >--" << endl;
        exit(1);
    }

    cout << "\n\n> Loading Hash File..." << endl;
    data.addHashes(readFile(hash_file_path));
    cout << "> Hash File Loaded" << endl;
    if(data.getAttackMode() == 0) {
        cout << "> Loading Dictionary..." << endl;
        data.addWords(readFile(dict_file_path));
        cout << "> Dictionary Loaded" << endl;
    }

    start_time = chrono::system_clock::now();

    displayData();

    pthread_t t_id[threads];

    vector<Attacks> atk(threads, Attacks(&data));

    for(int i = 0; i < threads; i++) {
        cout << "> Thread " << i+1 << " Created" << endl;
        pthread_create(&t_id[i],NULL,&execute, &atk[i]);
    }

    for(int i = 0; i < threads; i++)
        pthread_join(t_id[i], NULL);


    auto end_time = chrono::system_clock::now();
    time_t start = std::chrono::system_clock::to_time_t(end_time);
    chrono::duration<double> elapsed_seconds = end_time-start_time;

    cout << endl << endl;
    cout << "> Status...............: Exhausted" << endl;
    cout << "> Finished.............: " << ctime(&start);
    cout << "> Elapsed-Time.........: " << elapsed_seconds.count() << " sec" << endl;
    cout << "> Cracked..............: " << data.getCrackSize() << "/" << data.getHashSize() << endl;

    if(out_file_path != "")
        writeFile(out_file_path, data.getCracked());

    return 0;
}
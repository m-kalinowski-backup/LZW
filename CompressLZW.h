#ifndef COMPRESSLZW_H
#define COMPRESSLZW_H

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <unordered_map>
#include <math.h>

using namespace std;

class CompressLZW{

  public:
    CompressLZW();
    void openFileBinary(string filename, string filename2);
    void compressFile();
    void deltaEncode(int index);
    void gammaEncode(int index);
    void omegaEncode(int index);
    void fibEncode(int index);
    void writeByte(string temp);


  private:
    unordered_map<string, int> dictionary;
    ifstream inFile;
    ofstream outFile;
    string codeBuffer;
    void initDictionary();
    string toBinary(int n);
    int findDictIndex(string word);

};

#endif

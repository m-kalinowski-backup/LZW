#ifndef DECOMPRESSLZW_H
#define DECOMPRESSLZW_H

#include <string>
#include <fstream>
#include <vector>
#include <iostream>
#include <math.h>

using namespace std;

class DecompressLZW{

  public:
    DecompressLZW();
    void decompressFile();
    void openFileBinary(string fileIn, string fileOut);
    void decodeGamma();
    void omegaDecode();
    void deltaDecode();
    void fibDecode();
    void decompressLZW(int index);

  private:
    void initDictionary();
    void printDict();
    vector<string> dictionary;
    ifstream inFile;
    ofstream outFile;
};

#endif

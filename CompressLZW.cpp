#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <bits/stdc++.h>
#include <unordered_map>

#include "CompressLZW.h"

using namespace std;

CompressLZW::CompressLZW(){
  initDictionary();
}

void CompressLZW::initDictionary(){
  string s;

  for(int i = 0; i <= 255; i ++){
    s+= (char)i;
    dictionary.insert({s,i+1});
    s = "";
  }
}

void CompressLZW::openFileBinary(string filename, string filename2){
  inFile.open(filename, ios::in | ios::binary);
  outFile.open(filename2, ios::out |ios::binary);
}

int CompressLZW::findDictIndex(string word){
  int index = -1;
  unordered_map<string,int>::iterator it = dictionary.find(word);

  if(it != dictionary.end()){
    index = it->second;
  }
  return index;
}

void CompressLZW::deltaEncode(int index){
  int len = 0;
  int lengthOfLen = 0;
  string code  = "";

  for (int temp = index; temp > 0; temp >>= 1)  // calculate 1+floor(log2(num))
    len++;

  for (int temp = len; temp > 1; temp >>= 1)  // calculate floor(log2(len))
    lengthOfLen++;

  for (int i = lengthOfLen; i > 0; --i)
    code+="0";

  for (int i = lengthOfLen; i >= 0; --i){
    if((len >> i) & 1){
      code+="1";
    }
    else{
      code+="0";
    }
  }

  for (int i = len-2; i >= 0; i--){
    if((index >> i) & 1){
      code+="1";
    }
    else{
      code+="0";
    }
  }
  codeBuffer += code;

  while(codeBuffer.length() >= 8){
    string temp2 = codeBuffer.substr(0,8);
    writeByte(temp2);
    codeBuffer = codeBuffer.substr(8, codeBuffer.length());
  }
}

void CompressLZW::omegaEncode(int index){
  string code  = "0";
  string temp ="";
  int n = index;

  while(n>1){
    temp = toBinary(n);
    n = temp.length()-1;
    code = temp + code;
  }

  codeBuffer+=code;
  while(codeBuffer.length() >= 8){
    string temp2 = codeBuffer.substr(0,8);
    writeByte(temp2);
    codeBuffer = codeBuffer.substr(8, codeBuffer.length());
  }
}

void CompressLZW::fibEncode(int index){
  cout << index << endl;
  int fib[50];
  fib[0] = 1;
  fib[1] = 2;

  int i;
  for(i = 2; fib[i-1] <= index ; i ++){
    fib[i] = fib[i-1] + fib[i-2];
  }

  i = i -2;
  string code = "1";

  while(index) {
    code= '1'+code;
    index = index - fib[i];
    i = i - 1;

    while(i>=0 && fib[i]>index) {
      code = '0' + code;
      i = i - 1;
    }
  }
  codeBuffer += code;
  while(codeBuffer.length() >= 8){
    string temp = codeBuffer.substr(0,8);
    writeByte(temp);
    codeBuffer = codeBuffer.substr(8, codeBuffer.length());
  }
}

void CompressLZW::gammaEncode(int index){
  int size  = (int)log2(index) + 1;
  for(int i = 0; i < size - 1; i++){
    codeBuffer += '0';
  }
  codeBuffer+=toBinary(index);
  while(codeBuffer.length() >= 8){
    string temp = codeBuffer.substr(0,8);
    writeByte(temp);
    codeBuffer = codeBuffer.substr(8, codeBuffer.length());
  }
}
string CompressLZW::toBinary(int n)
{
    string r;
    while(n!=0) {r=(n%2==0 ?"0":"1")+r; n/=2;}
    return r;
}

void CompressLZW::writeByte(string byte){
  uint8_t byteBuff = 0;
  int currBit = 0;

  for(int i = 0; i <= byte.length()-1; i++){
    if(byte[i]=='1'){
      byteBuff |= (1<<currBit);
    }
    currBit++;
  }
  outFile.write((char*)&byteBuff,sizeof(char));
}

void CompressLZW::compressFile(){
  uint8_t currChar;
  int last_index,index = 0;
  string currWord = "";

  while(inFile.read((char*)&currChar,sizeof(char))){
    currWord += currChar;
    index = findDictIndex(currWord);

    if(index == -1){
      deltaEncode(last_index);
      dictionary.insert({currWord,dictionary.size()+1});
      currWord = currWord.substr(currWord.size()-1,currWord.size());
    }
    last_index = findDictIndex(currWord);
  }
  deltaEncode(last_index);

  if(codeBuffer.length() > 0){
    int size = 8 - codeBuffer.length();
    for(int i = 0; i < size; i++){
      codeBuffer += '0';
    }
    writeByte(codeBuffer);
  }
  inFile.close();
  outFile.flush();
  outFile.close();
}

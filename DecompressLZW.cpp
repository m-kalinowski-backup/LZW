#include "DecompressLZW.h"
#include "BitReader.h"

DecompressLZW::DecompressLZW(){
  initDictionary();
}

void DecompressLZW::initDictionary(){
  dictionary.push_back("");
  string s;
  for(int i = 0; i <= 255; i++ ){
    s+= (char) i;
    dictionary.push_back(s);
    s = "";
  }
}

void DecompressLZW::deltaDecode(){
  BitReader *reader = new BitReader();
  reader->setStream(inFile);

  while(!reader->eof()){
    int num = 1;
    int len = 1;
    int lengthOfLen = 0;

    while(!reader->getBit()){
      lengthOfLen++;
    }
    if(!reader->eof()){
      for (int i = 0; i < lengthOfLen; i++){
        len <<= 1;
        if (reader->getBit()){
          len |= 1;
        }
      }
      for (int i = 0; i < len-1; i++){
        num <<= 1;
        if (reader->getBit()){
          num |= 1;
        }
      }
      decompressLZW(num);
    }
  }
  outFile.close();
}

void DecompressLZW::openFileBinary(string fileIn, string fileOut){
  inFile.open(fileIn, ios::in | ios::binary);
  outFile.open(fileOut, ios::out | ios::binary);
}

void DecompressLZW::fibDecode(){
  BitReader *reader = new BitReader();
  reader->setStream(inFile);
  int fib[40];
  fib[0] = 1;
  fib[1] = 2;

  int i;
  for(i = 2; i < 40 ; i ++){
    fib[i] = fib[i-1] + fib[i-2];
  }
  int index = 0;
  i = 0;
  bool wasOne = false;

  while(!reader->eof()){
    if(reader->getBit()){
      if(wasOne){
        decompressLZW(index);
        index = 0;
        i = 0;
        wasOne = false;
      }
      else{
        index += fib[i];
        i++;
        wasOne = true;
      }
    }
    else{
      i++;
      wasOne = false;
    }
  }
  outFile.close();
}

void DecompressLZW::omegaDecode(){
  int n = 1;
  int length = n;
  BitReader *reader = new BitReader();
  reader->setStream(inFile);

  while(!reader->eof()){
    n = 1;

    while(reader->getBit()){
      length = n;
      n=1;

      for(int i =0; i < length; i++){
        n <<= 1;
        if(reader->getBit())
          n |= 1;
      }
    }
    decompressLZW(n);
  }
  outFile.close();
}

void DecompressLZW::decodeGamma(){
  int length = 0;
  bool readNum = false;
  uint8_t currChar = 0;
  int index = 0;

  while(inFile.read((char*)&currChar,sizeof(char))){
    for(int i = 0; i <= 7; i++){
      if(((currChar >> i) & 1) == 1){
        readNum = true;
        index += pow(2,length);
        length--;
      }
      else{
        if(!readNum){
          length++;
        }
        else{
          length --;
        }
      }
      if(length ==-1){
        decompressLZW(index);
        index = 0;
        length = 0;
        readNum= false;
      }
    }
  }
  inFile.close();
  outFile.flush();
  outFile.close();
}

void DecompressLZW::decompressLZW(int index){
  string code;

  if(dictionary.size() == 257){
    code = dictionary[index];
    dictionary.push_back(code);
  }
  else{
    if(index < dictionary.size()-1 ){
      code = dictionary[index];
      dictionary[dictionary.size()-1] += code[0];
      dictionary.push_back(code);
    }

    else{
      string lastCode = dictionary[dictionary.size()-1];
      dictionary[dictionary.size()-1] += lastCode[0];
      code = dictionary[index];
      dictionary.push_back(code);
    }
  }
  for(int i = 0; i < code.length(); i ++){
    outFile.write((char*)&code[i],sizeof(char));
  }
}

#include "BitReader.h"

BitReader::BitReader(){
   buffPointer = 8;
}

void BitReader::setStream(ifstream &file){
   inFile = (&file);
   inFile->seekg(0, ios::end);
   fileSize = inFile->tellg();
   inFile->seekg(0,ios::beg);
}

bool BitReader::eof(){
   if(fileSize>=0){
      return false;
   }
   return true;
}

int BitReader::getBit(){
   if(buffPointer == 8){
      inFile->read((char*)&buffer, sizeof(char));
      fileSize -= sizeof(char);
      buffPointer = 0;
   }

   if(((buffer >> buffPointer) & 1) == 1){
      buffPointer++;
      return 1;
   }
   else{
      buffPointer++;
      return 0;
   }
}

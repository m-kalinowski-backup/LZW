#ifndef BITREADER_H
#define BITREADER_H

#include <fstream>

using namespace std;

class BitReader{
   public:
      BitReader();
      int getBit();
      void setStream(ifstream &file);
      bool eof();

   private:
      int fileSize;
      int buffPointer;
      uint8_t buffer;
      ifstream *inFile;
};

#endif

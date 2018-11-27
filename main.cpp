#include "CompressLZW.h"
#include "DecompressLZW.h"
#include "BitReader.h"

#include <fstream>

using namespace std;

int main(){
  CompressLZW* cmp = new CompressLZW();
  DecompressLZW* dcmp = new DecompressLZW();

  cmp->openFileBinary("test1.bin","cmp_test1.bin");
 cmp->compressFile();
 dcmp->openFileBinary("cmp_test1.bin","dcmp_test1.bin");
 dcmp->deltaDecode();

}

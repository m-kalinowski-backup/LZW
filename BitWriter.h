#include <fstream>

class BitWriter{

  public:
    BitWriter();
    void setStream(ofstream outFile);
    void writeBit(int bit);

  private:
    uint8_t buffer;
    int buffPointer;
    ofstream* outFile;
}


BitWriter::BitWriter(){

}

BitWriter::setStream(ofstream &outFile){
  this->outFile = (&outFile);
}

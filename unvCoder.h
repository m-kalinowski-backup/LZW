class unvCoder{
  public:
    void encode();
    void decode();

  private:
    auto encFunction();
    auto decFunction();
    void fibEncode(int index);
    void gammaEncode(int index);
    void deltaEncode(int index);
    void omegaEncode(int index);
}

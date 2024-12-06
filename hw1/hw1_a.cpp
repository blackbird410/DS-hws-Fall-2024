#include <iostream>
#include <ctime>

class numberguessing {
private:
  int value;
  int times;
public:
  numberguessing() : value{0}, times{0} {};
  ~numberguessing() {};
  void Set(int v) { value = v; };
  int Guesser(int v) { 
    times++; 
    return (v < value) ? -1 : (v > value) ? 1 : 0; };
  int Time() { return times; };
  void Reset() { times = 0; };
};

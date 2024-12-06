#include <algorithm>
#include <chrono>
#include <fstream>
#include <iostream>
#include <iterator>
#include <stdexcept>

using namespace std;

void Magic(const int n) {
  const int MaxSize = 51;
  int square[MaxSize][MaxSize], k, l;
  if ((n >= MaxSize) || (n < 1))
    throw "Error!..n out of range!";
  else if (n % 2 == 0)
    throw "Error!..n is even!";

  for (int i = 0; i < n; i++)
    fill(square[i], square[i] + n, 0); // Using STL to initialize
  square[0][(n - 1) / 2] = 1;
  int key = 2, i = 0, j = (n - 1) / 2;
  while (key <= n * n) {
    if (i - 1 < 0)
      k = n - 1;
    else
      k = i - 1;
    if (j - 1 < 0)
      l = n - 1;
    else
      l = j - 1;
    if (square[k][l])
      i = (i + 1) % n;
    else {
      i = k;
      j = l;
    }
    square[i][j] = key;
    key++;
  }
  cout << "magic square of size " << n << endl;
  for (i = 0; i < n; i++) {
    copy(square[i], square[i] + n, ostream_iterator<int>(cout, " "));
    cout << endl;
  }
}

chrono::duration<double> TimeMagic(const int n, const int iterations) {
  auto start = chrono::high_resolution_clock::now();
  for (int i = 0; i < iterations; i++) {
    Magic(n);
  }
  auto end = chrono::high_resolution_clock::now();
  return end - start;
}

void analyzeMagic() {
  ofstream outFile("magic.txt");
  ofstream outFileCsv("magic.csv");
  outFile << "Magic Square Analysis" << endl;
  outFileCsv << "Magic Square Analysis" << endl;
  outFileCsv << "Size,Iterations,Total Time (s),Average Time (s)" << endl;

  int sizes[] = {11, 21, 31, 41, 51};
  for (int i = 0; i < 5; i++) {
    outFile << "Size: " << sizes[i] << endl;
    outFile << "Iterations\tTotal Time (s)\tAverage Time (s)" << endl;
    for (int j = 10; j <= 100; j += 10) {
      auto duration = TimeMagic(sizes[i], j);
      outFile << "\t" << j << "\t" << duration.count() << "\t"
              << duration.count() / j << endl;
      outFileCsv << sizes[i] << "," << j << "," << duration.count() << ","
                 << duration.count() / j << endl;
    }
  }
  outFile.close();
  outFileCsv.close();
}

void printData() {
  ifstream inFile("magic.txt");
  if (!inFile)
    throw runtime_error("File not found");
  string line;
  while (getline(inFile, line))
    cout << line << endl;
}

int main() {
  try {
    analyzeMagic();
    printData();
  } catch (const std::exception &e) {
    cerr << e.what() << endl;
    return 1;
  }
  return 0;
}

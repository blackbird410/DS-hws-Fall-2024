#include <iostream>
#include <string>
#include <fstream>
#include <sstream>

// Reads in a text file
// Writes :
// - The histogram of word counts sorted in alphabetical order
// - The statistics of character counts in ascending order to two separate output files

#define HISTOGRAM_FILE "hist.txt"
#define CHARACTER_FILE "freq.txt"
#define INPUT_FILE "input.txt"
#define ASCII_CHARACTERS "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ"

using namespace std;

class Frequency {
public:
  Frequency() : value{""}, frequency{0} {}
  Frequency(const string& v, const int& f) : value{v}, frequency{f} {}
  ~Frequency() {}

  void setValue(const string &v) { value = v; }
  void setFrequency(const int &f) { frequency = f; }
  string getValue() const { return value; }
  int getFrequency() const { return frequency; }

  Frequency &operator=(const Frequency &rhs) {
    if (this != &rhs) {
      value = rhs.value;
      frequency = rhs.frequency;
    }
    return *this;
  }

  Frequency &operator=(const string &rhs) {
    value = rhs;
    frequency = 1;
    return *this;
  }

  friend ostream& operator<<(ostream &os, const Frequency &f) {
    os << f.value << "\t" << f.frequency;
    return os;
  }

private:
  string value;
  int frequency;
};


class Node {
public:
  Node() {
    next = nullptr;
    pre = nullptr;
  }

  Node(const string &v) {
    data = Frequency(v, 1); 
    next = nullptr;
    pre = nullptr;
  }

  Node(Frequency &n) {
    data = n;
    next = nullptr;
    pre = nullptr;
  }

  // Return reference to allow modification of the Frequency object
  Frequency& getData() { return data; }

  Node *getNext() { return next; }
  Node *getPre() { return pre; }

  void setData(Frequency &d) { data = d; }
  void setNext(Node *n) { next = n; }
  void setPre(Node *p) { pre = p; }

private:
  Frequency data;
  Node *next, *pre;
};

class List {
public:
  List() { head = NULL; }
  ~List() {
    Node *current = head;
    Node *next = NULL;
    while (current != NULL) {
      next = current->getNext();
      delete current;
      current = next;
    }
  }

  void addElement(const string& v) {
    Node *newNode = new Node(v);
    Node *current = head;
    Node *previous = NULL;

    while (current != NULL && current->getData().getValue() < v) {
      previous = current;
      current = current->getNext();
    }

    if (previous == NULL) {
      newNode->setNext(head);
      head = newNode;
    } else {
      previous->setNext(newNode);
      newNode->setNext(current);
    }
  }

  void addElement(Frequency &f) {
    Node *newNode = new Node(f);
    Node *current = head;
    Node *previous = NULL;

    while (current != NULL && current->getData().getValue() < f.getValue()) {
      previous = current;
      current = current->getNext();
    }

    if (previous == NULL) {
      newNode->setNext(head);
      head = newNode;
    } else {
      previous->setNext(newNode);
      newNode->setNext(current);
    }
  }

  Node *findElement(const string &v) {
    Node *current = head;
    while (current != NULL) {
      if (current->getData().getValue() == v) {
        return current;
      }
      current = current->getNext();
    }
    return NULL;
  }

  int removeElement(const string &v) {
    Node *current = head;
    Node *previous = NULL;

    while (current != NULL) {
      if (current->getData().getValue() == v) {
        if (previous == NULL) {
          head = current->getNext();
        } else {
          previous->setNext(current->getNext());
        }
        delete current;
        return 1;
      }
      previous = current;
      current = current->getNext();
    }
    return -1;
  }

  void swap(Node *a, Node *b) {
    Frequency temp = a->getData();
    a->setData(b->getData());
    b->setData(temp);
  }

  void SortByValue() {
    Node *current = head;
    Node *smallest;
    Node *pTrav;

    while (current != NULL) {
      pTrav = current->getNext();
      smallest = current;

      while (pTrav != NULL) {
        if (pTrav->getData().getValue() < smallest->getData().getValue())
          smallest = pTrav;
        pTrav = pTrav->getNext();
      }

      if (smallest->getData().getValue() < current->getData().getValue())
        swap(current, smallest);

      current = current->getNext();
    }
  }

  void SortByFrequency() {
    Node *current = head;
    Node *smallest;
    Node *pTrav;

    while (current != NULL) {
      pTrav = current->getNext();
      smallest = current;

      while (pTrav != NULL) {
        if (pTrav->getData().getFrequency() < smallest->getData().getFrequency())
          smallest = pTrav;
        pTrav = pTrav->getNext();
      }

      if (smallest->getData().getFrequency() < current->getData().getFrequency())
        swap(current, smallest);

      current = current->getNext();
    }
  }

  void printList() {
    Node *current = head;
    while (current != NULL) {
      cout << current->getData().getValue() << "\t" << current->getData().getFrequency() << endl;
      current = current->getNext();
    }
  }


  void printHistogram(int maxWordLength, string filename = HISTOGRAM_FILE) {
    ofstream outFile(filename);
    Node *current = head;

    while (current != NULL) {
      outFile << current->getData().getValue() << "    ";

      for (int i = 0; i < maxWordLength - current->getData().getValue().length(); i++)
        outFile << " ";

      for (int i = 0; i < current->getData().getFrequency(); i++) 
        outFile << "*";

      outFile << current->getData().getFrequency()<< endl;

      current = current->getNext();
    }
  }

  friend ostream& operator<<(ostream &os, const List &l) {
    Node *current = l.head;
    while (current != NULL) {
      os << current->getData() << endl;
      current = current->getNext();
    }
    return os;
  }

private:
  Node* head;
};

class WordStatistics {
public:
  WordStatistics() : text{""}, wordList{new List()}, characterList{new List()} {};
  explicit WordStatistics(const string &, const string &, const string &);
  ~WordStatistics() {
    delete wordList;
    delete characterList;
  }
  void analyze();
  void outputStatistics();

private:
  string text;
  string histogramFile;
  string characterFile;

  int maxWordLength;
  List* wordList;
  List* characterList;
};

WordStatistics::WordStatistics(const string& filepath, const string& output, const string& character)
  : text{""}, 
  maxWordLength{0}, 
  wordList{new List()}, 
  characterList{new List()}, 
  histogramFile{output}, 
  characterFile{character} 
{
  ifstream inFile;
  try {
    cout << "Loading input file..." << endl;
    inFile.open(filepath);
    if (!inFile) {
      throw filepath;
    }
    getline(inFile, text, '\0');
    analyze();
    cout << "File loaded successfully!" << endl;
    inFile.close();
  } catch (const string& filepath) {
    cerr << "Error: Unable to open file " << filepath << endl;
  }
}

void WordStatistics::analyze() {
  stringstream ss(text);
  string word;
  Node* target = NULL;

  while (ss >> word) {
    for (int i = 0; i < word.length(); i++) {
      string character = word.substr(i, 1);
      target = characterList->findElement(character);
      if (target) {
        Frequency &data = target->getData(); 
        data.setFrequency(data.getFrequency() + 1);
      } else {
        Frequency f(character, 1);
        characterList->addElement(f);
      }
    }

    // Check if it is a valid word
    if (word.find_first_not_of(ASCII_CHARACTERS) != string::npos)
      continue;

    if (word.length() > maxWordLength)
      maxWordLength = word.length();

    target = wordList->findElement(word);
    if (target) {
      Frequency &data = target->getData();
      data.setFrequency(data.getFrequency() + 1);
    } else {
      Frequency f(word, 1);
      wordList->addElement(f);
    }
  }
}

void WordStatistics::outputStatistics() {
  if (text.empty()) {
    cerr << "Error: No text to analyze" << endl;
    return;
  }

  ofstream outFile(characterFile);

  wordList->SortByValue();
  wordList->printHistogram(maxWordLength, histogramFile);
  characterList->SortByFrequency();
  outFile << *characterList << endl;
}

int main (int argc, char *argv[]) {
  WordStatistics *analysis;
  string inputFileName;
  string outputFileName;
  string characterFileName;

  std::getline(std::cin, inputFileName);
  std::getline(std::cin, outputFileName);
  std::getline(std::cin, characterFileName);
  
  analysis = new WordStatistics(inputFileName, outputFileName, characterFileName);
  analysis->outputStatistics();
  cout << "Analysis completed!" << endl;

  delete analysis;
  
  return 0;
}

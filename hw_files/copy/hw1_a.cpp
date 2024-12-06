#include <iostream>
#include <ctime>

#define NEW_GAME 1
#define RESTART 2
#define QUIT 3

class NumberGuessing {
public:
  NumberGuessing();
  ~NumberGuessing();
  void Set(int v) { value = v; };
  int Guesser(int v) { 
    times++; 
    return (v < value) ? -1 : (v > value) ? 1 : 0; };
  int Time() { return times; };
  void Reset() { times = 0; };
  void printMenu() {
    std::cout << "---------- Number Guessing Game ----------" << std::endl;
    std::cout << "------------------------------------------" << std::endl;
    std::cout << "------------------- Menu -----------------" << std::endl;
    std::cout << "------------------------------------------" << std::endl;
    std::cout << "1. New Game" << std::endl;
    std::cout << "2. Restart" << std::endl;
    std::cout << "3. Quit" << std::endl;
    std::cout << "------------------------------------------" << std::endl;
  }

private:
  int value;
  int times;
};

NumberGuessing::NumberGuessing() : value{0}, times{0} {}
NumberGuessing::~NumberGuessing() {}

int main() {
  // Simulate the number guessing game
  int cmd = NEW_GAME;
  NumberGuessing *game = new NumberGuessing();
  int v, result;
  srand(time(NULL));

  while (cmd != QUIT) {
    game->printMenu();
    std::cout << "Please enter your command: ";
    std::cin >> cmd;

    if (cmd == NEW_GAME || cmd == RESTART) {
      game->Reset();
      game->Set(rand() % 100);
      std::cout << "Game started!" << std::endl;
      
      while (true) {
        std::cout << "Please enter your guess: ";
        std::cin >> v;
        result = game->Guesser(v);
        
        if (result == 0) {
          std::cout << "Congratulations! You have guessed the number in " << game->Time() << " attempts!" << std::endl;
          break;
        } else if (result == -1) {
          std::cout << "Your guess is too low!" << std::endl;
        } else {
          std::cout << "Your guess is too high!" << std::endl;
        }
      }
    } else if (cmd == QUIT) {
      std::cout << "Game quit!" << std::endl;
      break;
    } else {
      std::cout << "Invalid command! Please try again." << std::endl;
    }
  }

  delete game;
  return 0;
}

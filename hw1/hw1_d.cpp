#include <iostream>

int calculate(char op1, char op2, int i1, int i2, int i3) {
  auto isValidOperator = [](char op) {
    return (op == '+' || op == '-' || op == '*' || op == '/');
  };

  auto resolve = [](char op, int a, int b) -> int {
    switch (op) {
      case '+': return a + b;
      case '-': return a - b;
      case '*': return a * b;
      case '/':
        if (b == 0) throw "Division by zero error";
        return a / b;
      default:
        throw 'O';
    }
  };

  if ((op2 == '/' && i3 == 0) || (op1 == '/' && i2 == 0)) {
    throw "Division by zero error";
  }

  if (!isValidOperator(op1) || !isValidOperator(op2)) {
    throw 'O';
  }

  int intermediateResult = resolve(op2, i2, i3);
  return resolve(op1, i1, intermediateResult);
}

int main() {
  char op1, op2, ch;
  int i1, i2, i3;

  while (true) {
    if (!(std::cin >> i1 >> op1 >> i2 >> op2 >> i3)) {
      if (std::cin.eof() || std::cin.fail()) break;
      std::cin.clear();

      while (std::cin.get(ch) && ch != '\n');
      std::cout << "Invalid input. Please try again.\n";
      continue;
    }

    try {
      int result = calculate(op1, op2, i1, i2, i3);
      std::cout << result << std::endl;
    } catch (char exception) {
      std::cerr << "Error: Unknown operator" << std::endl;
    } catch (const char* exception) {
      std::cerr << "Error: " << exception << std::endl;
    } catch (const std::exception& e) {
      std::cerr << "An unexpected error occurred: " << e.what() << std::endl;
    }
  }

  return 0;
}


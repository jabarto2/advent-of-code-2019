#include "IntCodeComputer.h"

#include <iostream>
#include <fstream>
#include <string>
#include <vector>

int main()
{
  std::string input_file = "/home/joe/projects/advent-of-code-2019/day5/input/intcode.txt";
  std::ifstream file;
  file.open(input_file.c_str());

  std::vector<int> code;
  std::string word;
  while (std::getline(file, word, ','))
  {
    code.push_back(std::stoi(word));
  }

  IntCodeComputer computer(code);

  computer.executeProgram();

  return 0;
}
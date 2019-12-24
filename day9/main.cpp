#include "../day5/IntCodeComputer.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <memory>
#include <algorithm>

int main()
{
  std::string input_file = "/home/joe/projects/advent-of-code-2019/day9/input/boost_program.txt";
  std::ifstream file;
  file.open(input_file.c_str());

  std::vector<long> code;
  std::string word;
  while (std::getline(file, word, ','))
  {
    code.push_back(std::stol(word));
  }

  IntCodeComputer computer(code);
  std::deque<long> input, output;
  input.push_back(1);
  computer.executeProgram(input, output);
  for (auto& out : output)
  {
    std::cout << out << std::endl;
  }
  return 0;
}

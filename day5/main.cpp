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

  std::vector<long> code;
  std::string word;
  while (std::getline(file, word, ','))
  {
    code.push_back(std::stol(word));
  }

  IntCodeComputer computer(code);

  // Part 1
  std::deque<long> inputs, outputs;
  inputs.push_front(1);
  computer.executeProgram(inputs, outputs);

  for (auto& output : outputs)
  {
    std::cout << outputs.front() << std::endl;
    outputs.pop_front();
  }

  // Part 2
  computer.resetProgramMemory();
  inputs.clear();
  outputs.clear();
  inputs.push_front(5);
  computer.executeProgram(inputs, outputs);

  for (auto& output : outputs)
  {
    std::cout << outputs.front() << std::endl;
    outputs.pop_front();
  }

  return 0;
}
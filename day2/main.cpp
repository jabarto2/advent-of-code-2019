#include <iostream>
#include <fstream>
#include <string>
#include <vector>

class IntCodeComputer
{
public:
  IntCodeComputer(std::vector<int> program_memory)
  {
    original_program_memory_ = program_memory;
    program_memory_ = program_memory;
  }

  void resetProgramMemory()
  {
    program_memory_ = original_program_memory_;
  }

  int executeProgram(int noun, int verb)
  {
    program_memory_[1] = noun;
    program_memory_[2] = verb;

    for (int i = 0; i < program_memory_.size(); i += 4)
    {
      int opcode = program_memory_.at(i);
      if (opcode == 99)
        return program_memory_.front();
      else if (opcode == 1)
      {
        int operand1 = program_memory_[program_memory_[i + 1]];
        int operand2 = program_memory_[program_memory_[i + 2]];
        int destination = program_memory_[i + 3];
        program_memory_[destination] = operand1 + operand2;
      }
      else if (opcode == 2)
      {
        int operand1 = program_memory_[program_memory_[i + 1]];
        int operand2 = program_memory_[program_memory_[i + 2]];
        int destination = program_memory_[i + 3];
        program_memory_[destination] = operand1 * operand2;
      }
    }
  }

  std::vector<int> original_program_memory_;
  std::vector<int> program_memory_;
};

int main()
{
  std::string input_file = "/home/joe/projects/advent-of-code-2019/day2/input/intcode.txt";
  std::ifstream file;
  file.open(input_file.c_str());

  std::vector<int> code;
  std::string word;
  while (std::getline(file, word, ','))
  {
    code.push_back(std::stoi(word));
  }

  IntCodeComputer computer(code);

  // Part 1 - restore program to "1202 program alarm" state
  code[1] = 12;
  code[2] = 2;
  int output = computer.executeProgram(12, 2);
  std::cout << "Output of 1202 program alarm state program: " << output << std::endl;

  // Part 2 - find noun and verb to get an output of 19690720
  computer.resetProgramMemory();
  for (int noun = 0; noun < 100; ++noun)
  {
    for (int verb = 0; verb < 100; ++verb)
    {
      int output = computer.executeProgram(noun, verb);
      //   std::cout << output << std::endl;
      if (output == 19690720)
      {
        std::cout << "Noun = " << noun << ", Verb = " << verb << std::endl;
      }
      computer.resetProgramMemory();
    }
  }

  return 0;
}
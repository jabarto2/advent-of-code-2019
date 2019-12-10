#ifndef INTCODECOMPUTER_H_
#define INTCODECOMPUTER_H_

#include <vector>

class IntCodeComputer
{
public:
  IntCodeComputer(std::vector<int> program_memory);

  void resetProgramMemory();

  void executeProgram();

private:
  int getInstructionParameter(int index, int parameter_mode);

  std::vector<int> original_program_memory_;
  std::vector<int> program_memory_;
};

#endif
#ifndef INTCODECOMPUTER_H_
#define INTCODECOMPUTER_H_

#include <vector>
#include <deque>

class IntCodeComputer
{
public:
  IntCodeComputer(std::vector<long> program_memory);

  void resetProgramMemory();

  void executeProgram(std::deque<long>& inputs, std::deque<long>& outputs);

private:
  long readInstructionParameter(long index, long parameter_mode = 0);
  void writeInstructionResult(long index, long result, long parameter_mode = 0);

  std::vector<long> original_program_memory_;
  std::vector<long> program_memory_;
  long relative_base_offset_ = 0;
};

#endif
#include <vector>

struct Instruction
{
  Instruction(int instruction_value)
  {
    opcode = instruction_value % 100;

    parameter_modes.push_back(instruction_value / 100 % 10);
    parameter_modes.push_back(instruction_value / 1000 % 10);
    parameter_modes.push_back(instruction_value / 10000 % 10);
  }

  int opcode;
  std::vector<int> parameter_modes;
};
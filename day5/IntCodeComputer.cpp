#include "IntCodeComputer.h"
#include "Instruction.h"

#include <iostream>
#include <thread>

IntCodeComputer::IntCodeComputer(std::vector<int> program_memory)
{
  original_program_memory_ = program_memory;
  program_memory_ = program_memory;
}

void IntCodeComputer::resetProgramMemory()
{
  program_memory_ = original_program_memory_;
}

void IntCodeComputer::executeProgram(std::deque<int>& inputs, std::deque<int>& outputs)
{
  for (int i = 0; i < program_memory_.size();)
  {
    Instruction instruction(program_memory_.at(i));

    if (instruction.opcode == 99)
      return;
    else if (instruction.opcode == 1)
    {
      // Add
      int operand1 = getInstructionParameter(i + 1, instruction.parameter_modes.at(0));
      int operand2 = getInstructionParameter(i + 2, instruction.parameter_modes.at(1));
      int destination = program_memory_[i + 3];

      program_memory_[destination] = operand1 + operand2;
      i += 4;
    }
    else if (instruction.opcode == 2)
    {
      // Multiply
      int operand1 = getInstructionParameter(i + 1, instruction.parameter_modes.at(0));
      int operand2 = getInstructionParameter(i + 2, instruction.parameter_modes.at(1));
      int destination = program_memory_[i + 3];

      program_memory_[destination] = operand1 * operand2;
      i += 4;
    }
    else if (instruction.opcode == 3)
    {
      // take input and store at the operand address
      while (inputs.empty())
      {
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
      }

      int input = inputs.front();
      inputs.pop_front();
      program_memory_[program_memory_[i + 1]] = input;
      i += 2;
    }
    else if (instruction.opcode == 4)
    {
      // output the value at the operand address
      int output = getInstructionParameter(i + 1, instruction.parameter_modes.at(0));

      outputs.push_back(output);
      i += 2;
    }
    else if (instruction.opcode == 5)
    {
      // Jump to instruction if true
      int jump_condition = getInstructionParameter(i + 1, instruction.parameter_modes.at(0));
      int instruction_pointer = getInstructionParameter(i + 2, instruction.parameter_modes.at(1));

      if (jump_condition != 0)
        i = instruction_pointer;
      else
        i += 3;
    }
    else if (instruction.opcode == 6)
    {
      // Jump to instruction if false
      int jump_condition = getInstructionParameter(i + 1, instruction.parameter_modes.at(0));
      int instruction_pointer = getInstructionParameter(i + 2, instruction.parameter_modes.at(1));

      if (jump_condition == 0)
        i = instruction_pointer;
      else
        i += 3;
    }
    else if (instruction.opcode == 7)
    {
      // Less than
      int operand1 = getInstructionParameter(i + 1, instruction.parameter_modes.at(0));
      int operand2 = getInstructionParameter(i + 2, instruction.parameter_modes.at(1));
      int destination = program_memory_[i + 3];

      if (operand1 < operand2)
        program_memory_[destination] = 1;
      else
        program_memory_[destination] = 0;

      i += 4;
    }
    else if (instruction.opcode == 8)
    {
      // Equals
      int operand1 = getInstructionParameter(i + 1, instruction.parameter_modes.at(0));
      int operand2 = getInstructionParameter(i + 2, instruction.parameter_modes.at(1));
      int destination = program_memory_[i + 3];

      if (operand1 == operand2)
        program_memory_[destination] = 1;
      else
        program_memory_[destination] = 0;

      i += 4;
    }
  }
}

int IntCodeComputer::getInstructionParameter(int index, int parameter_mode)
{
  if (parameter_mode == 0)
    return program_memory_[program_memory_[index]];
  else if (parameter_mode == 1)
    return program_memory_[index];
}
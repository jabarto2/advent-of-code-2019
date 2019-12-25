#include "IntCodeComputer.h"
#include "Instruction.h"

#include <iostream>
#include <thread>

IntCodeComputer::IntCodeComputer(std::vector<long> program_memory)
{
  original_program_memory_ = program_memory;
  program_memory_ = program_memory;
}

void IntCodeComputer::resetProgramMemory()
{
  program_memory_ = original_program_memory_;
  relative_base_offset_ = 0;
}

void IntCodeComputer::executeProgram(std::deque<long>& inputs, std::deque<long>& outputs)
{
  for (long i = 0; i < program_memory_.size();)
  {
    Instruction instruction(program_memory_.at(i));

    if (instruction.opcode == 99)
      return;
    else if (instruction.opcode == 1)
    {
      // Add
      long operand1 = readInstructionParameter(i + 1, instruction.parameter_modes.at(0));
      long operand2 = readInstructionParameter(i + 2, instruction.parameter_modes.at(1));
      long result = operand1 + operand2;

      writeInstructionResult(i + 3, result, instruction.parameter_modes.at(2));

      i += 4;
    }
    else if (instruction.opcode == 2)
    {
      // Multiply
      long operand1 = readInstructionParameter(i + 1, instruction.parameter_modes.at(0));
      long operand2 = readInstructionParameter(i + 2, instruction.parameter_modes.at(1));
      long result = operand1 * operand2;

      writeInstructionResult(i + 3, result, instruction.parameter_modes.at(2));

      i += 4;
    }
    else if (instruction.opcode == 3)
    {
      // take input and store at the operand address
      while (inputs.empty())
      {
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
      }
      long input = inputs.front();
      inputs.pop_front();

      writeInstructionResult(i + 1, input, instruction.parameter_modes.at(0));

      i += 2;
    }
    else if (instruction.opcode == 4)
    {
      // output the value at the operand address
      long output = readInstructionParameter(i + 1, instruction.parameter_modes.at(0));

      outputs.push_back(output);
      i += 2;
    }
    else if (instruction.opcode == 5)
    {
      // Jump to instruction if true
      long jump_condition = readInstructionParameter(i + 1, instruction.parameter_modes.at(0));
      long instruction_pointer = readInstructionParameter(i + 2, instruction.parameter_modes.at(1));

      if (jump_condition != 0)
        i = instruction_pointer;
      else
        i += 3;
    }
    else if (instruction.opcode == 6)
    {
      // Jump to instruction if false
      long jump_condition = readInstructionParameter(i + 1, instruction.parameter_modes.at(0));
      long instruction_pointer = readInstructionParameter(i + 2, instruction.parameter_modes.at(1));

      if (jump_condition == 0)
        i = instruction_pointer;
      else
        i += 3;
    }
    else if (instruction.opcode == 7)
    {
      // Less than
      long operand1 = readInstructionParameter(i + 1, instruction.parameter_modes.at(0));
      long operand2 = readInstructionParameter(i + 2, instruction.parameter_modes.at(1));
      long result;
      if (operand1 < operand2)
        result = 1;
      else
        result = 0;

      writeInstructionResult(i + 3, result, instruction.parameter_modes.at(2));

      i += 4;
    }
    else if (instruction.opcode == 8)
    {
      // Equals
      long operand1 = readInstructionParameter(i + 1, instruction.parameter_modes.at(0));
      long operand2 = readInstructionParameter(i + 2, instruction.parameter_modes.at(1));
      long result;
      if (operand1 == operand2)
        result = 1;
      else
        result = 0;

      writeInstructionResult(i + 3, result, instruction.parameter_modes.at(2));

      i += 4;
    }
    else if (instruction.opcode == 9)
    {
      // Relative Base Offset
      long offset = readInstructionParameter(i + 1, instruction.parameter_modes.at(0));
      relative_base_offset_ += offset;

      i += 2;
    }
  }
}

long IntCodeComputer::readInstructionParameter(long index, long parameter_mode)
{
  long raw_value = program_memory_[index];

  long parameter;
  if (parameter_mode == 0)
    parameter = program_memory_[raw_value];
  else if (parameter_mode == 1)
    parameter = raw_value;
  else if (parameter_mode == 2)
    parameter = program_memory_[relative_base_offset_ + raw_value];

  return parameter;
}

void IntCodeComputer::writeInstructionResult(long index, long result, long parameter_mode)
{
  long raw_value = program_memory_[index];

  long destination;
  if (parameter_mode == 0)
    destination = raw_value;
  else
    destination = relative_base_offset_ + raw_value;

  if (destination > program_memory_.size())
  {
    program_memory_.resize(2 * destination);
  }
  program_memory_[destination] = result;
}
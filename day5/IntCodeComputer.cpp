#include "IntCodeComputer.h"

#include <iostream>

IntCodeComputer::IntCodeComputer(std::vector<int> program_memory)
{
  original_program_memory_ = program_memory;
  program_memory_ = program_memory;
}

void IntCodeComputer::resetProgramMemory()
{
  program_memory_ = original_program_memory_;
}

void IntCodeComputer::executeProgram()
{
  for (int i = 0; i < program_memory_.size();)
  {
    int instruction = program_memory_.at(i);
    int opcode = instruction % 100;
    int parameter1_mode = instruction / 100 % 10;
    int parameter2_mode = instruction / 1000 % 10;
    int parameter3_mode = instruction / 10000 % 10;

    if (opcode == 99)
      return;
    else if (opcode == 1)
    {
      // Add
      int operand1;
      if (parameter1_mode == 0)
        operand1 = program_memory_[program_memory_[i + 1]];
      else if (parameter1_mode == 1)
        operand1 = program_memory_[i + 1];

      int operand2;
      if (parameter2_mode == 0)
        operand2 = program_memory_[program_memory_[i + 2]];
      else if (parameter2_mode == 1)
        operand2 = program_memory_[i + 2];

      int destination = program_memory_[i + 3];
      program_memory_[destination] = operand1 + operand2;
      i += 4;
    }
    else if (opcode == 2)
    {
      // Multiply
      int operand1;
      if (parameter1_mode == 0)
        operand1 = program_memory_[program_memory_[i + 1]];
      else if (parameter1_mode == 1)
        operand1 = program_memory_[i + 1];

      int operand2;
      if (parameter2_mode == 0)
        operand2 = program_memory_[program_memory_[i + 2]];
      else if (parameter2_mode == 1)
        operand2 = program_memory_[i + 2];

      int destination = program_memory_[i + 3];
      program_memory_[destination] = operand1 * operand2;
      i += 4;
    }
    else if (opcode == 3)
    {
      // take input and store at the operand address
      int input;
      std::cout << "Please provide an input to the program: ";
      std::cin >> input;
      program_memory_[program_memory_[i + 1]] = input;
      i += 2;
    }
    else if (opcode == 4)
    {
      // output the value at the operand address
      if (parameter1_mode == 0)
        std::cout << program_memory_[program_memory_[i + 1]] << std::endl;
      else if (parameter1_mode == 1)
        std::cout << program_memory_[i + 1] << std::endl;

      i += 2;
    }
    else if (opcode == 5)
    {
      // Jump to instruction if true
      int jump_condition;
      if (parameter1_mode == 0)
        jump_condition = program_memory_[program_memory_[i + 1]];
      else if (parameter1_mode == 1)
        jump_condition = program_memory_[i + 1];

      int instruction_pointer;
      if (parameter2_mode == 0)
        instruction_pointer = program_memory_[program_memory_[i + 2]];
      else if (parameter2_mode == 1)
        instruction_pointer = program_memory_[i + 2];

      if (jump_condition != 0)
        i = instruction_pointer;
      else
        i += 3;
    }
    else if (opcode == 6)
    {
      // Jump to instruction if false
      int jump_condition;
      if (parameter1_mode == 0)
        jump_condition = program_memory_[program_memory_[i + 1]];
      else if (parameter1_mode == 1)
        jump_condition = program_memory_[i + 1];

      int instruction_pointer;
      if (parameter2_mode == 0)
        instruction_pointer = program_memory_[program_memory_[i + 2]];
      else if (parameter2_mode == 1)
        instruction_pointer = program_memory_[i + 2];

      if (jump_condition == 0)
        i = instruction_pointer;
      else
        i += 3;
    }
    else if (opcode == 7)
    {
      // Less than
      int operand1;
      if (parameter1_mode == 0)
        operand1 = program_memory_[program_memory_[i + 1]];
      else if (parameter1_mode == 1)
        operand1 = program_memory_[i + 1];

      int operand2;
      if (parameter2_mode == 0)
        operand2 = program_memory_[program_memory_[i + 2]];
      else if (parameter2_mode == 1)
        operand2 = program_memory_[i + 2];

      int destination = program_memory_[i + 3];
      if (operand1 < operand2)
        program_memory_[destination] = 1;
      else
        program_memory_[destination] = 0;

      i += 4;
    }
    else if (opcode == 8)
    {
      // Equals
      int operand1;
      if (parameter1_mode == 0)
        operand1 = program_memory_[program_memory_[i + 1]];
      else if (parameter1_mode == 1)
        operand1 = program_memory_[i + 1];

      int operand2;
      if (parameter2_mode == 0)
        operand2 = program_memory_[program_memory_[i + 2]];
      else if (parameter2_mode == 1)
        operand2 = program_memory_[i + 2];

      int destination = program_memory_[i + 3];
      if (operand1 == operand2)
        program_memory_[destination] = 1;
      else
        program_memory_[destination] = 0;

      i += 4;
    }
  }
}
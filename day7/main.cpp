#include "../day5/IntCodeComputer.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <memory>
#include <algorithm>
#include <thread>

int runSimpleAmplifierCircuit(std::vector<int> phase_settings, std::vector<std::shared_ptr<IntCodeComputer>> amplifiers)
{
  int amplifier_output = 0;

  std::deque<int> inputs, outputs;

  std::cout << "Amplifier outputs: ";
  for (int i = 0; i < amplifiers.size(); ++i)
  {
    inputs.push_back(phase_settings.at(i));
    inputs.push_back(amplifier_output);
    amplifiers.at(i)->executeProgram(inputs, outputs);
    std::cout << outputs.back() << ", ";
    amplifier_output = outputs.back();
    outputs.clear();
  }
  std::cout << std::endl;
  return amplifier_output;
}

int runFeedbackLoopAmplifierCircuit(std::vector<int> phase_settings,
                                    std::vector<std::shared_ptr<IntCodeComputer>> amplifiers)
{
  int amplifier_output = 0;

  std::deque<int> inputs, outputs;

  std::cout << "Amplifier outputs: ";
  for (int i = 0; i < amplifiers.size(); ++i)
  {
    inputs.push_back(phase_settings.at(i));
    inputs.push_back(amplifier_output);

    std::thread t(&IntCodeComputer::executeProgram, amplifiers.at(i), inputs, outputs);

    amplifiers.at(i)->executeProgram(inputs, outputs);

    std::cout << outputs.back() << ", ";
    amplifier_output = outputs.back();
    outputs.clear();
  }
  std::cout << std::endl;
  return amplifier_output;
}

int main()
{
  std::string input_file = "/home/joe/projects/advent-of-code-2019/day7/input/amplifier_program.txt";
  std::ifstream file;
  file.open(input_file.c_str());

  std::vector<int> code;
  std::string word;
  while (std::getline(file, word, ','))
  {
    code.push_back(std::stoi(word));
  }

  // Part 1
  std::vector<int> phase_settings = { 0, 1, 2, 3, 4 };
  std::vector<std::vector<int>> phase_setting_permutations;
  do
  {
    phase_setting_permutations.push_back(phase_settings);
  } while (std::next_permutation(phase_settings.begin(), phase_settings.end()));

  std::vector<std::shared_ptr<IntCodeComputer>> amplifiers;

  for (auto& index : phase_settings)
  {
    amplifiers.push_back(std::make_shared<IntCodeComputer>(code));
  }

  int largest_amplifier_output = 0;

  for (auto& permutation : phase_setting_permutations)
  {
    int amplifier_output = runSimpleAmplifierCircuit(permutation, amplifiers);
    if (amplifier_output > largest_amplifier_output)
      largest_amplifier_output = amplifier_output;
  }

  std::cout << std::endl
            << "The largest amplifier output in the simple configuration was " << largest_amplifier_output << std::endl
            << std::endl;

  for (auto& amplifier : amplifiers)
  {
    amplifier->resetProgramMemory();
  }

  // Part 2
  phase_settings = { 5, 6, 7, 8, 9 };
  phase_setting_permutations.clear();

  do
  {
    phase_setting_permutations.push_back(phase_settings);
  } while (std::next_permutation(phase_settings.begin(), phase_settings.end()));

  for (auto& permutation : phase_setting_permutations)
  {
    int amplifier_output = runFeedbackLoopAmplifierCircuit(permutation, amplifiers);
    if (amplifier_output > largest_amplifier_output)
      largest_amplifier_output = amplifier_output;
  }

  return 0;
}

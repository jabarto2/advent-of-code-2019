#include <iostream>
#include <fstream>
#include <string>
#include <vector>

int main()
{
  // Part 1
  std::string input_file = "/home/joe/projects/advent-of-code-2019/day1/input/module_masses.txt";
  std::ifstream file;
  file.open(input_file.c_str());

  long module_mass;
  long required_fuel = 0;
  while (file >> module_mass)
  {
    required_fuel += (module_mass / 3) - 2;
  }
  std::cout << "Nominal fuel required: " << required_fuel << std::endl;

  // Part 2
  file.clear();
  file.seekg(0, std::ios::beg);
  required_fuel = 0;

  long module_fuel = 0;
  while (file >> module_mass)
  {
    module_fuel = (module_mass / 3) - 2;
    while (module_fuel > 0)
    {
      required_fuel += module_fuel;
      module_fuel = (module_fuel / 3) - 2;
    }
  }
  std::cout << "Total fuel required : " << required_fuel << std::endl;
  return 0;
}
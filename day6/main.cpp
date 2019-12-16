#include "Body.h"
#include "OrbitalMap.h"

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <memory>
#include <unordered_map>

int main()
{
  std::string input_file = "/home/joe/projects/advent-of-code-2019/day6/input/orbits.txt";
  std::ifstream file;
  file.open(input_file.c_str());

  OrbitalMap orbital_map;

  std::string orbit;
  while (std::getline(file, orbit))
  {
    auto delimiter_position = orbit.find(')');

    std::string parent_body_name = orbit.substr(0, delimiter_position);
    std::string child_body_name = orbit.substr(delimiter_position + 1);

    orbital_map.addOrbitToMap(parent_body_name, child_body_name);
  }

  // Part 1
  Body::Ptr center_of_mass = orbital_map.getCentralBody();
  std::cout << "This orbit map contains " << orbital_map.calculateNumberOfOrbits(center_of_mass, 0)
            << " direct and indirect orbits." << std::endl;

  // Part 2
  Body::Ptr you = orbital_map.getBody("YOU");
  Body::Ptr santa = orbital_map.getBody("SAN");
  std::cout << "You must make " << orbital_map.calculateOrbitalTransfers(you, santa)
            << " orbital transfers to reach Santa." << std::endl;

  return 0;
}
#ifndef ORBITAL_MAP_H_
#define ORBITAL_MAP_H_

#include "Body.h"

#include <string>
#include <unordered_map>

class OrbitalMap
{
public:
  void addOrbitToMap(std::string parent_name, std::string child_name);
  Body::Ptr getBody(std::string body_name);
  Body::Ptr getCentralBody();
  int calculateNumberOfOrbits(Body::Ptr body, int depth);
  int calculateOrbitalTransfers(Body::Ptr start, Body::Ptr finish);

private:
  std::vector<Body::Ptr> getAncestors(Body::Ptr body);

  std::unordered_map<std::string, Body::Ptr> bodies_;
};

#endif  // ORBITAL_MAP_H_
#include "OrbitalMap.h"

#include <algorithm>

void OrbitalMap::addOrbitToMap(std::string parent_name, std::string child_name)
{
  Body::Ptr parent_body = getBody(parent_name);
  Body::Ptr child_body = getBody(child_name);
  parent_body->addOrbitingChild(child_body);
  child_body->setParentBody(parent_body);
}

Body::Ptr OrbitalMap::getBody(std::string body_name)
{
  Body::Ptr body;
  if (bodies_.find(body_name) != bodies_.end())
  {
    body = bodies_.find(body_name)->second;
  }
  else
  {
    body = std::make_shared<Body>(body_name, nullptr);
    bodies_.insert({ body_name, body });
  }
  return body;
}

Body::Ptr OrbitalMap::getCentralBody()
{
  std::vector<Body::Ptr> parentless_bodies;
  for (auto& entry : bodies_)
  {
    if (entry.second->getParentBody() == nullptr)
    {
      parentless_bodies.push_back(entry.second);
    }
  }
  if (parentless_bodies.size() > 1)
    throw std::invalid_argument("Too many bodies have no parent.");
  return parentless_bodies.front();
}

int OrbitalMap::calculateNumberOfOrbits(Body::Ptr body, int depth)
{
  int child_orbits = 0;
  if (body->getChildrenBodies().size() == 0)
  {
    return depth;
  }
  for (auto& child : body->getChildrenBodies())
  {
    child_orbits += calculateNumberOfOrbits(child, depth + 1);
  }
  return child_orbits + depth;
}

int OrbitalMap::calculateOrbitalTransfers(Body::Ptr start, Body::Ptr finish)
{
  std::vector<Body::Ptr> starting_body_ancestors = getAncestors(start);
  std::vector<Body::Ptr> finishing_body_ancestors = getAncestors(finish);

  int orbital_transfers = 0;

  for (auto& ancestor : starting_body_ancestors)
  {
    auto common_ancestor = std::find(finishing_body_ancestors.begin(), finishing_body_ancestors.end(), ancestor);
    if (common_ancestor != finishing_body_ancestors.end())
    {
      return orbital_transfers + (common_ancestor - finishing_body_ancestors.begin());
    }
    else
    {
      orbital_transfers++;
    }
  }
}

std::vector<Body::Ptr> OrbitalMap::getAncestors(Body::Ptr body)
{
  std::vector<Body::Ptr> ancestors;
  Body::Ptr current_body = body->getParentBody();
  while (current_body != nullptr)
  {
    ancestors.push_back(current_body);
    current_body = current_body->getParentBody();
  }
  return ancestors;
}
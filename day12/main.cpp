#include <iostream>
#include <vector>
#include <memory>
#include <cmath>
#include <algorithm>

struct Position
{
  Position(int x, int y, int z) : x(x), y(y), z(z)
  {
  }

  void set(int x, int y, int z)
  {
    this->x = x;
    this->y = y;
    this->z = z;
  }
  int x;
  int y;
  int z;
};

struct Velocity
{
  Velocity(int dx, int dy, int dz) : dx(dx), dy(dy), dz(dz)
  {
  }
  int dx;
  int dy;
  int dz;
};

struct Acceleration
{
  Acceleration(int ddx, int ddy, int ddz) : ddx(ddx), ddy(ddy), ddz(ddz)
  {
  }

  int ddx;
  int ddy;
  int ddz;
};

class Moon
{
public:
  typedef std::shared_ptr<Moon> Ptr;
  Moon(Position position, Velocity velocity) : position_(position), velocity_(velocity)
  {
  }

  void applySingleTimestep()
  {
    position_.x += velocity_.dx;
    position_.y += velocity_.dy;
    position_.z += velocity_.dz;
  }

  Position getPosition()
  {
    return position_;
  }

  Velocity getVelocity()
  {
    return velocity_;
  }

  void setPosition(Position position)
  {
    position_ = position;
  }

  void setVelocity(Velocity velocity)
  {
    velocity_ = velocity;
  }

private:
  Position position_;
  Velocity velocity_;
};

std::vector<Moon::Ptr> initializeMoons(std::vector<Position> initial_positions,
                                       std::vector<Velocity> initial_velocities)
{
  if (initial_positions.size() != initial_velocities.size())
    throw std::invalid_argument("The number of initial positions must match the number of initial velocities.");

  std::vector<Moon::Ptr> moons;
  for (unsigned int i = 0; i < initial_positions.size(); ++i)
  {
    Position initial_position = initial_positions[i];
    Velocity initial_velocity = initial_velocities[i];
    Moon::Ptr moon = std::make_shared<Moon>(initial_position, initial_velocity);
    moons.push_back(moon);
  }

  return moons;
}

Acceleration applyGravity(Moon::Ptr moon1, Moon::Ptr moon2)
{
  Position moon1_position = moon1->getPosition();
  Position moon2_position = moon2->getPosition();

  int x_acceleration = 0;
  int y_acceleration = 0;
  int z_acceleration = 0;
  if (moon1_position.x > moon2_position.x)
    x_acceleration = -1;
  else if (moon1_position.x < moon2_position.x)
    x_acceleration = 1;
  if (moon1_position.y > moon2_position.y)
    y_acceleration = -1;
  else if (moon1_position.y < moon2_position.y)
    y_acceleration = 1;
  if (moon1_position.z > moon2_position.z)
    z_acceleration = -1;
  else if (moon1_position.z < moon2_position.z)
    z_acceleration = 1;

  return Acceleration(x_acceleration, y_acceleration, z_acceleration);
}

long gcd(long a, long b)
{
  if (a == 0)
    return b;
  return gcd(b % a, a);
}

long lcm(long a, long b)
{
  return (a * b) / gcd(a, b);
}

/*
<x=17, y=-12, z=13>
<x=2, y=1, z=1>
<x=-1, y=-17, z=7>
<x=12, y=-14, z=18>
*/
int main()
{
  std::vector<Position> initial_positions = { { 17, -12, 13 }, { 2, 1, 1 }, { -1, -17, 7 }, { 12, -14, 18 } };
  std::vector<Velocity> initial_velocities = { 4, { 0, 0, 0 } };
  std::vector<Moon::Ptr> moons = initializeMoons(initial_positions, initial_velocities);
  for (unsigned int i = 0; i < 1000; ++i)
  {
    for (auto first = moons.begin(); first != moons.end(); ++first)
    {
      for (auto second = first + 1; second != moons.end(); ++second)
      {
        auto& moon1 = *first;
        auto& moon2 = *second;
        Acceleration moon1_acceleration = applyGravity(moon1, moon2);

        int new_moon1_dx = moon1->getVelocity().dx + moon1_acceleration.ddx;
        int new_moon1_dy = moon1->getVelocity().dy + moon1_acceleration.ddy;
        int new_moon1_dz = moon1->getVelocity().dz + moon1_acceleration.ddz;

        int new_moon2_dx = moon2->getVelocity().dx - moon1_acceleration.ddx;
        int new_moon2_dy = moon2->getVelocity().dy - moon1_acceleration.ddy;
        int new_moon2_dz = moon2->getVelocity().dz - moon1_acceleration.ddz;

        Velocity new_moon1_velocity(new_moon1_dx, new_moon1_dy, new_moon1_dz);
        moon1->setVelocity(new_moon1_velocity);
        Velocity new_moon2_velocity(new_moon2_dx, new_moon2_dy, new_moon2_dz);
        moon2->setVelocity(new_moon2_velocity);
      }
    }
    for (auto& moon : moons)
    {
      moon->applySingleTimestep();
    }
  }

  int total_energy_of_moons = 0;
  for (auto& moon : moons)
  {
    Position position = moon->getPosition();
    int potential_energy = std::abs(position.x) + std::abs(position.y) + std::abs(position.z);

    Velocity velocity = moon->getVelocity();
    int kinetic_energy = std::abs(velocity.dx) + std::abs(velocity.dy) + std::abs(velocity.dz);

    total_energy_of_moons += (potential_energy * kinetic_energy);
  }

  std::cout << "After 1000 timesteps, the total energy of the moons is " << total_energy_of_moons << std::endl;

  // Part 2
  moons = initializeMoons(initial_positions, initial_velocities);

  std::vector<std::pair<int, int>> initial_x_state, initial_y_state, initial_z_state;
  for (unsigned int i = 0; i < initial_positions.size(); ++i)
  {
    initial_x_state.push_back({ initial_positions[i].x, initial_velocities[i].dx });
    initial_y_state.push_back({ initial_positions[i].y, initial_velocities[i].dy });
    initial_z_state.push_back({ initial_positions[i].z, initial_velocities[i].dz });
  }

  int x_cycle, y_cycle, z_cycle;
  bool x_cycle_found = false;
  bool y_cycle_found = false;
  bool z_cycle_found = false;
  int timestep = 1;
  while (true)
  {
    for (auto first = moons.begin(); first != moons.end(); ++first)
    {
      for (auto second = first + 1; second != moons.end(); ++second)
      {
        auto& moon1 = *first;
        auto& moon2 = *second;
        Acceleration moon1_acceleration = applyGravity(moon1, moon2);

        int new_moon1_dx = moon1->getVelocity().dx + moon1_acceleration.ddx;
        int new_moon1_dy = moon1->getVelocity().dy + moon1_acceleration.ddy;
        int new_moon1_dz = moon1->getVelocity().dz + moon1_acceleration.ddz;

        int new_moon2_dx = moon2->getVelocity().dx - moon1_acceleration.ddx;
        int new_moon2_dy = moon2->getVelocity().dy - moon1_acceleration.ddy;
        int new_moon2_dz = moon2->getVelocity().dz - moon1_acceleration.ddz;

        Velocity new_moon1_velocity(new_moon1_dx, new_moon1_dy, new_moon1_dz);
        moon1->setVelocity(new_moon1_velocity);
        Velocity new_moon2_velocity(new_moon2_dx, new_moon2_dy, new_moon2_dz);
        moon2->setVelocity(new_moon2_velocity);
      }
    }
    std::vector<std::pair<int, int>> current_x_state, current_y_state, current_z_state;
    for (auto& moon : moons)
    {
      moon->applySingleTimestep();

      current_x_state.push_back({ moon->getPosition().x, moon->getVelocity().dx });
      current_y_state.push_back({ moon->getPosition().y, moon->getVelocity().dy });
      current_z_state.push_back({ moon->getPosition().z, moon->getVelocity().dz });
    }
    if (!x_cycle_found && current_x_state == initial_x_state)
    {
      std::cout << "X state repeats every " << timestep << " timesteps" << std::endl;
      x_cycle = timestep;
      x_cycle_found = true;
    }
    if (!y_cycle_found && current_y_state == initial_y_state)
    {
      std::cout << "Y state repeats every " << timestep << " timesteps" << std::endl;
      y_cycle = timestep;
      y_cycle_found = true;
    }
    if (!z_cycle_found && current_z_state == initial_z_state)
    {
      std::cout << "Z state repeats every " << timestep << " timesteps" << std::endl;
      z_cycle = timestep;
      z_cycle_found = true;
    }
    if (x_cycle_found && y_cycle_found && z_cycle_found)
      break;
    timestep++;
  }

  long cycle = lcm(x_cycle, y_cycle);
  cycle = lcm(cycle, z_cycle);

  std::cout << "The state of the moons will repeate every " << cycle << " timesteps" << std::endl;

  return 0;
}
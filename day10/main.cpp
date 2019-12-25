#include <iostream>
#include <fstream>
#include <vector>
#include <set>
#include <list>
#include <limits>
#include <cmath>
#include <algorithm>

struct Asteroid
{
  Asteroid(int x, int y) : x(x), y(y)
  {
  }
  bool operator==(const Asteroid& rhs) const
  {
    if (this->x == rhs.x && this->y == rhs.y)
      return true;
    return false;
  }

  void operator=(const Asteroid& rhs)
  {
    this->x = rhs.x;
    this->y = rhs.y;
  }

  static double distanceBetweenTwoAsteroids(Asteroid a, Asteroid b)
  {
    return sqrt((a.x - b.x) * (a.x - b.x) + (a.y - b.y) * (a.y - b.y));
  }

  int x;
  int y;
};

class LineOfSight
{
public:
  enum Direction
  {
    LEFT,
    RIGHT,
    UP,
    DOWN,
    DIAGONAL_RIGHT,
    DIAGONAL_LEFT
  };
  LineOfSight(Asteroid a, Asteroid b)
  {
    int dx = b.x - a.x;
    int dy = b.y - a.y;
    if (dx == 0)
    {
      slope_ = std::numeric_limits<int>::max();
      if (dy < 0)
        direction_ = Direction::UP;
      else
        direction_ = Direction::DOWN;
    }
    else if (dy == 0)
    {
      slope_ = 0;
      if (dx < 0)
        direction_ = Direction::LEFT;
      else
        direction_ = Direction::RIGHT;
    }
    else
    {
      slope_ = (double)dy / (double)dx;
      if (dx > 0)
        direction_ = Direction::DIAGONAL_RIGHT;
      else
        direction_ = Direction::DIAGONAL_LEFT;
    }
  }

  bool operator==(const LineOfSight& rhs) const
  {
    if (this->slope_ == rhs.slope_ && this->direction_ == rhs.direction_)
      return true;
    return false;
  }

private:
  double slope_;
  Direction direction_;
};

int main()
{
  std::string input_file = "/home/joe/projects/advent-of-code-2019/day10/input/asteroid_map.txt";
  std::ifstream file;
  file.open(input_file.c_str());

  char asteroid = '#';
  char empty = '.';

  std::vector<Asteroid> asteroids;

  std::string row;
  int x = 0;
  int y = 0;
  while (std::getline(file, row))
  {
    for (auto& space : row)
    {
      if (space == asteroid)
        asteroids.push_back({ x, y });
      x++;
    }
    y++;
    x = 0;
  }

  Asteroid station_location(0, 0);
  std::list<Asteroid> asteroids_visible_from_station;
  for (auto& asteroid1 : asteroids)
  {
    std::list<Asteroid> visible_asteroids;
    for (auto& asteroid2 : asteroids)
    {
      if (asteroid1 == asteroid2)
        continue;

      LineOfSight line_of_sight(asteroid1, asteroid2);
      Asteroid* current_visible_asteroid = nullptr;
      for (auto& visible_asteroid : visible_asteroids)
      {
        if (line_of_sight == LineOfSight(asteroid1, visible_asteroid))
        {
          current_visible_asteroid = &visible_asteroid;
        }
      }
      if (current_visible_asteroid == nullptr)
      {
        visible_asteroids.push_back(asteroid2);
      }
      else
      {
        double curr_distance = Asteroid::distanceBetweenTwoAsteroids(asteroid1, *current_visible_asteroid);
        double new_distance = Asteroid::distanceBetweenTwoAsteroids(asteroid1, asteroid2);
        if (new_distance < curr_distance)
        {
          visible_asteroids.remove(*current_visible_asteroid);
          visible_asteroids.push_back(asteroid2);
        }
      }
    }
    if (visible_asteroids.size() > asteroids_visible_from_station.size())
    {
      station_location = asteroid1;
      asteroids_visible_from_station = visible_asteroids;
    }
  }

  std::cout << "The best asteroid location at (" << station_location.x << "," << station_location.y << ") can see "
            << asteroids_visible_from_station.size() << " other asteroids" << std::endl;

  // Part 2

  std::vector<Asteroid> vaporization_order;
  std::vector<std::pair<double, Asteroid>> theta_values;
  for (auto& asteroid : asteroids_visible_from_station)
  {
    double dx = asteroid.x - station_location.x;
    double dy = asteroid.y - station_location.y;
    double theta;
    if (dx >= 0 && dy < 0)
      theta = std::atan(dx / (-1 * dy));
    if (dx > 0 && dy >= 0)
      theta = (M_PI / 2) + std::atan(dy / dx);
    if (dx <= 0 && dy > 0)
      theta = M_PI + std::atan((-1 * dx) / dy);
    if (dx < 0 && dy <= 0)
      theta = (3 * M_PI / 2) + std::atan((-1 * dy) / (-1 * dx));
    theta_values.push_back({ theta, asteroid });
  }
  std::sort(
      theta_values.begin(), theta_values.end(),
      [](const std::pair<double, Asteroid>& a, const std::pair<double, Asteroid>& b) { return a.first < b.first; });

  std::cout << "The 200th asteroid to be vaporized will be the one at (" << theta_values.at(199).second.x << ","
            << theta_values.at(199).second.y << ")" << std::endl;

  return 0;
}
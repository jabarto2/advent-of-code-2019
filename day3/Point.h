#ifndef POINT_H_
#define POINT_H_

#include <algorithm>

struct Point
{
  Point(int x = 0, int y = 0) : x(x), y(y)
  {
  }

  bool operator==(const Point& point)
  {
    if (x == point.x && y == point.y)
      return true;
    else
      return false;
  }
  bool operator!=(const Point& point)
  {
    return !(*this == point);
  }
  static int getManhattanDistanceBetweenPoints(const Point& a, const Point& b)
  {
    return std::abs(a.x - b.x) + std::abs(a.y - b.y);
  }
  int x;
  int y;
};

#endif
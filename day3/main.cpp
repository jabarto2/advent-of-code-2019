#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include <algorithm>
#include <limits>
#include <ctime>

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
  int x;
  int y;
};

struct Segment
{
  enum SegmentType
  {
    HORIZONTAL,
    VERTICAL
  };

  Segment(Point previous_point, std::string movement)
  {
    char movement_direction = movement.front();
    int movement_magnitude = std::stoi(movement.substr(1));
    switch (movement.front())
    {
      case 'U':
        start_point = previous_point;
        end_point = Point(previous_point.x, previous_point.y + movement_magnitude);
        type = SegmentType::VERTICAL;
        break;
      case 'D':
        start_point = previous_point;
        end_point = Point(previous_point.x, previous_point.y - movement_magnitude);
        type = SegmentType::VERTICAL;
        break;
      case 'L':
        start_point = previous_point;
        end_point = Point(previous_point.x - movement_magnitude, previous_point.y);
        type = SegmentType::HORIZONTAL;
        break;
      case 'R':
        start_point = previous_point;
        end_point = Point(previous_point.x + movement_magnitude, previous_point.y);
        type = SegmentType::HORIZONTAL;
        break;
    }
  }

  Point start_point;
  Point end_point;
  SegmentType type;
};

std::pair<int, int> getXBoundariesOfSegment(const Segment& segment)
{
  // Rightward segment movement
  if (segment.start_point.x < segment.end_point.x)
    return { segment.start_point.x, segment.end_point.x };

  // Leftward segment movement
  else
    return { segment.end_point.x, segment.start_point.x };
}

std::pair<int, int> getYBoundariesOfSegment(const Segment& segment)
{
  // Upward segment movement
  if (segment.start_point.y < segment.end_point.y)
    return { segment.start_point.y, segment.end_point.y };

  // Downward segment movement
  else
    return { segment.end_point.y, segment.start_point.y };
}

bool valueIsWithinBounds(int lower_bound, int upper_bound, int value)
{
  if ((value > lower_bound) && (value < upper_bound))
    return true;
  else
    return false;
}

int determineIfPerpendicularSegmentsIntersect()
{
}

int determineIfSegmentsIntersect(Segment segment1, Segment segment2)
{
  std::pair<int, int> segment1_x_boundaries = getXBoundariesOfSegment(segment1);
  std::pair<int, int> segment1_y_boundaries = getYBoundariesOfSegment(segment1);
  std::pair<int, int> segment2_x_boundaries = getXBoundariesOfSegment(segment2);
  std::pair<int, int> segment2_y_boundaries = getYBoundariesOfSegment(segment2);

  if (segment1.type == segment2.type)  // Parallel segments can't intersect
    return std::numeric_limits<int>::max();

  int manhattan_distance_to_intersection = std::numeric_limits<int>::max();

  if (segment1.type == Segment::SegmentType::HORIZONTAL)
  {
    int segment1_y_value = segment1_y_boundaries.first;
    int segment2_x_value = segment2_x_boundaries.first;

    if (valueIsWithinBounds(segment2_y_boundaries.first, segment2_y_boundaries.second, segment1_y_value))
    {
      if (valueIsWithinBounds(segment1_x_boundaries.first, segment1_x_boundaries.second, segment2_x_value))
      {
        std::cout << "Wires intersect at point (" << segment2_x_value << "," << segment1_y_value << ")" << std::endl;
        manhattan_distance_to_intersection = std::abs(segment2_x_value) + std::abs(segment1_y_value);
      }
    }
  }
  else if (segment1.type == Segment::SegmentType::VERTICAL)
  {
    int segment1_x_value = segment1_x_boundaries.first;
    int segment2_y_value = segment2_y_boundaries.first;

    if (valueIsWithinBounds(segment2_x_boundaries.first, segment2_x_boundaries.second, segment1_x_value))
    {
      if (valueIsWithinBounds(segment1_y_boundaries.first, segment1_y_boundaries.second, segment2_y_value))
      {
        std::cout << "Wires intersect at point (" << segment1_x_value << "," << segment2_y_value << ")" << std::endl;
        manhattan_distance_to_intersection = std::abs(segment1_x_value) + std::abs(segment2_y_value);
      }
    }
  }
}

int main()
{
  std::clock_t start;
  double duration;
  start = std::clock();

  std::string input_file = "/home/joe/projects/advent-of-code-2019/day3/input/wire_paths.txt";
  std::ifstream file;
  file.open(input_file.c_str());

  std::string wire1;
  std::getline(file, wire1);
  std::stringstream wire1_stream(wire1);

  Point central_point(0, 0);
  Point previous_point = central_point;

  std::vector<Segment> wire1_segments;
  std::string movement;
  while (std::getline(wire1_stream, movement, ','))
  {
    Segment new_segment(previous_point, movement);
    wire1_segments.push_back(new_segment);
    previous_point = new_segment.end_point;
  }

  std::string wire2;
  std::getline(file, wire2);
  std::stringstream wire2_stream(wire2);

  previous_point = central_point;

  std::vector<Segment> wire2_segments;
  while (std::getline(wire2_stream, movement, ','))
  {
    Segment new_segment(previous_point, movement);
    wire2_segments.push_back(new_segment);
    previous_point = new_segment.end_point;
  }

  int closest_intersection_distance = std::numeric_limits<int>::max();
  for (auto& segment1 : wire1_segments)
  {
    for (auto& segment2 : wire2_segments)
    {
      int manhattan_distance_to_intersection = determineIfSegmentsIntersect(segment1, segment2);
      if (manhattan_distance_to_intersection < closest_intersection_distance)
        closest_intersection_distance = manhattan_distance_to_intersection;
    }
  }

  std::cout << "Closest intersection distance to central point: " << closest_intersection_distance << std::endl;
  duration = (std::clock() - start) / (double)CLOCKS_PER_SEC;
  std::cout << "Computation time: " << duration << std::endl;
  return 0;
}
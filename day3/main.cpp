#include "Point.h"
#include "Segment.h"

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include <limits>

std::vector<Segment> getSegmentsFromWireStream(std::stringstream& wire_stream,
                                               const Point& starting_point = Point(0, 0))
{
  Point previous_point = starting_point;
  std::vector<Segment> wire_segments;

  std::string movement;
  while (std::getline(wire_stream, movement, ','))
  {
    Segment new_segment(previous_point, movement);
    wire_segments.push_back(new_segment);
    previous_point = new_segment.end_point;
  }

  return wire_segments;
}

int main()
{
  Point origin(0, 0);

  std::string input_file = "/home/joe/projects/advent-of-code-2019/day3/input/wire_paths.txt";
  std::ifstream file;
  file.open(input_file.c_str());

  std::string wire1;
  std::getline(file, wire1);
  std::stringstream wire1_stream(wire1);

  std::vector<Segment> wire1_segments = getSegmentsFromWireStream(wire1_stream, origin);

  std::string wire2;
  std::getline(file, wire2);
  std::stringstream wire2_stream(wire2);

  std::vector<Segment> wire2_segments = getSegmentsFromWireStream(wire2_stream, origin);

  int closest_intersection_to_origin_distance = std::numeric_limits<int>::max();     // Part 1
  int shortest_combined_distance_to_intersection = std::numeric_limits<int>::max();  // Part 2
  int wire1_length_traveled = 0;
  int wire2_length_traveled = 0;
  for (auto& segment1 : wire1_segments)
  {
    for (auto& segment2 : wire2_segments)
    {
      Point intersection;
      try
      {
        intersection = Segment::getPointOfSegmentIntersection(segment1, segment2);
      }
      catch (std::invalid_argument)
      {
        wire2_length_traveled += segment2.length;
        continue;
      }

      std::cout << "Wires intersect at point (" << intersection.x << "," << intersection.y << ")" << std::endl;

      int intersection_distance_to_origin = Point::getManhattanDistanceBetweenPoints(intersection, origin);

      if (intersection_distance_to_origin < closest_intersection_to_origin_distance)
        closest_intersection_to_origin_distance = intersection_distance_to_origin;

      int partial_segment1_distance = Point::getManhattanDistanceBetweenPoints(intersection, segment1.start_point);
      int partial_segment2_distance = Point::getManhattanDistanceBetweenPoints(intersection, segment2.start_point);

      int combined_length_traveled =
          wire1_length_traveled + partial_segment1_distance + wire2_length_traveled + partial_segment2_distance;

      if (combined_length_traveled < shortest_combined_distance_to_intersection)
        shortest_combined_distance_to_intersection = combined_length_traveled;
      wire2_length_traveled += segment2.length;
    }
    wire1_length_traveled += segment1.length;
    wire2_length_traveled = 0;
  }

  std::cout << "Closest intersection distance to origin: " << closest_intersection_to_origin_distance << std::endl;
  std::cout << "The closest intersection when measuring distance along both wires is a combined "
            << shortest_combined_distance_to_intersection << " steps from the origin" << std::endl;
  return 0;
}
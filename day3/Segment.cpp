#include "Segment.h"

#include <stdexcept>

Segment::Segment(Point previous_point, std::string movement)
{
  char movement_direction = movement.front();
  int movement_magnitude = std::stoi(movement.substr(1));
  length = movement_magnitude;
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

Point Segment::getPointOfSegmentIntersection(const Segment& segment1, const Segment& segment2)
{
  Point intersection_point;
  bool intersection_found = false;

  std::pair<int, int> segment1_x_boundaries = segment1.getXBoundariesOfSegment();
  std::pair<int, int> segment1_y_boundaries = segment1.getYBoundariesOfSegment();
  std::pair<int, int> segment2_x_boundaries = segment2.getXBoundariesOfSegment();
  std::pair<int, int> segment2_y_boundaries = segment2.getYBoundariesOfSegment();

  if (segment1.type == segment2.type)  // Parallel segments can't intersect
    throw std::invalid_argument("Parallel segments will never intersect");

  if (segment1.type == Segment::SegmentType::HORIZONTAL)
  {
    int segment1_y_value = segment1_y_boundaries.first;
    int segment2_x_value = segment2_x_boundaries.first;

    if (valueIsWithinBounds(segment2_y_boundaries.first, segment2_y_boundaries.second, segment1_y_value))
    {
      if (valueIsWithinBounds(segment1_x_boundaries.first, segment1_x_boundaries.second, segment2_x_value))
      {
        intersection_point = Point(segment2_x_value, segment1_y_value);
        intersection_found = true;
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
        intersection_point = Point(segment1_x_value, segment2_y_value);
        intersection_found = true;
      }
    }
  }

  if (intersection_found)
    return intersection_point;
  else
    throw std::invalid_argument("Segments do not intersect");
}

std::pair<int, int> Segment::getXBoundariesOfSegment() const
{
  // Rightward segment movement
  if (start_point.x < end_point.x)
    return { start_point.x, end_point.x };

  // Leftward segment movement
  else
    return { end_point.x, start_point.x };
}

std::pair<int, int> Segment::getYBoundariesOfSegment() const
{
  // Upward segment movement
  if (start_point.y < end_point.y)
    return { start_point.y, end_point.y };

  // Downward segment movement
  else
    return { end_point.y, start_point.y };
}

bool Segment::valueIsWithinBounds(int lower_bound, int upper_bound, int value)
{
  if ((value > lower_bound) && (value < upper_bound))
    return true;
  else
    return false;
}
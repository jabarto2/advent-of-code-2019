#ifndef SEGMENT_H_
#define SEGMENT_H_

#include "Point.h"

#include <string>

class Segment
{
public:
  enum SegmentType
  {
    HORIZONTAL,
    VERTICAL
  };

  Point start_point;
  Point end_point;
  int length;
  SegmentType type;

  Segment(Point previous_point, std::string movement);

  static Point getPointOfSegmentIntersection(const Segment& segment1, const Segment& segment2);

  std::pair<int, int> getXBoundariesOfSegment() const;

  std::pair<int, int> getYBoundariesOfSegment() const;

private:
  static bool valueIsWithinBounds(int lower_bound, int upper_bound, int value);
};

#endif
#include "../day5/IntCodeComputer.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <memory>
#include <algorithm>
#include <thread>
#include <future>

struct HullSquare
{
  enum Color
  {
    BLACK,
    WHITE
  };

  HullSquare(int x, int y, Color color = Color::BLACK) : x(x), y(y), color(color)
  {
  }

  int x;
  int y;
  Color color;
};

class HullPaintingRobot
{
  enum Direction
  {
    UP,
    DOWN,
    LEFT,
    RIGHT
  };

public:
  HullPaintingRobot() : direction_(Direction::UP), unique_squares_painted_(0)
  {
    current_square_ = std::make_shared<HullSquare>(0, 0, HullSquare::Color::BLACK);
  }

  std::shared_ptr<HullSquare> findNextSquare(int x, int y)
  {
    for (auto& square : visited_squares_)
    {
      if (square->x == x && square->y == y)
        return square;
    }
    return std::make_shared<HullSquare>(x, y, HullSquare::Color::BLACK);
  }

  void turnLeft()
  {
    std::shared_ptr<HullSquare> next_square;
    switch (direction_)
    {
      case UP:
        direction_ = LEFT;
        next_square = findNextSquare(current_square_->x - 1, current_square_->y);
        break;
      case DOWN:
        direction_ = RIGHT;
        next_square = findNextSquare(current_square_->x + 1, current_square_->y);
        break;
      case LEFT:
        direction_ = DOWN;
        next_square = findNextSquare(current_square_->x, current_square_->y - 1);
        break;
      case RIGHT:
        direction_ = UP;
        next_square = findNextSquare(current_square_->x, current_square_->y + 1);
        break;
    }

    visited_squares_.push_back(current_square_);
    current_square_ = next_square;
  }
  void turnRight()
  {
    std::shared_ptr<HullSquare> next_square;
    switch (direction_)
    {
      case UP:
        direction_ = RIGHT;
        next_square = findNextSquare(current_square_->x + 1, current_square_->y);
        break;
      case DOWN:
        direction_ = LEFT;
        next_square = findNextSquare(current_square_->x - 1, current_square_->y);
        break;
      case LEFT:
        direction_ = UP;
        next_square = findNextSquare(current_square_->x, current_square_->y + 1);
        break;
      case RIGHT:
        direction_ = DOWN;
        next_square = findNextSquare(current_square_->x, current_square_->y - 1);
        break;
    }

    visited_squares_.push_back(current_square_);
    current_square_ = next_square;
  }

  void paintSquare(HullSquare::Color color)
  {
    current_square_->color = color;
    unique_squares_painted_++;
    if (std::find(visited_squares_.begin(), visited_squares_.end(), current_square_) != visited_squares_.end())
      unique_squares_painted_--;
  }

  HullSquare::Color getColorOfCurrentSquare()
  {
    return current_square_->color;
  }

  long getNumberOfUniqueSquaresPainted()
  {
    return unique_squares_painted_;
  }

  void reset()
  {
    direction_ = Direction::UP;
    current_square_ = std::make_shared<HullSquare>(0, 0, HullSquare::Color::BLACK);
    visited_squares_.clear();
    unique_squares_painted_ = 0;
  }

  std::vector<std::shared_ptr<HullSquare>> getVisitedSquares()
  {
    return visited_squares_;
  }

private:
  Direction direction_;
  std::shared_ptr<HullSquare> current_square_;
  std::vector<std::shared_ptr<HullSquare>> visited_squares_;
  long unique_squares_painted_;
};

void executeHullPaintingProgram(HullPaintingRobot& robot, IntCodeComputer& computer, long start_value)
{
  std::deque<long> input;
  input.push_back(start_value);
  std::deque<long> output;
  auto future =
      std::async(std::launch::async, &IntCodeComputer::executeProgram, computer, std::ref(input), std::ref(output));

  while (future.wait_for(std::chrono::milliseconds(0)) != std::future_status::ready)
  {
    while (output.size() < 2)
    {
      std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }
    long paint_color = output.front();
    output.pop_front();
    long turn_direction = output.front();
    output.pop_front();

    robot.paintSquare((HullSquare::Color)paint_color);
    if (turn_direction == 0)
      robot.turnLeft();
    else
      robot.turnRight();
    input.push_back((int)robot.getColorOfCurrentSquare());
  }
}

int main()
{
  std::string input_file = "/home/joe/projects/advent-of-code-2019/day11/input/hull_painting_robot_program.txt";
  std::ifstream file;
  file.open(input_file.c_str());

  std::vector<long> code;
  std::string word;
  while (std::getline(file, word, ','))
  {
    code.push_back(std::stol(word));
  }

  IntCodeComputer computer(code);

  // Part 1
  HullPaintingRobot robot;

  executeHullPaintingProgram(robot, computer, 0);

  std::cout << "The robot visited and painted " << robot.getNumberOfUniqueSquaresPainted() << " unique squares."
            << std::endl;

  // Part2
  robot.reset();
  computer.resetProgramMemory();
  executeHullPaintingProgram(robot, computer, 1);

  std::cout << "The robot visited and painted " << robot.getNumberOfUniqueSquaresPainted() << " unique squares."
            << std::endl;

  // std::vector<std::vector<int>> decoded_image = image.decodeImage();
  int min_x = std::numeric_limits<int>::max();
  int max_x = std::numeric_limits<int>::min();
  int min_y = std::numeric_limits<int>::max();
  int max_y = std::numeric_limits<int>::min();
  for (auto& square : robot.getVisitedSquares())
  {
    if (square->x < min_x)
      min_x = square->x;
    if (square->x > max_x)
      max_x = square->x;
    if (square->y < min_y)
      min_y = square->y;
    if (square->y > max_y)
      max_y = square->y;
  }
  int x_size = max_x - min_x + 1;
  int y_size = max_y - min_y + 1;
  std::vector<int> column(max_y - min_y + 1, 0);
  std::vector<std::vector<int>> registration(max_x - min_x + 1, column);
  for (auto& square : robot.getVisitedSquares())
  {
    registration[square->x - min_x][square->y - min_y] = (int)square->color;
  }
  char black = 32;
  char white = 35;
  for (int y = registration.front().size() - 1; y >= 0; --y)
  {
    for (int x = 0; x < registration.size(); ++x)
    {
      if (registration[x][y] == 0)
        std::cout << black;
      else if (registration[x][y] == 1)
        std::cout << white;
    }
    std::cout << std::endl;
  }
  return 0;
}

#include <iostream>
#include <fstream>
#include <vector>
#include <limits>

struct Pixel
{
  std::vector<int> layers;
};

class SpaceImage
{
public:
  SpaceImage(int width, int height)
  {
    pixels_.resize(width);
    for (auto& column : pixels_)
    {
      column.resize(height);
    }
  }

  void addLayer(int x, int y, int value)
  {
    Pixel& pixel = pixels_[x][y];
    pixel.layers.push_back(value);
  }

  int getNumberOfLayers()
  {
    return pixels_.front().front().layers.size();
  }

  std::vector<int> getImageLayer(int layer_index)
  {
    std::vector<int> layer;
    for (auto& column : pixels_)
    {
      for (auto& row : column)
      {
        layer.push_back(row.layers.at(layer_index));
      }
    }
    return layer;
  }

  std::vector<std::vector<int>> decodeImage()
  {
    std::vector<std::vector<int>> decoded_image;
    for (auto& column : pixels_)
    {
      std::vector<int> decoded_column;
      for (auto& row : column)
      {
        for (auto& layer : row.layers)
        {
          if (layer == 2)  // transparent layers are ignored
            continue;
          decoded_column.push_back(layer);
          break;
        }
      }
      decoded_image.push_back(decoded_column);
    }
    return decoded_image;
  }

private:
  std::vector<std::vector<Pixel>> pixels_;
};

int countValueOccurencesInLayer(std::vector<int> layer, int value)
{
  int occurences = 0;
  for (auto pixel_value : layer)
  {
    if (pixel_value == value)
      occurences++;
  }
  return occurences;
}

int main()
{
  std::string input_file = "/home/joe/projects/advent-of-code-2019/day8/input/space_image.txt";
  std::ifstream file;
  file.open(input_file.c_str());

  std::pair<int, int> image_dimensions = { 25, 6 };
  SpaceImage image(image_dimensions.first, image_dimensions.second);

  std::string row;
  int row_number = 0;
  while (true)
  {
    row.resize(image_dimensions.first);
    file.read(&row[0], image_dimensions.first);
    if (!file.good())
      break;
    for (int column_number = 0; column_number < image_dimensions.first; ++column_number)
    {
      int layer_value = row.at(column_number) - '0';
      if (layer_value < 0 || layer_value > 9)
        break;
      image.addLayer(column_number, row_number, layer_value);
    }
    if (++row_number == image_dimensions.second)
      row_number = 0;
  }
  int number_of_layers = image.getNumberOfLayers();
  int fewest_number_of_zeros = std::numeric_limits<int>::max();
  int return_value;
  for (int l = 0; l < number_of_layers; ++l)
  {
    std::vector<int> layer = image.getImageLayer(l);
    int number_of_zeros = countValueOccurencesInLayer(layer, 0);
    if (number_of_zeros < fewest_number_of_zeros)
    {
      fewest_number_of_zeros = number_of_zeros;
      int number_of_ones = countValueOccurencesInLayer(layer, 1);
      int number_of_twos = countValueOccurencesInLayer(layer, 2);
      return_value = number_of_ones * number_of_twos;
    }
  }

  std::cout << "The layer with the fewest 0's has a value of " << return_value
            << " when the number of 1's is multiplied by the number of 2's" << std::endl;

  // Part 2
  std::vector<std::vector<int>> decoded_image = image.decodeImage();
  char black = 32;
  char white = 35;
  for (int y = 0; y < image_dimensions.second; ++y)
  {
    for (int x = 0; x < image_dimensions.first; ++x)
    {
      if (decoded_image[x][y] == 0)
        std::cout << black;
      else if (decoded_image[x][y] == 1)
        std::cout << white;
    }
    std::cout << std::endl;
  }
  return 0;
}
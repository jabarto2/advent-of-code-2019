#include <iostream>
#include <list>
#include <vector>

std::list<int> getListOfDigits(int number)
{
  std::list<int> digits;
  do
  {
    digits.push_front(number % 10);
  } while (number /= 10);

  return digits;
}

bool numberContainsEquivalentAdjacentDigits(int number)
{
  std::list<int> digits = getListOfDigits(number);

  int previous_digit = -1;
  for (int digit : digits)
  {
    if (digit == previous_digit)
      return true;
    else
      previous_digit = digit;
  }
  return false;
}

bool numberContainsAnExactPairOfEquivalentAdjacentDigits(int number)
{
  std::vector<int> digit_counts(10, 0);
  std::list<int> digits = getListOfDigits(number);

  for (int digit : digits)
  {
    digit_counts.at(digit)++;
  }

  for (int count : digit_counts)
  {
    if (count == 2)
      return true;
  }
  return false;
}

bool numberContainsOnlyIncreasingDigits(int number)
{
  std::list<int> digits = getListOfDigits(number);

  int previous_digit = -1;
  for (int digit : digits)
  {
    if (digit >= previous_digit)
    {
      previous_digit = digit;
      continue;
    }
    else
      return false;
  }
  return true;
}

int main()
{
  int lower_password_bound = 171309;
  int upper_password_bound = 643603;

  // Part 1
  int possible_passwords = 0;
  for (int i = lower_password_bound; i <= upper_password_bound; ++i)
  {
    if (numberContainsEquivalentAdjacentDigits(i))
    {
      if (numberContainsOnlyIncreasingDigits(i))
      {
        possible_passwords++;
      }
    }
  }
  std::cout << "With the old rules, there are " << possible_passwords << " possible passwords between the values of "
            << lower_password_bound << " and " << upper_password_bound << std::endl;

  // Part 2
  possible_passwords = 0;
  for (int i = lower_password_bound; i <= upper_password_bound; ++i)
  {
    if (numberContainsAnExactPairOfEquivalentAdjacentDigits(i))
    {
      if (numberContainsOnlyIncreasingDigits(i))
      {
        possible_passwords++;
      }
    }
  }
  std::cout << "With the new rules, there are " << possible_passwords << " possible passwords between the values of "
            << lower_password_bound << " and " << upper_password_bound << std::endl;

  std::cout << numberContainsAnExactPairOfEquivalentAdjacentDigits(112233) << std::endl;

  return 0;
}
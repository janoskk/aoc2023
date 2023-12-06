#include <algorithm>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <tuple>
#include <vector>

using namespace std;

uint64_t calc(uint64_t max_time, uint64_t distance)
{
  // example: max_time=7
  // time = 1 --> distance = (7-1)*1 = 6
  // time = 2 --> distance = (7-2)*2 = 10
  // time = 3 --> distance = (7-3)*3 = 12
  // time = 4 --> distance = (7-4)*4 = 12
  // time = 5 --> distance = (7-5)*5 = 10
  // time = 6 --> distance = (7-6)*6 = 6

  uint64_t counter{};
  for (uint64_t time = 1; time < max_time; time++)
  {
    if ((max_time - time) * time > distance)
      counter++;
  }
  return counter;
}

int main(int argc, char** argv)
{
  if (argc != 2)
  {
    cerr << "Usage: " << argv[0] << " file.txt\n";
    return 1;
  }

  vector<uint64_t> time_vect;
  vector<uint64_t> distance_vect;
  ifstream infile(argv[1]);

  for (string line; getline(infile, line); )
  {
    istringstream iss(line);
    if (line.starts_with("Time:"))
    {
      string tmp;
      iss >> tmp;
      for (decltype(time_vect)::value_type input; iss >> input; )
        time_vect.push_back(input);

      continue;
    }
    else if (line.starts_with("Distance:"))
    {
      string tmp;
      iss >> tmp;
      for (decltype(distance_vect)::value_type input; iss >> input; )
        distance_vect.push_back(input);

      break;
    }
  }

  uint64_t variations{1};
  for (unsigned i = 0; i < time_vect.size(); i++)
  {
    uint64_t result = calc(time_vect[i], distance_vect[i]);
    variations *= result;
    cout << "(" << time_vect[i] << ", " << distance_vect[i] 
      << ") result: " << result << "\n";
  }

  cout << "variations: " << variations << "\n";

  cout << "part 2, lazy way: " << calc(52947594, 426137412791216) << "\n";
  

  return 0;
}


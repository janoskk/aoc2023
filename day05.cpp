#include <algorithm>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <tuple>
#include <vector>

using namespace std;

struct mapping_t
{
  using data_t = tuple<uint64_t, uint64_t, uint64_t>;
  void add(data_t&& data) { intervals.push_back(std::move(data)); }
  void reset() { intervals.clear(); }
  bool empty() { return intervals.empty(); }

  uint64_t get(uint64_t input)
  {
    for (const auto& [dst_start, src_start, length] : intervals)
    {
      if (src_start <= input && input < src_start + length)
        return dst_start + input - src_start;
    }
    return input;
  }

  vector<data_t> intervals;
};


int main(int argc, char** argv)
{
  if (argc != 2)
  {
    cerr << "Usage: " << argv[0] << " file.txt\n";
    return 1;
  }

  vector<uint64_t> input_vect;
  mapping_t mapping;
  ifstream infile(argv[1]);

  auto process_f = [&]{
    decltype(input_vect) new_input_vect;
    // calculate the new input using the mapping.
    for (auto input: input_vect)
      new_input_vect.push_back(mapping.get(input));

    // set the new input.
    input_vect = new_input_vect;

    // drop the current mapping data.
    mapping.reset();
  };

  for (string line; getline(infile, line); )
  {
    istringstream iss(line);
    if (line.starts_with("seeds: "))
    {
      // first line contains the input data that needs to be iterated.
      string tmp;
      iss >> tmp;
      for (decltype(input_vect)::value_type input; iss >> input; )
        input_vect.push_back(input);

      continue;
    }
    
    if (line.empty())
    {
      // empty line means end of mapping block --> processing time!
      process_f();
      continue;
    }

    // create mapping with the data parsed from the current line.
    decltype(input_vect)::value_type a, b, c;
    if (iss >> a >> b >> c) 
      mapping.add({a, b, c});
  }

  // process if needed.
  if (!mapping.empty())
    process_f();
  
  // sort the end result.
  std::sort(input_vect.begin(), input_vect.end());

  // voila.
  cout << "Lowest: " << input_vect[0] << "\n";

  return 0;
}


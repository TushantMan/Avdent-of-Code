#include <algorithm>
#include <cassert>
#include <cmath>
#include <fstream>
#include <iostream>
#include <numeric>
#include <string>
#include <vector>
#include <array>

// Structure to hold mapping information
struct Mapping {
  std::array<long long, 2> from;
  std::array<long long, 2> to;

  // Function to apply the mapping
  long long apply_map(const long long n) const {
    return to[0] + (n - from[0]);
  }
};

// Structure to represent a map with multiple mappings
struct Map {
  std::string map_from;
  std::string map_to;
  std::vector<Mapping> mappings;
};

// Function to extract numbers from a string
std::vector<long long> extract(const std::string& s) {
  std::vector<long long> numbers;
  std::size_t start = 0;
  std::size_t end = s.find(' ');
  while (end != std::string::npos) {
    numbers.push_back(std::stoll(s.substr(start, end - start)));
    start = end + 1;
    end = s.find(' ', start);
  }
  numbers.push_back(std::stoll(s.substr(start, s.size() - start)));
  return numbers;
}

// Function to apply mappings to relevant ranges
std::vector<std::pair<long long, long long>> apply_filter(std::vector<std::pair<long long, long long>> relevant_ranges, const std::vector<Mapping>& mappings) {
  std::vector<std::pair<long long, long long>> new_relevant_values;

  for (const auto& relevant_range : relevant_ranges) {
    std::vector<std::pair<long long, long long>> ranges_where_filter_applied;
    std::vector<std::pair<long long, long long>> values_when_filter_applied;
    for (const auto& mapping : mappings) {
      if (relevant_range.second < mapping.from[0] || relevant_range.first > mapping.from[1]) continue;
      if (relevant_range.first >= mapping.from[0] && relevant_range.second <= mapping.from[1]) {
        ranges_where_filter_applied.emplace_back(relevant_range.first, relevant_range.second);
      } else if (relevant_range.first >= mapping.from[0] && relevant_range.second >= mapping.from[1]) {
        ranges_where_filter_applied.emplace_back(relevant_range.first, mapping.from[1]);
      } else if (relevant_range.first <= mapping.from[0] && relevant_range.second <= mapping.from[1]) {
        ranges_where_filter_applied.emplace_back(mapping.from[0], relevant_range.second);
      } else if (relevant_range.first <= mapping.from[0] && relevant_range.second >= mapping.from[1]) {
        ranges_where_filter_applied.emplace_back(mapping.from[0], mapping.from[1]);
      } else {
        std::cout << "?!" << '\n';
        exit(0);
      }
      values_when_filter_applied.emplace_back(mapping.apply_map(ranges_where_filter_applied.back().first), mapping.apply_map(ranges_where_filter_applied.back().second));
    }
    auto current_start = relevant_range.first;
    for (int i = 0; i < ranges_where_filter_applied.size(); i++) {
      new_relevant_values.emplace_back(current_start, ranges_where_filter_applied[i].first - 1);
      new_relevant_values.emplace_back(values_when_filter_applied[i].first, values_when_filter_applied[i].second);
      current_start = ranges_where_filter_applied[i].second + 1;
    }
    if (current_start <= relevant_range.second) {
      new_relevant_values.emplace_back(current_start, relevant_range.second);
    }
  }
  std::vector<std::pair<long long, long long>> filtered_new_relevant_values;
  std::copy_if(std::begin(new_relevant_values), std::end(new_relevant_values), std::back_inserter(filtered_new_relevant_values), [](const auto& ele ){
    return ele.first <= ele.second; 
  });
  std::sort(std::begin(filtered_new_relevant_values), std::end(filtered_new_relevant_values), [](const auto& v1, const auto& v2) {return v1.first < v2.first;});
  return filtered_new_relevant_values;
}

int main(int argc, char* argv[]) {
  std::string input = "day5_input.txt";
  if (argc > 1) {
    input = argv[1];
  }

  std::string line;
  std::fstream file(input);
  std::vector<Map> maps;
  std::getline(file, line);
  std::vector<long long> input_ranges = extract(line.substr(7, line.size() - 7));
  std::vector<std::pair<long long, long long>> seeds;
  for (int i = 0; i < input_ranges.size(); i += 2) {
    seeds.emplace_back(input_ranges[i], input_ranges[i] + input_ranges[i + 1] - 1);
  }
  while (std::getline(file, line)) {
    if (line.empty()) {
      std::getline(file, line);
      Map map;
      long long start = 0;
      long long end = line.find('-');
      map.map_from = line.substr(start, end - start);
      start = end + 4;
      end = line.find(' ', start);
      map.map_to = line.substr(start, end - start);
      maps.push_back(map);
      std::getline(file, line);
    }
    const auto numbers = extract(line);
    Mapping mapping;
    mapping.to = {{numbers[0], numbers[0] + numbers[2] - 1}};
    mapping.from = {{numbers[1], numbers[1] + numbers[2] - 1}};
    maps.back().mappings.push_back(mapping);
  }

  for (auto& map : maps) {
    std::sort(std::begin(map.mappings), std::end(map.mappings), [](const auto& m1, const auto& m2) { return m1.from[0] < m2.from[0]; });
  }

  for (int i = 0; i < maps.size() - 1; i++) {
    assert(maps[i + 1].map_from == maps[i].map_to);
  }

  std::vector<std::pair<long long, long long>> relevant_values = seeds;
  for (const auto& map : maps) {
    relevant_values = apply_filter(relevant_values, map.mappings);
  }
  std::cout << "The puzzle answer is: ";
  std::cout << min_element(std::begin(relevant_values), std::end(relevant_values))->first << '\n';
  return 0;
}

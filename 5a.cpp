#include <algorithm>
#include <cmath>
#include <fstream>
#include <iostream>
#include <numeric>
#include <string>
#include <vector>

//Structure for mapping
struct Mapping {
  long long destination_start;
  long long source_start;;
  long long range;

  // Check if a value is within the destination range
  bool in_destination_range(const long long n) const {
    return n >= destination_start && n <= destination_start + range;
  }

  // Check if a value is within the source range
  bool in_source_range(const long long n) const {
    return n >= source_start && n <= source_start + range;
  }

  // Get the mapped value of a source value
  long long get_mapping(const long long n) const {
    return destination_start + (n - source_start);
  }
};

// Structure for mapping rules
struct Map {
  std::string map_from; // Source parameter
  std::string map_to;   // Destination parameter
  std::vector<Mapping> mappings; // Mapping rules
};

// Function to extract numbers from a string
std::vector<long long> extract(const std::string& s) {
  std::vector<long long> numbers;
  long long start = 0;
  long long end = s.find(' ');
  while (end != std::string::npos) {
    numbers.push_back(std::stoll(s.substr(start, end - start)));
    start = end + 1;
    end = s.find(' ', start);
  }
  numbers.push_back(std::stoll(s.substr(start, s.size() - start)));
  return numbers;
}

// Parameters for mapping
std::vector<std::string> parameters {
  "soil", "fertilizer", "water", "light", "temperature", "humidity", "location"
};

// Recursive function to get the final location
long long get_location (long long value, const std::vector<Map>& maps, const std::string& map_from) {
  if (map_from == "location") { // Base case: if the destination is 'location', return the value
    return value;
  }
  const auto map = *std::find_if(
    std::begin(maps), 
    std::end(maps), 
    [&map_from](const auto& map) {
      return map.map_from == map_from; // Find the mapping rule for the current parameter
    }
  );
  for (const auto& mapping : map.mappings) {
    if (mapping.in_source_range(value)) { // Check if the value falls within the source range
      return get_location(mapping.get_mapping(value), maps, map.map_to); // Recursively get the final location
    } 
  }
  return get_location(value, maps, map.map_to); // Recursively get the final location
}

int main(int argc, char * argv[]) {
  std::string input = "day5_input.txt"; // Default input file
  if (argc > 1) {
    input = argv[1]; // Use the provided input file
  }

  std::string line;
  std::fstream file(input);
  std::vector<Map> maps;
  std::getline(file, line);

  std::vector<long long> seeds = extract(line.substr(7, line.size() - 7)); // Extract seed values from input

  while(std::getline(file, line)) {
    if (line.empty()) { // Empty line separates mapping rules
      std::getline(file, line);

      Map map;
      long long start = 0;
      long long end = line.find('-');
      map.map_from = line.substr(start, end - start); // Extract source parameter
      start = end + 4;
      end = line.find(' ', start);
      map.map_to = line.substr(start, end - start); // Extract destination parameter
      maps.push_back(map);
      std::getline(file, line);
    }

    const auto numbers = extract(line);
    Mapping mapping;
    mapping.destination_start = numbers[0]; // Extract destination start value
    mapping.source_start = numbers[1];      // Extract source start value
    mapping.range = numbers[2];             // Extract range
    maps.back().mappings.push_back(mapping); // Add mapping to the current map
  }

  std::vector<long long> locations;
  for (const auto& seed : seeds) {
    locations.push_back(get_location(seed, maps, "seed")); // Get the final location for each seed
  }
  std::cout << "The puzzle answer is: ";
  std::cout << *std::min_element(std::begin(locations), std::end(locations)) << '\n'; // Output the minimum final location
  return 0;
}

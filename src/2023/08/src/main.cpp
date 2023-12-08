// adventofcode.com/2023/day/8

#include <functional>
#include <numeric>
#include <string>
#include <unordered_map>
#include <vector>

#include <fmt/core.h>

#include "lib/io.h"
#include "lib/parse.h"
#include "lib/run.h"
#include "lib/to.h"

namespace {

struct Network {
  using Node = std::unordered_map<std::string, std::pair<std::string, std::string>>;

  std::string instructions;
  Node nodes;
};

Network readFile(const std::string& path) {
  auto [instructionsString, nodesString] = splitToPair(read(path), "\n\n", true);

  Network network = {
      .instructions = std::move(instructionsString),
  };

  for (const auto& nodeString : split(std::move(nodesString), "\n")) {
    // "AAA = (BBB, CCC)"
    assert(nodeString.size() == 16);
    auto name = nodeString.substr(0, 3);
    auto left = nodeString.substr(7, 3);
    auto right = nodeString.substr(12, 3);
    network.nodes.emplace(std::move(name),
                          std::pair<std::string, std::string>{std::move(left), std::move(right)});
  }

  return network;
}

size_t findTarget(const Network& network,
                  std::string node,
                  const std::function<bool(const std::string&)>& comparison) {
  assert(comparison);
  size_t count = 0;
  while (!comparison(node)) {
    const char instruction = network.instructions[count % network.instructions.size()];
    const auto networkNodeIt = network.nodes.find(node);
    const auto& networkNodeName = networkNodeIt->first;
    assert(networkNodeName == node);
    const auto& [networkNodeLeft, networkNodeRight] = networkNodeIt->second;
    // fmt::print("'{}', node: '{}', left: '{}', right: '{}'\n", instruction, node, networkNodeLeft,
    //            networkNodeRight);
    node = (instruction == 'L') ? networkNodeLeft : networkNodeRight;
    ++count;
  }

  return count;
}

size_t part1(const std::string& path) {
  return findTarget(readFile(path), "AAA", [](const auto& node) { return node == "ZZZ"; });
}

size_t part2(const std::string& path) {
  const auto network = readFile(path);

  std::vector<std::string> targets{};
  for (const auto& node : network.nodes) {
    const auto& [name, _leftRight] = node;
    assert(name.size() == 3);
    if (name[2] == 'A') {
      targets.emplace_back(name);
    }
  }
  // fmt::print("targets: '{}'\n", fmt::join(targets, ", "));

  std::vector<size_t> counts{};
  std::transform(targets.begin(), targets.end(), std::back_inserter(counts),
                 [&network](const auto& target) {
                   return findTarget(network, target, [](const auto& node) {
                     assert(node.size() == 3);
                     return node[2] == 'Z';
                   });
                 });
  // fmt::print("counts: '{}'\n", fmt::join(counts, ", "));

  return std::accumulate(counts.begin(), counts.end(), 1UL,
                         [](const auto& lcm, const auto& count) { return std::lcm(lcm, count); });
}

}  // namespace

int main() {
  run(1, part1, true, 2UL);
  run(1, part1, true, 6UL, "data/example2.txt");
  run(1, part1, false, 16409UL);
  run(2, part2, true, 6UL, "data/example3.txt");
  run(2, part2, false, 11795205644011UL);
}

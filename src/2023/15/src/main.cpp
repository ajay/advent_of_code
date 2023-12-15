// adventofcode.com/2023/day/15

#include <array>
#include <numeric>
#include <string>
#include <vector>

#include <fmt/core.h>

#include "lib/io.h"
#include "lib/parse.h"
#include "lib/run.h"
#include "lib/to.h"

namespace {

constexpr uint8_t hash(const std::string_view& sv) {
  return std::accumulate(sv.begin(), sv.end(), uint8_t{},
                         [](const auto& sum, const auto& ch) { return 17 * (sum + ch); });
}
static_assert(hash("HASH") == 52);

std::vector<std::string> readFile(const std::string& path) {
  return split(read(path), ",");
}

size_t part1(const std::string& path) {
  const auto data = readFile(path);

  return std::accumulate(data.begin(), data.end(), size_t{},
                         [](const auto& sum, const auto& s) { return sum + hash(s); });
}

size_t part2(const std::string& path) {
  std::array<std::vector<std::pair<std::string, size_t>>, 256> boxes{};

  const auto steps = readFile(path);
  for (const auto& step : steps) {
    assert(step.size());
    const auto indexEqOrMinus = step.find_first_of("=-");
    const auto label = step.substr(0, indexEqOrMinus);
    const auto box = hash(label);

    const bool isEqualSign = (indexEqOrMinus != (step.size() - 1));
    const auto focalLength = isEqualSign ? to<size_t>(step.substr(indexEqOrMinus + 1)) : size_t{};
    // fmt::print(
    //     "step: {:10}, step.size(): {}, indexEqOrMinus: {}, isEqualSign: {:>5}, focalLength:
    //     {}\n", step, step.size(), indexEqOrMinus, isEqualSign, focalLength);

    if (const auto it = std::find_if(boxes[box].begin(), boxes[box].end(),
                                     [&label](const auto& node) { return node.first == label; });
        it != boxes[box].end()) {
      if (isEqualSign) {
        it->second = focalLength;
      } else {
        boxes[box].erase(it);
      }
    } else if (isEqualSign) {
      boxes[box].emplace_back(label, focalLength);
    }
  }

  // clang-format off
  return std::accumulate(boxes.begin(), boxes.end(), size_t{}, [indexBox = size_t{}](const auto& sum, const auto& box) mutable {
    ++indexBox;
    // fmt::print("\nindexBox: {}, sum: {}\n", indexBox, sum);
    return sum + std::accumulate(box.begin(), box.end(), size_t{}, [&indexBox, indexLens = size_t{}](const auto& sumBox, const auto& lens) mutable {
      // fmt::print("indexBox: {}, indexLens: {}, focalLength: {}, sumBox: {}\n", indexBox, indexLens + 1, lens.second, sumBox);
        return sumBox + (indexBox * ++indexLens * lens.second);
      });
    });
  // clang-format on
}

}  // namespace

int main() {
  run(1, part1, true, 1320UL);
  run(1, part1, false, 503154UL);
  run(2, part2, true, 145UL);
  run(2, part2, false, 251353UL);
}

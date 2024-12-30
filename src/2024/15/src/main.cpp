// adventofcode.com/2024/day/15

#include <cassert>
#include <cstddef>
#include <string>
#include <utility>
#include <vector>

#include <fmt/core.h>

#include "lib/io.h"
#include "lib/parse.h"
#include "lib/run.h"

struct Simulation {
  std::vector<std::string> grid;
  std::string sequence;
  size_t row = 0;
  size_t col = 0;

  enum class Direction : char {
    Up = '^',
    Down = 'v',
    Left = '<',
    Right = '>',
  };
};

std::vector<std::string> readFile(const std::string& path) {
  return split(read(path), "\n\n", true);
}

void findAt(Simulation& simulation) {
  const auto& grid = simulation.grid;

  for (size_t r = 0; r < grid.size(); ++r) {
    assert(grid[r].size() == grid[0].size());
    for (size_t c = 0; c < grid[0].size(); ++c) {
      if (grid[r][c] == '@') {
        assert(simulation.row == 0);
        assert(simulation.col == 0);
        simulation.row = r;
        simulation.col = c;
      }
    }
  }
};

Simulation parse(const std::string& path) {
  auto data = readFile(path);
  assert(data.size() == 2);

  auto sequenceParts = split(std::move(data[1]), "\n");
  std::string sequence{};
  for (auto&& part : sequenceParts) {
    sequence += std::move(part);
  }

  Simulation simulation = {
      .grid = split(std::move(data[0]), "\n"),
      .sequence = std::move(sequence),
  };
  findAt(simulation);

  // fmt::println("simulation.grid: {}", simulation.grid);
  // fmt::println("simulation.sequence: {}", simulation.sequence);

  return simulation;
};

std::string print(const std::vector<std::string>& grid) {
  std::string gridStr;
  for (const auto& row : grid) {
    gridStr += fmt::format("{}\n", row);
  }

  return gridStr;
}

void move(Simulation& simulation, const char turnCh) {
  const auto turn = static_cast<Simulation::Direction>(turnCh);
  auto& grid = simulation.grid;
  bool moveable = false;
  size_t row = simulation.row;
  size_t col = simulation.col;
  size_t openRow = row;
  size_t openCol = col;

  const auto checkMoveable = [&grid, &row, &col, &openRow, &openCol, &moveable]() {
    // fmt::println("row: {}, col: {}, grid[row][col]: '{}'", row, col, grid[row][col]);
    if (grid[row][col] == '.') {
      moveable = true;
      openRow = row;
      openCol = col;
    }
  };

  switch (turn) {
    case Simulation::Direction::Up:
    case Simulation::Direction::Down: {
      const bool down = (turn == Simulation::Direction::Down);
      for (; !moveable && (grid[row][col] != '#') && (row < grid.size()); down ? ++row : --row) {
        checkMoveable();
      }
      break;
    }
    case Simulation::Direction::Left:
    case Simulation::Direction::Right: {
      const bool right = (turn == Simulation::Direction::Right);
      for (; !moveable && (grid[row][col] != '#') && (col < grid[row].size());
           right ? ++col : --col) {
        checkMoveable();
      }
      break;
    }
    default:
      assert(false && "Unknown turn");
      break;
  }
  // fmt::println("moveable: {}", moveable);

  if (moveable) {
    row = simulation.row;
    col = simulation.col;
    // fmt::println(" row,  col: ({}, {})", row, col);
    // fmt::println("oRow, oCol: ({}, {})", openRow, openCol);

    switch (turn) {
      case Simulation::Direction::Up:
        for (; openRow < row; ++openRow) {
          grid[openRow][openCol] = grid[openRow + 1][openCol];
        }
        --simulation.row;
        break;
      case Simulation::Direction::Down:
        for (; openRow > row; --openRow) {
          grid[openRow][openCol] = grid[openRow - 1][openCol];
        }
        ++simulation.row;
        break;
      case Simulation::Direction::Left:
        for (; openCol < col; ++openCol) {
          grid[openRow][openCol] = grid[openRow][openCol + 1];
        }
        --simulation.col;
        break;
      case Simulation::Direction::Right:
        for (; openCol > col; --openCol) {
          grid[openRow][openCol] = grid[openRow][openCol - 1];
        }
        ++simulation.col;
        break;
      default:
        assert(false && "Unknown turn");
        break;
    }

    grid[openRow][openCol] = '.';
  }
}

size_t gps(const Simulation& simulation, const char box = 'O') {
  const auto& grid = simulation.grid;
  size_t gps = 0;

  for (size_t r = 0; r < grid.size(); ++r) {
    for (size_t c = 0; c < grid[r].size(); ++c) {
      if (grid[r][c] == box) {
        gps += (100 * r) + c;
      }
    }
  }

  return gps;
}

size_t part1(const std::string& path) {
  auto simulation = parse(path);

  for (const auto& turn : simulation.sequence) {
    move(simulation, turn);
    // fmt::println("({}, {}) '{}'\n{}", simulation.row, simulation.col, turn,
    // print(simulation.grid));
  }

  return gps(simulation);
}

size_t part2(const std::string& path) {
  return path.size();
}

int main() {
  run(1, part1, true, 2028UL, "data/example.txt");
  run(1, part1, true, 10092UL, "data/example2.txt");
  run(1, part1, false, 1497888UL);
  // run(2, part2, true, 9021UL, "data/example2.txt");
  // run(2, part2, false, 0UL);
}

#include <algorithm>
#include <bitset>
#include <cassert>
#include <fstream>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <map>
#include <numeric>
#include <sstream>
#include <streambuf>
#include <string>
#include <string_view>
#include <vector>

std::string readFile(const std::string& filename = "input.txt") {
    std::ifstream file{filename};
    return {(std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>()};
}

std::vector<std::string> split(const std::string& str, const std::string& delim = " ") {
    std::vector<std::string> v{};
    size_t start = 0;
    size_t end = 0;

    while ((start = str.find_first_not_of(delim, end)) != std::string::npos)
    {
        end = str.find(delim, start);
        v.push_back(str.substr(start, end - start));
    }

    return v;
}

std::vector<size_t> strVecToNumVec(const std::vector<std::string>& strVec) {
    std::vector<size_t> numVec{};

    std::transform(strVec.begin(), strVec.end(), std::back_inserter(numVec), [] (const auto& s) {
        return std::stoi(s);
    });

    return numVec;
}

template<class T>
std::ostream& operator<<(std::ostream& os, const std::vector<T>& v) {
    constexpr auto delim = ' ';
    std::stringstream s{};

    s << '{' << delim;
    for (const auto& e : v) {
        s << e << delim;
    }
    s << '}';

    return os << s.str();
}

class BingoBoard final {
public:
    BingoBoard(const std::string& str) {
        const auto lines = split(str, "\n");
        for (const auto& line : lines) {
            const auto nums = split(line);
            board.emplace_back(strVecToNumVec(nums));
        }
    }

    bool checkWin(const std::vector<size_t>& moves) const {
        bool status = false;

        // rows
        for (const auto& row: board) {
            status |= std::all_of(row.begin(), row.end(), [&moves] (const auto& e) {
                return std::find(moves.begin(), moves.end(), e) != moves.end();
            });
        }

        // cols
        for (size_t col = 0; col < board.size(); ++col) {

            std::vector<size_t> column{};
            for (size_t row = 0; row < board.size(); ++row) {
                column.emplace_back(board[row][col]);
            }

            status |= std::all_of(column.begin(), column.end(), [&moves] (const auto& e) {
                return std::find(moves.begin(), moves.end(), e) != moves.end();
            });
        }

        return status;
    }

    size_t sumUnmarked(const std::vector<size_t>& moves) const {
        size_t sum = 0;

        for (const auto& line : board) {
            sum += std::accumulate(line.begin(), line.end(), 0UL, [&moves] (const auto& a, const auto& num) {
                return std::find(moves.begin(), moves.end(), num) != moves.end() ? a : (a + num);
            });
        }

        return sum;
    }

    std::string print() const {
        std::stringstream s{};

        s << '\n';
        for (const auto& line : board) {
            s << line << '\n';
        }

        return s.str();
    }

private:
    std::vector<std::vector<size_t>> board;
};

std::ostream& operator<<(std::ostream& os, const BingoBoard& c) {
    return os << c.print();
}

std::pair<std::vector<size_t>, std::vector<BingoBoard>> parseFile() {
    const auto raw = readFile();
    const auto data = split(raw, "\n\n");

    const auto movesStr = split(data[0], ",");
    const auto moves = strVecToNumVec(movesStr);

    std::vector<BingoBoard> boards{};
    std::for_each(std::next(data.begin()), data.end(), [&boards] (const auto& b) {
        boards.emplace_back(b);
    });

    return {moves, boards};
}

size_t part1() {
    const auto [moves, boards] = parseFile();

    // std::cout << "\nmoves:\n" << moves << '\n';

    for(size_t i = 1; i < moves.size(); ++i) {
        std::vector<size_t> subMoves{moves.begin(), std::next(moves.begin(), static_cast<ssize_t>(i))};

        for (const auto& b : boards) {
            if (b.checkWin(subMoves)) {
                // std::cout << "submoves:\n" << subMoves << '\n';
                // std::cout << "winner:\n" << b << '\n';
                // std::cout << "sumUnmarked:\n" << b.sumUnmarked(subMoves);
                return b.sumUnmarked(subMoves) * subMoves.back();
            }
        }
    }

    return 0;
}

size_t part2() {
    const auto [moves, boards] = parseFile();

    size_t i = 0;
    for(i = 1; i < moves.size(); ++i) {
        std::vector<size_t> subMoves{moves.begin(), std::next(moves.begin(), static_cast<ssize_t>(i))};
        if (std::all_of(boards.begin(), boards.end(), [&subMoves] (const auto& b) {
            return b.checkWin(subMoves);
        })) {
            // std::cout << "found all winners " << i << '\n';
            break;
        }
    }

    std::vector<size_t> subMoves{moves.begin(), std::next(moves.begin(), static_cast<ssize_t>(i-1))};
    for (const auto& b : boards) {
        if (!b.checkWin(subMoves)) {
            std::vector<size_t> subMovesWin{moves.begin(), std::next(moves.begin(), static_cast<ssize_t>(i))};
            return b.sumUnmarked(subMovesWin) * subMovesWin.back();
        }
    }

    return 0;
}

int main() {
    const auto p1 = part1();
    assert(p1 == 54275);
    std::cout << "part1: " << p1 << '\n';

    const auto p2 = part2();
    assert(p2 == 13158);
    std::cout << "part2: " << p2 << '\n';
}

#include "backend/Board.h"
#include <algorithm>
#include <random>

Board::Board()
    : m_width(9), m_height(9), m_mineCount(10),
      m_cells(m_height * m_width)
{
}

Board::Board(int width, int height, int mineCount)
    : m_width(width), m_height(height), m_mineCount(mineCount),
      m_cells(m_height * m_width)
{
}

void Board::initialize()
{
    initialize(-1, -1); // No safe cell
}

void Board::initialize(int safeX, int safeY)
{
    // Clear all cell states with fresh default cells
    m_cells.assign(m_height * m_width, Cell{});

    // Collect safe positions (clicked cell + 3x3 neighborhood)
    // Classic Minesweeper guarantees the entire 3x3 around first click is safe
    std::vector<std::pair<int, int>> safeZone;
    if (safeX >= 0 && safeY >= 0) {
        for (int dy = -1; dy <= 1; ++dy)
            for (int dx = -1; dx <= 1; ++dx) {
                int nx = safeX + dx;
                int ny = safeY + dy;
                if (nx >= 0 && nx < m_width && ny >= 0 && ny < m_height)
                    safeZone.push_back({nx, ny});
            }
    }

    std::mt19937 gen(std::random_device{}());
    std::vector<int> positions;
    for (int y = 0; y < m_height; ++y)
        for (int x = 0; x < m_width; ++x) {
            // Skip safe zone positions
            bool isSafe = false;
            for (auto &sz : safeZone) {
                if (sz.first == x && sz.second == y) { isSafe = true; break; }
            }
            if (!isSafe) {
                positions.push_back(index(x, y));
            }
        }

    std::shuffle(positions.begin(), positions.end(), gen);

    int minesToPlace = std::min(m_mineCount, static_cast<int>(positions.size()));
    for (int i = 0; i < minesToPlace; ++i)
        m_cells[positions[i]].setMine(true);

    calculateAdjacentMines();
}

void Board::calculateAdjacentMines()
{
    std::vector<std::pair<int, int>> directions = {{-1,0}, {1,0}, {0,-1}, {0,1}, {-1,-1}, {-1,1}, {1,-1}, {1,1}};

    for (int y = 0; y < m_height; ++y) {
        for (int x = 0; x < m_width; ++x) {
            int count = 0;
            for (auto &d : directions) {
                int nx = x + d.first;
                int ny = y + d.second;
                if (nx >= 0 && nx < m_width && ny >= 0 && ny < m_height) {
                    if (m_cells[index(nx, ny)].isMine()) ++count;
                }
            }
            m_cells[index(x, y)].setAdjacentMines(count);
        }
    }
}

Cell& Board::cell(int x, int y)
{
    return m_cells[index(x, y)];
}

const Cell& Board::cell(int x, int y) const
{
    return m_cells[index(x, y)];
}

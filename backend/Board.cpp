#include "backend/Board.h"
#include <algorithm>

Board::Board(int width, int height, int mineCount)
    : m_width(width), m_height(height), m_mineCount(mineCount)
{
    m_cells.resize(width, std::vector<Cell>(height));
}

void Board::initialize()
{
    std::mt19937 gen(std::random_device{}());
    std::vector<int> positions;
    for (int y = 0; y < m_height; ++y)
        for (int x = 0; x < m_width; ++x)
            positions.push_back(x + y * m_width);
    
    std::shuffle(positions.begin(), positions.end(), gen);
    
    for (int i = 0; i < m_mineCount; ++i)
        m_cells[positions[i] % m_width][positions[i] / m_width].setMine(true);
    
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
                    if (m_cells[nx][ny].isMine()) ++count;
                }
            }
            m_cells[x][y].setAdjacentMines(count);
        }
    }
}

Cell& Board::cell(int x, int y)
{
    return m_cells[x][y];
}

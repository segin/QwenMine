#include "backend/Board.h"

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
}

Cell& Board::cell(int x, int y)
{
    return m_cells[x][y];
}

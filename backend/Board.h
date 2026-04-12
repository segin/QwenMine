#ifndef BOARD_H
#define BOARD_H

#include <vector>
#include <random>

class Cell;

class Board
{
public:
    Board(int width, int height, int mineCount);
    void initialize();
    Cell& cell(int x, int y);
    int width() const { return m_width; }
    int height() const { return m_height; }
    int mineCount() const { return m_mineCount; }

private:
    int m_width;
    int m_height;
    int m_mineCount;
    std::vector<std::vector<Cell>> m_cells;
};

#endif // BOARD_H

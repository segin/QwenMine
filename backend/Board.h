#ifndef BOARD_H
#define BOARD_H

#include <vector>
#include <random>
#include "backend/Cell.h"

class Board
{
public:
    Board();
    Board(int width, int height, int mineCount);
    void initialize();
    void initialize(int safeX, int safeY);
    Cell& cell(int x, int y);
    const Cell& cell(int x, int y) const;
    int width() const { return m_width; }
    int height() const { return m_height; }
    int mineCount() const { return m_mineCount; }
    void calculateAdjacentMines();

private:
    int index(int x, int y) const { return y * m_width + x; }

    int m_width;
    int m_height;
    int m_mineCount;
    std::vector<Cell> m_cells; // flat: row-major, index = y * m_width + x
};

#endif // BOARD_H

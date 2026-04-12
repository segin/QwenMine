#include "backend/Board.h"

class Cell {
public:
    bool isMine() const { return m_isMine; }
    bool isFlagged() const { return m_isFlagged; }
    bool isRevealed() const { return m_isRevealed; }
    int adjacentMines() const { return m_adjacentMines; }
    void toggleFlag() { m_isFlagged = !m_isFlagged; }
    void reveal() { m_isRevealed = true; }
    void setMine(bool mine) { m_isMine = mine; }

private:
    bool m_isMine = false;
    bool m_isFlagged = false;
    bool m_isRevealed = false;
    int m_adjacentMines = 0;
};

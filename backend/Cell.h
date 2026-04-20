#ifndef CELL_H
#define CELL_H

class Cell {
public:
    bool isMine() const { return m_isMine; }
    bool isFlagged() const { return m_isFlagged; }
    bool isRevealed() const { return m_isRevealed; }
    bool isWronglyFlagged() const { return m_isWronglyFlagged; }
    void setWronglyFlagged(bool wrong) { m_isWronglyFlagged = wrong; }
    int adjacentMines() const { return m_adjacentMines; }
    void toggleFlag() { m_isFlagged = !m_isFlagged; }
    void reveal() { m_isRevealed = true; }
    void setMine(bool mine) { m_isMine = mine; }
    void setAdjacentMines(int count) { m_adjacentMines = count; }

private:
    bool m_isMine = false;
    bool m_isFlagged = false;
    bool m_isRevealed = false;
    bool m_isWronglyFlagged = false;
    int m_adjacentMines = 0;
};

#endif // CELL_H

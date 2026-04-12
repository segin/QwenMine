#ifndef GAME_H
#define GAME_H

#include <QObject>
#include "backend/Board.h"

class Game : public QObject
{
    Q_OBJECT

public:
    enum class State { Playing, Won, Lost };
    
    Game(int width, int height, int mineCount);
    void revealCell(int x, int y);
    void toggleFlag(int x, int y);
    State state() const { return m_state; }
    Board& board() { return m_board; }
    int remainingMines() const;
    
signals:
    void stateChanged();

private:
    Board m_board;
    State m_state;
    void floodFill(int x, int y);
    bool checkWin();
};

#endif // GAME_H

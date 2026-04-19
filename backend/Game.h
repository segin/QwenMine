#ifndef GAME_H
#define GAME_H

#include <QObject>
#include "backend/Board.h"
#include <QTimer>

class Game : public QObject
{
    Q_OBJECT

public:
    enum class State { Playing, Won, Lost };

    Game(int width, int height, int mineCount, QObject *parent = nullptr);
    Game(int difficulty, QObject *parent = nullptr);
    void revealCell(int x, int y);
    void toggleFlag(int x, int y);
    void chord(int x, int y);
    State state() const { return m_state; }
    Board& board() { return m_board; }
    int remainingMines() const;
    void startTimer();
    int elapsedSeconds() const;
    void resetTimer();
    void revealAllMines();

    // Exposed for testing
    void floodFill(int x, int y);
    bool checkWin();

signals:
    void stateChanged();
    void boardInitialized();
    void mineCountAdjusted(int requested, int actual);

private:
    static Board createBoard(int difficulty);

    State m_state;
    Board m_board;
    int m_elapsedSeconds;
    QTimer *m_timer;
    bool m_firstMove;
    bool m_timerStarted;
    int m_revealedCount;
    int m_flagCount;
};

#endif // GAME_H

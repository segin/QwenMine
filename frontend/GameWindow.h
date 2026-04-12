#ifndef GAMEWINDOW_H
#define GAMEWINDOW_H

#include <QMainWindow>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include "frontend/BoardView.h"
#include "backend/Game.h"

class GameWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit GameWindow(QWidget *parent = nullptr);

private:
    void initUI();
    void resetGame();

    BoardView* m_boardView;
    Game m_game;
    QLabel* m_mineCounter;
    QLabel* m_statusLabel;
    QPushButton* m_newGameButton;
    int m_cellSize;
};

#endif // GAMEWINDOW_H

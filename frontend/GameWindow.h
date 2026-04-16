#ifndef GAMEWINDOW_H
#define GAMEWINDOW_H

#include <QMainWindow>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QMenuBar>
#include <QMenu>
#include <QStatusBar>
#include <QAction>
#include <QActionGroup>
#include "frontend/BoardView.h"
#include "backend/Game.h"

class GameWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit GameWindow(QWidget *parent = nullptr);
    ~GameWindow();

private:
    void initUI();
    void resetGame();
    void lockWindowSize();
    void handleCellClick(int x, int y);
    void updateStatus();
    void onStateChanged();
    void createMenuBar();
    void createStatusBar();

    BoardView* m_boardView;
    Game* m_game;
    QLabel* m_mineCounter;
    QLabel* m_timerLabel;
    int m_cellSize;

    QActionGroup* m_difficultyGroup;
    QAction* m_actionNew;
    QAction* m_actionExit;
    QAction* m_actionEasy;
    QAction* m_actionMedium;
    QAction* m_actionHard;
};

#endif // GAMEWINDOW_H

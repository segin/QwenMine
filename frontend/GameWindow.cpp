#include "frontend/GameWindow.h"
#include <QApplication>
#include <QMouseEvent>
#include <QTimer>

GameWindow::GameWindow(QWidget *parent)
    : QMainWindow(parent), m_game(9, 9, 10), m_cellSize(25)
{
    initUI();
}

void GameWindow::initUI()
{
    QWidget* centralWidget = new QWidget(this);
    QVBoxLayout* mainLayout = new QVBoxLayout(centralWidget);
    
    m_boardView = new BoardView(&m_game, centralWidget);
    m_boardView->setFixedSize(m_game.board().width() * m_cellSize + 2, 
                              m_game.board().height() * m_cellSize + 2);
    
    QHBoxLayout* infoLayout = new QHBoxLayout();
    m_mineCounter = new QLabel(QString::number(m_game.remainingMines()));
    m_mineCounter->setMinimumWidth(60);
    m_statusLabel = new QLabel("Playing");
    m_newGameButton = new QPushButton("New Game");
    
    infoLayout->addWidget(m_mineCounter);
    infoLayout->addStretch();
    infoLayout->addWidget(m_statusLabel);
    infoLayout->addWidget(m_newGameButton);
    
    connect(m_newGameButton, SIGNAL(clicked()), this, SLOT(resetGame()));
    connect(m_boardView, SIGNAL(cellClicked(int, int)), this, SLOT(handleCellClick(int, int)));
    connect(&m_game, SIGNAL(stateChanged()), this, SLOT(updateStatus()));
    
    mainLayout->addStretch();
    mainLayout->addWidget(m_boardView);
    mainLayout->addStretch();
    mainLayout->addLayout(infoLayout);
    
    setCentralWidget(centralWidget);
    setWindowTitle("QwenMine");
    resize(400, 450);
}

void GameWindow::resetGame()
{
    m_game = Game(9, 9, 10);
    m_boardView->update();
    m_mineCounter->setText(QString::number(m_game.remainingMines()));
    m_statusLabel->setText("Playing");
}

void GameWindow::handleCellClick(int x, int y)
{
    m_game.revealCell(x, y);
    m_boardView->update();
}

void GameWindow::updateStatus()
{
    m_mineCounter->setText(QString::number(m_game.remainingMines()));
    
    switch(m_game.state()) {
    case Game::State::Won:
        m_statusLabel->setText("You Win!");
        break;
    case Game::State::Lost:
        m_statusLabel->setText("Game Over!");
        break;
    default:
        m_statusLabel->setText("Playing");
        break;
    }
}

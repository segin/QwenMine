#include "frontend/GameWindow.h"

#include <QMenuBar>
#include <QMenu>
#include <QStatusBar>
#include <QAction>
#include <QSignalBlocker>

static constexpr int BORDER_PAD = 2 * BoardView::GRID_LINE;

GameWindow::GameWindow(QWidget *parent)
    : QMainWindow(parent), m_cellSize(BoardView::CELL_SIZE)
{
    initUI();
}

void GameWindow::createMenuBar()
{
    m_actionNew = new QAction("New Game", this);
    m_actionNew->setShortcut(QKeySequence("Ctrl+N"));

    m_actionExit = new QAction("Exit", this);
    m_actionExit->setShortcut(QKeySequence("Ctrl+Q"));

    m_actionEasy = new QAction("Easy", this);
    m_actionEasy->setCheckable(true);
    m_actionEasy->setChecked(true);
    m_actionEasy->setShortcut(QKeySequence("Ctrl+1"));

    m_actionMedium = new QAction("Medium", this);
    m_actionMedium->setCheckable(true);
    m_actionMedium->setShortcut(QKeySequence("Ctrl+2"));

    m_actionHard = new QAction("Hard", this);
    m_actionHard->setCheckable(true);
    m_actionHard->setShortcut(QKeySequence("Ctrl+3"));

    // Group difficulty actions for mutual exclusivity.
    // Block signals so setChecked(true) above doesn't trigger resetGame()
    // before m_game has been created in initUI().
    m_difficultyGroup = new QActionGroup(this);
    m_difficultyGroup->setExclusive(true);
    {
        QSignalBlocker blocker(m_difficultyGroup);
        m_difficultyGroup->addAction(m_actionEasy);
        m_difficultyGroup->addAction(m_actionMedium);
        m_difficultyGroup->addAction(m_actionHard);
    }

    QMenu *fileMenu = menuBar()->addMenu("File");
    fileMenu->addAction(m_actionNew);
    fileMenu->addSeparator();
    fileMenu->addAction(m_actionExit);

    QMenu *difficultyMenu = menuBar()->addMenu("Difficulty");
    difficultyMenu->addAction(m_actionEasy);
    difficultyMenu->addAction(m_actionMedium);
    difficultyMenu->addAction(m_actionHard);

    connect(m_actionNew, &QAction::triggered, this, &GameWindow::resetGame);
    connect(m_actionExit, &QAction::triggered, this, &GameWindow::close);
    connect(m_difficultyGroup, &QActionGroup::triggered, this, &GameWindow::resetGame);
}

void GameWindow::createStatusBar()
{
    m_mineCounter = new QLabel("Mines: 0");
    m_timerLabel = new QLabel("Time: 0s");
    statusBar()->addWidget(m_mineCounter);
    statusBar()->addWidget(m_timerLabel);
}

void GameWindow::initUI()
{
    createMenuBar();
    createStatusBar();

    // Game is parented to this window, so it's auto-deleted on close.
    m_game = new Game(0, this);

    m_centralWidget = new QWidget(this);
    QVBoxLayout *mainLayout = new QVBoxLayout(m_centralWidget);
    mainLayout->setContentsMargins(0, 0, 0, 0);
    mainLayout->setSpacing(0);

    m_boardView = new BoardView(m_game, m_centralWidget);
    m_boardView->setFixedWidth(m_game->board().width() * m_cellSize + BORDER_PAD);
    m_boardView->setFixedHeight(m_game->board().height() * m_cellSize + BORDER_PAD);

    mainLayout->addWidget(m_boardView, 0, Qt::AlignHCenter);

    // Info bar below the board
    QHBoxLayout *infoLayout = new QHBoxLayout();
    infoLayout->setContentsMargins(8, 4, 8, 4);
    QPushButton *newGameButton = new QPushButton("New Game");
    infoLayout->addStretch();
    infoLayout->addWidget(newGameButton);
    infoLayout->addStretch();
    mainLayout->addLayout(infoLayout, 0);

    // Spacer so the board stays at the top and doesn't stretch
    mainLayout->addStretch();

    setCentralWidget(m_centralWidget);
    setWindowTitle("QwenMine");

    // Lock window to exact content size — no resize handles
    lockWindowSize();

    // Connections
    connect(newGameButton, &QPushButton::clicked, this, &GameWindow::resetGame);
    connect(m_boardView, &BoardView::cellClicked, this, &GameWindow::handleCellClick);
    connect(m_game, &Game::stateChanged, this, &GameWindow::onStateChanged);
    connect(m_game, &Game::boardInitialized, this, &GameWindow::startGameTimer);

    updateStatus();
}

void GameWindow::lockWindowSize()
{
    // Force the window to the exact size of its contents.
    // adjustSize() computes the minimum size needed for the central widget,
    // menu bar, and status bar. setFixedSize() then prevents user resizing.
    adjustSize();
    setFixedSize(size());
}

void GameWindow::onStateChanged()
{
    updateStatus();

    // On game over reveal all mines
    if (m_game->state() == Game::State::Lost || m_game->state() == Game::State::Won) {
        m_game->revealAllMines();
        m_boardView->update();
    }
}

void GameWindow::resetGame()
{
    int difficulty = 0; // Easy (default)
    if (m_actionMedium->isChecked()) difficulty = 1;
    else if (m_actionHard->isChecked()) difficulty = 2;

    auto oldGame = m_game;
    disconnect(oldGame, nullptr, this, nullptr);
    oldGame->deleteLater();

    // Create new game parented to this window
    m_game = new Game(difficulty, this);

    // Re-establish signal-slot connections for the new game object
    connect(m_game, &Game::stateChanged, this, &GameWindow::onStateChanged);
    connect(m_game, &Game::boardInitialized, this, &GameWindow::startGameTimer);
    connect(m_game, &Game::mineCountAdjusted, this, [this](int, int actual) {
        m_mineCounter->setText("Mines: " + QString::number(actual));
    });

    // Update BoardView's internal pointer so it doesn't dangle,
    // and resize the widget to fit the new board.
    const int bw = m_game->board().width() * m_cellSize + BORDER_PAD;
    const int bh = m_game->board().height() * m_cellSize + BORDER_PAD;
    m_boardView->setGame(m_game);
    m_boardView->setFixedWidth(bw);
    m_boardView->setFixedHeight(bh);
    m_boardView->updateGeometry();
    m_boardView->update();

    // Resize window to fit the new board dimensions.
    m_centralWidget->layout()->activate();
    lockWindowSize();

    updateStatus();
}

void GameWindow::handleCellClick(int x, int y)
{
    m_game->revealCell(x, y);
    m_boardView->update();
}

void GameWindow::updateStatus()
{
    m_mineCounter->setText("Mines: " + QString::number(m_game->remainingMines()));
    m_timerLabel->setText("Time: " + QString::number(m_game->elapsedSeconds()) + "s");

    switch (m_game->state()) {
    case Game::State::Won:
        statusBar()->showMessage("You Win!", 5000);
        break;
    case Game::State::Lost:
        statusBar()->showMessage("Game Over!", 5000);
        break;
    default:
        break;
    }
}

void GameWindow::startGameTimer()
{
    m_game->startTimer();
}

GameWindow::~GameWindow()
{
    // m_game is a child of this (set via QObject parent in constructor),
    // so Qt auto-deletes it. Same for m_boardView (child of centralWidget).
    // Nothing to do here.
}

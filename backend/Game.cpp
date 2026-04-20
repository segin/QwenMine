#include "backend/Game.h"
#include <cassert>
#include <queue>

static void boardDimensions(int difficulty, int &width, int &height, int &mines)
{
    assert(difficulty >= 0 && difficulty <= 2);

    switch (difficulty) {
    case 0: // Easy
        width = 9;  height = 9;  mines = 10; break;
    case 1: // Medium
        width = 16; height = 16; mines = 40; break;
    case 2: // Hard
        width = 30; height = 16; mines = 66; break;
    default:
        width = 9;  height = 9;  mines = 10; break;
    }
}

Game::Game(int width, int height, int mineCount, QObject *parent)
    : QObject(parent), m_state(State::Playing), m_board(width, height, mineCount),
      m_elapsedSeconds(0), m_firstMove(true), m_timerStarted(false),
      m_revealedCount(0), m_flagCount(0)
{
    m_timer = new QTimer(this);
    m_timer->setInterval(1000);
    connect(m_timer, &QTimer::timeout, this, [this]() {
        ++m_elapsedSeconds;
        emit stateChanged();
    });
}

Game::Game(int difficulty, QObject *parent)
    : QObject(parent), m_state(State::Playing), m_board(createBoard(difficulty)),
      m_elapsedSeconds(0), m_firstMove(true), m_timerStarted(false),
      m_revealedCount(0), m_flagCount(0)
{
    m_timer = new QTimer(this);
    m_timer->setInterval(1000);
    connect(m_timer, &QTimer::timeout, this, [this]() {
        ++m_elapsedSeconds;
        emit stateChanged();
    });
}

Board Game::createBoard(int difficulty)
{
    int w, h, m;
    boardDimensions(difficulty, w, h, m);
    return Board(w, h, m);
}

void Game::revealCell(int x, int y)
{
    if (m_state != State::Playing) return;

    Cell& cell = m_board.cell(x, y);
    if (cell.isRevealed() || cell.isFlagged()) return;

    if (m_firstMove) {
        m_board.initialize(x, y);
        if (m_board.mineCount() != m_board.actualMineCount())
            emit mineCountAdjusted(m_board.mineCount(), m_board.actualMineCount());
        m_firstMove = false;
        emit boardInitialized();
    }

    if (cell.isRevealed() || cell.isFlagged()) return;

    cell.reveal();
    ++m_revealedCount;

    if (cell.isMine()) {
        m_state = State::Lost;
        emit stateChanged();
        m_timer->stop();
        return;
    }

    if (cell.adjacentMines() == 0) {
        floodFill(x, y);
    }

    checkWin();
}

void Game::toggleFlag(int x, int y)
{
    if (m_state != State::Playing) return;

    Cell& cell = m_board.cell(x, y);
    if (cell.isRevealed()) return;

    cell.toggleFlag();
    if (cell.isFlagged())
        ++m_flagCount;
    else
        --m_flagCount;
    emit stateChanged();
}

void Game::chord(int x, int y)
{
    if (m_state != State::Playing) return;

    Cell& cell = m_board.cell(x, y);
    if (!cell.isRevealed() || cell.adjacentMines() == 0) return;

    std::vector<std::pair<int, int>> directions = {{-1,0}, {1,0}, {0,-1}, {0,1}, {-1,-1}, {-1,1}, {1,-1}, {1,1}};

    int flagCount = 0;
    for (auto &d : directions) {
        int nx = x + d.first;
        int ny = y + d.second;
        if (nx < 0 || nx >= m_board.width() || ny < 0 || ny >= m_board.height()) continue;
        if (m_board.cell(nx, ny).isFlagged()) ++flagCount;
    }

    if (flagCount == cell.adjacentMines()) {
        for (auto &d : directions) {
            int nx = x + d.first;
            int ny = y + d.second;
            if (nx < 0 || nx >= m_board.width() || ny < 0 || ny >= m_board.height()) continue;
            Cell& neighbor = m_board.cell(nx, ny);
            if (!neighbor.isRevealed() && !neighbor.isFlagged()) {
                revealCell(nx, ny);
            }
        }
    }
}

void Game::floodFill(int x, int y)
{
    struct Coord { int x, y; };
    std::vector<std::pair<int, int>> directions = {{-1,0}, {1,0}, {0,-1}, {0,1}, {-1,-1}, {-1,1}, {1,-1}, {1,1}};

    std::queue<Coord> queue;
    queue.push({x, y});

    while (!queue.empty()) {
        Coord c = queue.front();
        queue.pop();

        for (auto &d : directions) {
            int nx = c.x + d.first;
            int ny = c.y + d.second;

            if (nx < 0 || nx >= m_board.width() || ny < 0 || ny >= m_board.height()) continue;

            Cell& cell = m_board.cell(nx, ny);
            if (cell.isRevealed() || cell.isFlagged()) continue;
            if (cell.isMine()) continue;

            cell.reveal();
            ++m_revealedCount;
            if (cell.adjacentMines() > 0) continue;
            queue.push({nx, ny});
        }
    }
}

  bool Game::checkWin()
{
    int total = m_board.width() * m_board.height();

    if (m_revealedCount == total - m_board.actualMineCount()) {
        m_state = State::Won;
        emit stateChanged();
        m_timer->stop();
        return true;
    }

    return false;
}

int Game::remainingMines() const
{
    return m_board.actualMineCount() - m_flagCount;
}

void Game::startTimer()
{
    if (m_timerStarted) return;
    m_timerStarted = true;
    m_timer->start();
}

int Game::elapsedSeconds() const
{
    return m_elapsedSeconds;
}

void Game::resetTimer()
{
    m_timer->stop();
    m_elapsedSeconds = 0;
    m_timerStarted = false;
}

void Game::revealAllMines()
{
    for (int y = 0; y < m_board.height(); ++y)
        for (int x = 0; x < m_board.width(); ++x) {
            Cell &cell = m_board.cell(x, y);
            if (cell.isMine()) {
                cell.reveal();
            }
            if (cell.isFlagged() && !cell.isMine()) {
                cell.setWronglyFlagged(true);
                cell.reveal();
            }
        }
}

void Game::autoFlagRemainingMines()
{
    for (int y = 0; y < m_board.height(); ++y)
        for (int x = 0; x < m_board.width(); ++x) {
            Cell& cell = m_board.cell(x, y);
            if (cell.isMine() && !cell.isFlagged() && !cell.isRevealed()) {
                cell.toggleFlag();
                ++m_flagCount;
            }
        }
}

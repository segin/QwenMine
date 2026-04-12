#include "backend/Game.h"
#include <queue>

Game::Game(int width, int height, int mineCount)
    : m_state(State::Playing)
{
    m_board = Board(width, height, mineCount);
    m_board.initialize();
}

void Game::revealCell(int x, int y)
{
    if (m_state != State::Playing) return;
    
    Cell& cell = m_board.cell(x, y);
    if (cell.isRevealed() || cell.isFlagged()) return;
    
    cell.reveal();
    
    if (cell.isMine()) {
        m_state = State::Lost;
        emit stateChanged();
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
    m_board.cell(x, y).toggleFlag();
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
            
            cell.reveal();
            if (cell.adjacentMines() > 0) continue;
            queue.push({nx, ny});
        }
    }
    
    checkWin();
}

bool Game::checkWin()
{
    int revealed = 0;
    int total = m_board.width() * m_board.height();
    
    for (int y = 0; y < m_board.height(); ++y)
        for (int x = 0; x < m_board.width(); ++x) {
            if (!m_board.cell(x, y).isRevealed()) break;
            ++revealed;
        }
    
    if (revealed == total - m_board.mineCount()) {
        m_state = State::Won;
        emit stateChanged();
        return true;
    }
    
    return false;
}

int Game::remainingMines() const
{
    int count = 0;
    for (int y = 0; y < m_board.height(); ++y)
        for (int x = 0; x < m_board.width(); ++x)
            if (m_board.cell(x, y).isFlagged()) ++count;
    
    return m_board.mineCount() - count;
}

# QwenMine Bug Fixes Tasklist

## High Priority Defects

- [x] Fix GameWindow::resetGame() - reconnect signals after new game creation
- [x] Stop QTimer on game over (win/loss) in Game::revealCell and Game::checkWin
- [x] Fix early flags being erased on first click (Board::initialize)
- [x] Fix mine count desync - update m_mineCount when capped (Board::initialize)
- [x] Connect Game::mineCountAdjusted signal in GameWindow

## Architectural Fixes

- [x] Optimize checkWin() to use m_revealedCount (O(1))
- [x] Optimize remainingMines() to use m_flagCount (O(1))
- [x] Track m_revealedCount in floodFill()

## Build & Test

- [x] Build project
- [x] Test all fixes

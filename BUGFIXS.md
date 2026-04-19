# QwenMine Bug Fixes Tasklist

## High Priority Defects

- [x] Fix GameWindow::resetGame() - reconnect signals after new game creation
- [ ] Stop QTimer on game over (win/loss) in Game::revealCell and Game::checkWin
- [ ] Fix early flags being erased on first click (Board::initialize)
- [ ] Fix mine count desync - update m_mineCount when capped (Board::initialize)
- [ ] Connect Game::mineCountAdjusted signal in GameWindow

## Architectural Fixes

- [ ] Optimize checkWin() to use m_revealedCount (O(1))
- [ ] Optimize remainingMines() to use m_flagCount (O(1))
- [ ] Track m_revealedCount in floodFill()

## Build & Test

- [ ] Build project
- [ ] Test all fixes

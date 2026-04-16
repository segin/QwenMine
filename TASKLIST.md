# QwenMine Remediation Tasklist

This document outlines the necessary fixes and improvements for the QwenMine project. Each task includes a user story, formal requirements in INCOSE/EARS format, and verification steps.

---

## 1. Fix Win Condition Logic
**User Story:** As a player, I want the game to end and declare a win when I have revealed all safe cells so that I know I have completed the challenge.

**Requirement (EARS):**
- **While** the game is in progress, **if** the number of revealed cells equals the total number of cells minus the mine count, the system **shall** set the game state to `Won`.

**Verification:**
1. Create a 2x2 board with 1 mine.
2. Reveal all 3 safe cells.
3. Assert that `Game::state()` returns `State::Won`.

---

## 2. Fix Board Mine Placement Logic
**User Story:** As a developer, I want the board to be initialized consistently so that the number of mines matches the difficulty settings exactly, regardless of how many times it's reset.

**Requirement (EARS):**
- **When** `Board::initialize()` is called, the system **shall** clear all existing mine states from all cells before placing new mines.

**Verification:**
1. Call `Board::initialize()` on an existing board.
2. Verify that the total number of mines is exactly `m_mineCount`.
3. Call `Board::initialize()` a second time and verify the count remains exactly `m_mineCount`.

---

## 3. Refactor First-Click Safety
**User Story:** As a player, I want my first click to never be a mine so that I have a fair start without the entire board being regenerated and shuffled.

**Requirement (EARS):**
- **When** a player reveals their first cell of a game, **if** that cell contains a mine, the system **shall** move the mine to the first available non-mine cell and update adjacency counts before proceeding with the reveal.

**Verification:**
1. Force a mine at (0,0).
2. Call `Game::revealCell(0,0)` as the first move.
3. Assert that `Game::state()` is `Playing`.
4. Assert that `Board::cell(0,0).isMine()` is `false`.

---

## 4. Optimize Game Initialization
**User Story:** As a developer, I want the `Game` object to be initialized efficiently to avoid redundant memory allocations for the board.

**Requirement (EARS):**
- **During** `Game` construction, the system **shall** initialize the `m_board` member exactly once using the member initializer list.

**Verification:**
1. Audit `Game.cpp` constructors.
2. Ensure no re-assignment of `m_board` occurs within the constructor body.

---

## 5. Correct Timer Signal Logic
**User Story:** As a player, I want the game timer to track my time accurately without accelerating if the game is restarted.

**Requirement (EARS):**
- **When** `Game::startTimer()` is called, the system **shall** ensure exactly one connection exists between the `QTimer::timeout` signal and the elapsed time increment logic.

**Verification:**
1. Start the game and observe the timer.
2. Reset and start the game again.
3. Assert that `m_elapsedSeconds` increments by exactly 1 every second.

---

## 6. Complete Flagging Implementation
**User Story:** As a player, I want to be able to flag suspected mines to prevent accidental clicks and track my progress.

**Requirement (EARS):**
- **When** the player right-clicks an unrevealed cell, the system **shall** toggle the cell's `isFlagged` state.
- **While** a cell is flagged, the system **shall** ignore any left-click (reveal) actions on that cell.

**Verification:**
1. Right-click a cell and verify `isFlagged()` is true.
2. Left-click the flagged cell and verify `isRevealed()` remains false.
3. Right-click again to unflag, then left-click and verify the cell reveals.

---

## 7. Establish Unit Testing Suite
**User Story:** As a developer, I want a suite of automated tests to ensure the core game logic remains correct during future refactoring.

**Requirement (EARS):**
- **The** system **shall** include unit tests covering Win/Loss conditions, Flood Fill accuracy, and Mine Placement logic.

**Verification:**
1. Execute `ctest` or the project's test runner.
2. Assert that all tests pass.

#include <QtTest>
#include "backend/Cell.h"
#include "backend/Board.h"
#include "backend/Game.h"

// ==================== Board Tests ====================

class BoardTests : public QObject
{
    Q_OBJECT
private slots:
    void testCellInitialState();
    void testCellToggleFlag();
    void testCellReveal();
    void testCellMine();
    void testCellAdjacentMines();
    void testBoardDefaultConstructor();
    void testBoardCustomConstructor();
    void testBoardInitialize();
    void testBoardMinePlacement();
    void testBoardCellAccess();
    void testBoardCalculateAdjacentMines();
    void testBoardInitializeClearsState();
};

void BoardTests::testCellInitialState()
{
    Cell cell;
    QVERIFY(!cell.isMine());
    QVERIFY(!cell.isFlagged());
    QVERIFY(!cell.isRevealed());
    QCOMPARE(cell.adjacentMines(), 0);
}

void BoardTests::testCellToggleFlag()
{
    Cell cell;
    QVERIFY(!cell.isFlagged());
    cell.toggleFlag();
    QVERIFY(cell.isFlagged());
    cell.toggleFlag();
    QVERIFY(!cell.isFlagged());
}

void BoardTests::testCellReveal()
{
    Cell cell;
    QVERIFY(!cell.isRevealed());
    cell.reveal();
    QVERIFY(cell.isRevealed());
}

void BoardTests::testCellMine()
{
    Cell cell;
    QVERIFY(!cell.isMine());
    cell.setMine(true);
    QVERIFY(cell.isMine());
    cell.setMine(false);
    QVERIFY(!cell.isMine());
}

void BoardTests::testCellAdjacentMines()
{
    Cell cell;
    QCOMPARE(cell.adjacentMines(), 0);
    cell.setAdjacentMines(5);
    QCOMPARE(cell.adjacentMines(), 5);
}

void BoardTests::testBoardDefaultConstructor()
{
    Board board;
    QCOMPARE(board.width(), 9);
    QCOMPARE(board.height(), 9);
    QCOMPARE(board.mineCount(), 10);
}

void BoardTests::testBoardCustomConstructor()
{
    Board board(16, 16, 40);
    QCOMPARE(board.width(), 16);
    QCOMPARE(board.height(), 16);
    QCOMPARE(board.mineCount(), 40);
}

void BoardTests::testBoardInitialize()
{
    Board board(5, 5, 10);
    QVERIFY(!board.cell(0, 0).isRevealed());
    QVERIFY(!board.cell(2, 2).isFlagged());
}

void BoardTests::testBoardMinePlacement()
{
    Board board(5, 5, 10);
    board.initialize();

    int mineCount = 0;
    for (int y = 0; y < board.height(); ++y)
        for (int x = 0; x < board.width(); ++x)
            if (board.cell(x, y).isMine()) ++mineCount;

    QCOMPARE(mineCount, board.mineCount());
}

void BoardTests::testBoardCellAccess()
{
    Board board(3, 3, 3);
    board.initialize();

    board.cell(0, 0).setMine(true);
    QVERIFY(board.cell(0, 0).isMine());

    board.cell(1, 1).reveal();
    QVERIFY(board.cell(1, 1).isRevealed());
}

void BoardTests::testBoardCalculateAdjacentMines()
{
    Board board(3, 3, 2);
    board.cell(0, 0).setMine(true);
    board.cell(0, 1).setMine(true);
    board.calculateAdjacentMines();

    // Cell (0,0) neighbors: (0,1)=mine, (1,0), (1,1) -> 1 adjacent mine
    QCOMPARE(board.cell(1, 1).adjacentMines(), 2);
    QCOMPARE(board.cell(1, 0).adjacentMines(), 2);
    QCOMPARE(board.cell(0, 0).isMine(), true);
    QCOMPARE(board.cell(0, 1).isMine(), true);
}

void BoardTests::testBoardInitializeClearsState()
{
    Board board(3, 3, 1);
    board.initialize();

    board.cell(0, 0).reveal();
    board.cell(0, 1).toggleFlag();

    board.initialize();

    QVERIFY(!board.cell(0, 0).isRevealed());
    QVERIFY(!board.cell(0, 1).isFlagged());

    int mineCount = 0;
    for (int y = 0; y < board.height(); ++y)
        for (int x = 0; x < board.width(); ++x)
            if (board.cell(x, y).isMine()) ++mineCount;
    QCOMPARE(mineCount, board.mineCount());
}

// ==================== Game Tests ====================

class GameTests : public QObject
{
    Q_OBJECT
private slots:
    void testWinCondition();
    void testLossCondition();
    void testFloodFillAccuracy();
    void testMinePlacementLogic();
    void testFirstClickSafety();
    void testFlaggedCellNotRevealed();
    void testInitialState();
    void testTimerReset();
    void testToggleFlagEmitsSignal();
};

void GameTests::testWinCondition()
{
    Game game(3, 3, 2);
    game.board().cell(0, 0).setMine(true);
    game.board().cell(2, 2).setMine(true);
    game.board().calculateAdjacentMines();

    for (int y = 0; y < game.board().height(); ++y)
        for (int x = 0; x < game.board().width(); ++x)
            if (!game.board().cell(x, y).isMine())
                game.board().cell(x, y).reveal();

    QVERIFY(game.checkWin());
    QCOMPARE(game.state(), Game::State::Won);
}

void GameTests::testLossCondition()
{
    // Use a 9x9 board with 10 mines so a single click won't
    // flood-fill all safe cells and auto-win the game.
    Game game(9, 9, 10);
    game.revealCell(0, 0);
    QCOMPARE(game.state(), Game::State::Playing);

    // Find a mine and click it
    bool foundMine = false;
    for (int y = 0; y < game.board().height() && !foundMine; ++y)
        for (int x = 0; x < game.board().width() && !foundMine; ++x)
            if (game.board().cell(x, y).isMine()) {
                game.revealCell(x, y);
                QCOMPARE(game.state(), Game::State::Lost);
                foundMine = true;
            }
    if (!foundMine) QFAIL("No mine found on board");
}

void GameTests::testFloodFillAccuracy()
{
    Board board(3, 3, 2);
    board.cell(0, 1).setMine(true);
    board.cell(2, 1).setMine(true);
    board.calculateAdjacentMines();

    QCOMPARE(board.cell(1, 1).adjacentMines(), 2);
}

void GameTests::testMinePlacementLogic()
{
    Board board(5, 5, 10);
    board.initialize();

    int mineCount = 0;
    for (int y = 0; y < board.height(); ++y)
        for (int x = 0; x < board.width(); ++x)
            if (board.cell(x, y).isMine()) ++mineCount;

    QCOMPARE(mineCount, board.mineCount());
}

void GameTests::testFirstClickSafety()
{
    Game game(5, 5, 10);

    game.revealCell(2, 2);
    QCOMPARE(game.state(), Game::State::Playing);
    QVERIFY(!game.board().cell(2, 2).isMine());
    QVERIFY(game.board().cell(2, 2).isRevealed());
}

void GameTests::testFlaggedCellNotRevealed()
{
    Game game(9, 9, 10);

    // First click to initialize the board
    game.revealCell(0, 0);
    QCOMPARE(game.state(), Game::State::Playing);

    // Find an unrevealed, non-mine cell to flag
    int flagX = -1, flagY = -1;
    for (int y = 0; y < game.board().height(); ++y)
        for (int x = 0; x < game.board().width(); ++x) {
            if (!game.board().cell(x, y).isRevealed() && !game.board().cell(x, y).isMine()) {
                flagX = x; flagY = y;
                y = game.board().height(); // break outer loop
                break;
            }
        }
    QVERIFY(flagX >= 0);

    // Flag the cell
    game.toggleFlag(flagX, flagY);
    QVERIFY(game.board().cell(flagX, flagY).isFlagged());

    // Try to reveal flagged cell — should be ignored
    game.revealCell(flagX, flagY);
    QVERIFY(!game.board().cell(flagX, flagY).isRevealed());

    // Unflag and reveal
    game.toggleFlag(flagX, flagY);
    QVERIFY(!game.board().cell(flagX, flagY).isFlagged());
    game.revealCell(flagX, flagY);
    QVERIFY(game.board().cell(flagX, flagY).isRevealed());
}

void GameTests::testInitialState()
{
    Game game(0);
    QCOMPARE(game.state(), Game::State::Playing);
    QCOMPARE(game.elapsedSeconds(), 0);
}

void GameTests::testTimerReset()
{
    Game game(0);
    game.startTimer();
    QCOMPARE(game.elapsedSeconds(), 0);

    game.resetTimer();
    QCOMPARE(game.elapsedSeconds(), 0);

    game.startTimer();
    QCOMPARE(game.elapsedSeconds(), 0);
}

void GameTests::testToggleFlagEmitsSignal()
{
    Game game(9, 9, 10);
    game.revealCell(0, 0); // trigger first-move init

    // Find an unrevealed, non-flagged, non-revealed cell far from (0,0)
    int targetX = -1, targetY = -1;
    for (int y = 0; y < game.board().height(); ++y)
        for (int x = 0; x < game.board().width(); ++x) {
            if (!game.board().cell(x, y).isRevealed() && !game.board().cell(x, y).isFlagged()
                && x >= 4 && y >= 4) {
                targetX = x; targetY = y;
                y = game.board().height();
                break;
            }
        }
    QVERIFY(targetX >= 0);

    QSignalSpy spy(&game, &Game::stateChanged);
    game.toggleFlag(targetX, targetY);
    QCOMPARE(spy.count(), 1);
}

// ==================== Test Runner ====================

int main(int argc, char *argv[])
{
    int result = 0;

    {
        BoardTests boardTests;
        result |= QTest::qExec(&boardTests, argc, argv);
    }

    {
        GameTests gameTests;
        result |= QTest::qExec(&gameTests, argc, argv);
    }

    return result;
}

#include "main.moc"

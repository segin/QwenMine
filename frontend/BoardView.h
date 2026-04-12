#ifndef BOARDVIEW_H
#define BOARDVIEW_H

#include <QWidget>
#include "backend/Board.h"
#include "backend/Game.h"

class BoardView : public QWidget
{
    Q_OBJECT

public:
    explicit BoardView(Game* game, QWidget *parent = nullptr);
    void paintEvent(QPaintEvent *event) override;

signals:
    void cellClicked(int x, int y);

private:
    Game* m_game;
    int m_cellSize;
    void drawCell(QPainter& painter, int x, int y);
};

#endif // BOARDVIEW_H

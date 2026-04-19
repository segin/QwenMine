#ifndef BOARDVIEW_H
#define BOARDVIEW_H

#include <QWidget>
#include "backend/Board.h"
#include "backend/Game.h"

class BoardView : public QWidget
{
    Q_OBJECT

public:
    explicit BoardView(Game *game, QWidget *parent = nullptr);

    void setGame(Game *game);

    void paintEvent(QPaintEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void leaveEvent(QEvent *event) override;
    QSize sizeHint() const override;

signals:
    void cellClicked(int x, int y);

private:
    // Grid geometry
    int boardPixelWidth() const;
    int boardPixelHeight() const;
    int cellToPixelX(int col) const;
    int cellToPixelY(int row) const;
    void pixelToCell(int px, int py, int &col, int &row) const;

    // Cell rendering
    void drawCell(QPainter &painter, int col, int row, bool hovered);
    void drawMine(QPainter &painter, const QRect &rect);
    void drawFlag(QPainter &painter, const QRect &rect);
    void drawNumber(QPainter &painter, const QRect &rect, int number);

    Game *m_game;

    // Hover tracking
    int m_hoverCol;
    int m_hoverRow;

public:
    static constexpr int CELL_SIZE = 30;
    static constexpr int GRID_LINE = 1;
};

#endif // BOARDVIEW_H

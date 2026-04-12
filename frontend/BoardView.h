#ifndef BOARDVIEW_H
#define BOARDVIEW_H

#include <QWidget>
#include "backend/Board.h"

class BoardView : public QWidget
{
    Q_OBJECT

public:
    explicit BoardView(Board *board, QWidget *parent = nullptr);

protected:
    void mousePressEvent(QMouseEvent *event) override;

private:
    Board *m_board;
    int m_cellSize;
};

#endif // BOARDVIEW_H

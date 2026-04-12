#include "frontend/BoardView.h"
#include <QMouseEvent>

BoardView::BoardView(Board *board, QWidget *parent)
    : QWidget(parent), m_board(board), m_cellSize(25)
{
    setFixedSize(m_board->width() * m_cellSize + 2, m_board->height() * m_cellSize + 2);
}

void BoardView::mousePressEvent(QMouseEvent *event)
{
    if (event->button() != Qt::LeftButton) return;
    
    int x = (event->pos().x() - 1) / m_cellSize;
    int y = (event->pos().y() - 1) / m_cellSize;
    
    if (x >= 0 && x < m_board->width() && y >= 0 && y < m_board->height())
        m_board->revealCell(x, y);
    
    update();
}

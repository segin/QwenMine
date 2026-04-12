#include "frontend/BoardView.h"
#include <QPainter>
#include <QMouseEvent>
#include <QTimer>

BoardView::BoardView(Game* game, QWidget *parent)
    : QWidget(parent), m_game(game), m_cellSize(25)
{
    setFixedSize(m_game->board().width() * m_cellSize + 2, 
                 m_game->board().height() * m_cellSize + 2);
}

void BoardView::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);
    QPainter painter(this);
    
    painter.fillRect(rect(), Qt::gray);
    painter.setPen(Qt::black);
    
    int margin = 1;
    int width = m_game->board().width();
    int height = m_game->board().height();
    
    for (int x = 0; x < width; ++x) {
        for (int y = 0; y < height; ++y) {
            Cell& cell = m_game->board().cell(x, y);
            QRect cellRect(margin + x * m_cellSize + 1, margin + y * m_cellSize + 1, 
                          m_cellSize - 2, m_cellSize - 2);
            
            painter.setBrush(cell.isFlagged() ? Qt::red : Qt::white);
            painter.setPen(Qt::black);
            painter.drawRect(cellRect);
            
            if (cell.isRevealed()) {
                painter.setPen(cell.isMine() ? Qt::red : Qt::black);
                painter.drawText(cellRect, Qt::AlignCenter, QString::number(cell.adjacentMines()));
            }
        }
    }
    
    painter.fillRect(rect(), Qt::gray);
    
    painter.setPen(Qt::black);
    painter.drawRect(margin - 1, margin - 1, width * m_cellSize + 2, height * m_cellSize + 2);
}

void BoardView::mousePressEvent(QMouseEvent *event)
{
    if (event->button() != Qt::LeftButton) return;
    
    int x = (event->pos().x() - 1) / m_cellSize;
    int y = (event->pos().y() - 1) / m_cellSize;
    
    if (x >= 0 && x < m_game->board().width() && y >= 0 && y < m_game->board().height()) {
        m_game->revealCell(x, y);
        update();
        emit cellClicked(x, y);
    }
}

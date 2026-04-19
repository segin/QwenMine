#include "frontend/BoardView.h"
#include <QPainter>
#include <QMouseEvent>
#include <QEvent>

// ============================================================
// Construction & game swap
// ============================================================

BoardView::BoardView(Game *game, QWidget *parent)
    : QWidget(parent), m_game(game), m_hoverCol(-1), m_hoverRow(-1)
{
    setMinimumSize(boardPixelWidth() + 2 * GRID_LINE,
                   boardPixelHeight() + 2 * GRID_LINE);
    setMouseTracking(true);
}

void BoardView::setGame(Game *game)
{
    m_game = game;
    m_hoverCol = -1;
    m_hoverRow = -1;
    setMinimumSize(boardPixelWidth() + 2 * GRID_LINE,
                   boardPixelHeight() + 2 * GRID_LINE);
   update();
}

// ============================================================
// Size hint
// ============================================================

QSize BoardView::sizeHint() const {
    return QSize(boardPixelWidth() + 2 * GRID_LINE,
                 boardPixelHeight() + 2 * GRID_LINE);
}

// ============================================================
// Mouse events
// ============================================================
// Grid geometry helpers
// ============================================================

int BoardView::boardPixelWidth() const {
    return m_game->board().width() * CELL_SIZE;
}

int BoardView::boardPixelHeight() const {
    return m_game->board().height() * CELL_SIZE;
}

int BoardView::cellToPixelX(int col) const {
    return GRID_LINE + col * CELL_SIZE;
}

int BoardView::cellToPixelY(int row) const {
    return GRID_LINE + row * CELL_SIZE;
}

void BoardView::pixelToCell(int px, int py, int &col, int &row) const {
    col = (px - GRID_LINE) / CELL_SIZE;
    row = (py - GRID_LINE) / CELL_SIZE;
}

// ============================================================
// Painting helpers
// ============================================================

static QColor numberColor(int n) {
    switch (n) {
        case 1: return QColor(0,   0, 255);
        case 2: return QColor(0, 130,   0);
        case 3: return QColor(255,  0,   0);
        case 4: return QColor(0,   0, 128);
        case 5: return QColor(128,  0,   0);
        case 6: return QColor(0, 128, 128);
        case 7: return QColor(0,   0,   0);
        case 8: return QColor(128, 128, 128);
        default: return Qt::black;
    }
}

void BoardView::drawMine(QPainter &painter, const QRect &rect) {
    const int sz = rect.width();
    const int margin = sz / 6;
    const int cx = rect.left() + sz / 2;
    const int cy = rect.top() + sz / 2;
    const int r = sz / 2 - margin;

    // Body
    painter.setPen(QPen(Qt::black, 2));
    painter.setBrush(QBrush(Qt::black));
    painter.drawEllipse(QPoint(cx, cy), r, r);

    // Red fuse dot at top-right quadrant
    const int fuseR = r / 3;
    painter.setPen(Qt::NoPen);
    painter.setBrush(QBrush(QColor(220, 40, 40)));
    painter.drawEllipse(QPoint(cx + r / 2, cy - r / 2), fuseR, fuseR);
}

void BoardView::drawFlag(QPainter &painter, const QRect &rect) {
    const int sz = rect.width();
    const int cx = rect.left() + sz / 2;
    const int top = rect.top() + sz / 5;
    const int bot = rect.bottom() - sz / 5;

    // Pole
    painter.setPen(QPen(Qt::black, 2));
    painter.drawLine(QLine(cx, top, cx, bot));

    // Flag triangle
    const int flagW = sz / 3;
    const int flagH = sz / 4;
    QPolygon flag;
    flag << QPoint(cx, top + 1)
         << QPoint(cx + flagW, top + flagH / 2)
         << QPoint(cx, top + flagH);
    painter.setBrush(QBrush(QColor(220, 40, 40)));
    painter.setPen(Qt::NoPen);
    painter.drawPolygon(flag);
}

void BoardView::drawNumber(QPainter &painter, const QRect &rect, int number) {
    painter.setPen(numberColor(number));
    QFont font = painter.font();
    font.setBold(true);
    font.setPixelSize(CELL_SIZE * 2 / 3);
    painter.setFont(font);
    painter.drawText(rect, Qt::AlignCenter, QString::number(number));
}

// ============================================================
// Cell drawing
// ============================================================

void BoardView::drawCell(QPainter &painter, int col, int row, bool hovered) {
    const Cell &cell = m_game->board().cell(col, row);
    QRect r(cellToPixelX(col), cellToPixelY(row), CELL_SIZE, CELL_SIZE);

    if (cell.isRevealed()) {
        // Revealed: light grey
        if (cell.isMine() && cell.isFlagged()) {
            // Correctly flagged mine — green background
            painter.fillRect(r, QColor(100, 255, 100));
            drawMine(painter, r);
            drawFlag(painter, r);
        } else if (cell.isMine()) {
            // Unflagged mine — red background
            painter.fillRect(r, QColor(255, 100, 100));
            drawMine(painter, r);
        } else {
            painter.fillRect(r, QColor(220, 220, 220));
            if (cell.adjacentMines() > 0) {
                drawNumber(painter, r, cell.adjacentMines());
            }
        }
    } else {
        // Unrevealed: raised 3D bevel
        QColor base = QColor(160, 180, 200);
        if (hovered)
            base = base.lighter(115);
        painter.fillRect(r, base);

        // Top-left highlight
        painter.setPen(QColor(210, 225, 240));
        painter.drawLine(r.topLeft(), r.topRight());
        painter.drawLine(r.topLeft(), r.bottomLeft());

        // Bottom-right shadow
        painter.setPen(QColor(110, 130, 150));
        painter.drawLine(r.bottomLeft(), r.bottomRight());
        painter.drawLine(r.topRight(), r.bottomRight());

        if (cell.isFlagged())
            drawFlag(painter, r);
    }

    // Cell separator lines
    painter.setPen(QPen(QColor(80, 80, 80), GRID_LINE));
    painter.drawLine(r.topRight(), r.bottomRight());
    painter.drawLine(r.bottomLeft(), r.bottomRight());
}

// ============================================================
// paintEvent
// ============================================================

void BoardView::paintEvent(QPaintEvent *) {
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing, false);

    const int w = m_game->board().width();
    const int h = m_game->board().height();
    const int bw = boardPixelWidth() + 2 * GRID_LINE;
    const int bh = boardPixelHeight() + 2 * GRID_LINE;

    // Dark background
    painter.fillRect(0, 0, bw, bh, QColor(60, 60, 60));

    // Outer border
    painter.setPen(QPen(QColor(40, 40, 40), GRID_LINE));
    painter.drawRect(0, 0, bw, bh);

    // Cells
    for (int row = 0; row < h; ++row)
        for (int col = 0; col < w; ++col) {
            bool hovered = (col == m_hoverCol && row == m_hoverRow
                           && !m_game->board().cell(col, row).isRevealed());
            drawCell(painter, col, row, hovered);
        }
}

// ============================================================
// Mouse events
// ============================================================

void BoardView::mousePressEvent(QMouseEvent *event) {
    int col, row;
    pixelToCell(event->pos().x(), event->pos().y(), col, row);

    if (col < 0 || col >= m_game->board().width() ||
        row < 0 || row >= m_game->board().height())
        return;

    if (event->button() == Qt::LeftButton) {
        emit cellClicked(col, row);
    } else if (event->button() == Qt::RightButton) {
        m_game->toggleFlag(col, row);
        update();
    }
}

void BoardView::mouseMoveEvent(QMouseEvent *event) {
    int col, row;
    pixelToCell(event->pos().x(), event->pos().y(), col, row);

    const int w = m_game->board().width();
    const int h = m_game->board().height();

    if (col >= 0 && col < w && row >= 0 && row < h) {
        if (col != m_hoverCol || row != m_hoverRow) {
            m_hoverCol = col;
            m_hoverRow = row;
            update();
        }
    }
}

void BoardView::leaveEvent(QEvent *) {
    m_hoverCol = -1;
    m_hoverRow = -1;
    update();
}

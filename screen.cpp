#include "screen.h"
#include <QDebug>

Screen::Screen(QWidget *parent) : QWidget(parent)
{
    dataInfoLabel = new QLabel(this);
    viewInfoLabel = new QLabel(this);
    operation = MOVE;
    scale = 1;
    pos = QPoint(0, 0);
    board = nullptr;
    showLines = true;

    dataInfoLabel->setAlignment(Qt::AlignBottom | Qt::AlignLeft);
    dataInfoLabel->setMargin(10);
    viewInfoLabel->setAlignment(Qt::AlignTop | Qt::AlignLeft);
    viewInfoLabel->setMargin(10);
}

void Screen::setBoard(Board *b)
{
    board = b;
}

void Screen::setSize(QSize size)
{
    setFixedSize(size);
    canvas = QPixmap(size);
}

void Screen::setOperation(Screen::OPERATION op)
{
    operation = op;
}

void Screen::setScale(double f)
{
    scale *= f;
    pos.setX((pos.x() + width() / 2) * f - width() / 2);
    pos.setY((pos.y() + height() / 2) * f - height() / 2);
    update();
}

void Screen::setScale(double f, QPoint center)
{
    scale *= f;
    pos.setX((pos.x() + center.x()) * f - center.x());
    pos.setY((pos.y() + center.y()) * f - center.y());
    update();
}

void Screen::setShowLines(bool b)
{
    showLines = b;
}

void Screen::update()
{
    QPainter painter(&canvas);
    int up = floor(pos.y() / (UNIT * scale));
    int dn = ceil((pos.y() + height()) / (UNIT * scale));
    int lt = floor(pos.x() / (UNIT * scale));
    int rt = ceil((pos.x() + width()) / (UNIT * scale));

    // clear canvas
    canvas.fill();

    // fill blocks
    for (int r = up; r <= dn; ++r) {
        for (int c = lt; c <= rt; ++c) {
            if (0 <= r && r < board->Height() && 0 <= c && c < board->Width()) {
                QColor color(128, 128, 128, 128);
                switch (board->single_state(r, c)) {
                case Board::NEW_BORN:
                    color = QColor(180, 255, 200, 255);
                    break;
                case Board::NEW_DEAD:
                    color = QColor(255, 240, 240, 255);
                    break;
                case Board::STILL_ALIVE:
                    color = QColor(100, 255, 120, 255);
                    break;
                case Board::STILL_NULL:
                    color = QColor(240, 240, 255, 255);
                    break;
                }
                painter.fillRect(c * UNIT * scale - pos.x(), r * UNIT * scale - pos.y(), UNIT * scale + 1, UNIT * scale + 1, color);
            }
        }
    }

    // draw lines
    if (showLines) {
        painter.setPen(QColor(128, 128, 255, 128));
        for (int r = up; r <= dn; ++r) {
            painter.drawLine(QPoint(0, r * UNIT * scale - pos.y()), QPoint(width(), r * UNIT * scale - pos.y()));
        }
        for (int c = lt; c <= rt; ++c) {
            painter.drawLine(QPoint(c * UNIT * scale - pos.x(), 0), QPoint(c * UNIT * scale - pos.x(), height()));
        }
    }

    // update labels
    QString text;

    text.sprintf("Rounds:%d\n"
                 "Seed:%x\n"
                 "Size:%d*%d\n"
                 "Amount:%d\n"
                 "New Born:%d\n"
                 "New Dead:%d",
                 board->Rounds(), board->Seed(),
                 board->Width(), board->Height(),
                 board->cell_amount(),
                 board->increment(), board->decrement());
    dataInfoLabel->setText(text);
    dataInfoLabel->adjustSize();
    dataInfoLabel->setGeometry(0, size().height() - dataInfoLabel->height(), dataInfoLabel->width(), dataInfoLabel->height());

    text.sprintf("Scale:%.2f\n"
                 "X:%d, Y:%d",
                 scale, -pos.x(), -pos.y());
    viewInfoLabel->setText(text);
    viewInfoLabel->adjustSize();
    viewInfoLabel->setGeometry(0, 0, viewInfoLabel->width(), viewInfoLabel->height());

    // update the whole widget
    QWidget::update();
}

void Screen::paintEvent(QPaintEvent *e)
{
    QPainter painter(this);
    painter.drawPixmap(rect(), canvas);
    QWidget::paintEvent(e);
}

void Screen::mousePressEvent(QMouseEvent *e)
{
    if (e->buttons() & Qt::LeftButton) {
        pressPosition = e->pos();
        movePosition = e->pos();
    }
    QWidget::mousePressEvent(e);
}

void Screen::mouseMoveEvent(QMouseEvent *e)
{
    if (e->buttons() & Qt::LeftButton) {
        int dx = e->x() - movePosition.x();
        int dy = e->y() - movePosition.y();
        movePosition = e->pos();
        pos.setX(pos.x() - dx);
        pos.setY(pos.y() - dy);
        update();
    }
    QWidget::mouseMoveEvent(e);
}

void Screen::mouseReleaseEvent(QMouseEvent *e)
{
    if (e->button() == Qt::LeftButton) {
        if ((e->pos() - pressPosition).manhattanLength() <= PRECISION) {
            // mouse click
            int x = pos.x() + e->x();
            int y = pos.y() + e->y();
            int c = x / (UNIT * scale);
            int r = y / (UNIT * scale);
            emit cell_flipped(r, c);
        }
    }
}

void Screen::wheelEvent(QWheelEvent *e)
{
    int step = e->angleDelta().y();
    double f = 1.0;
    if (step > 0 && scale < 10) {
        f = 1.2;
    }
    if (step < 0 && scale > 0.2){
        f = 1 / 1.2;
    }
    setScale(f, e->pos());
    QWidget::wheelEvent(e);
}

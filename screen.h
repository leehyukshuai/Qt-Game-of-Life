#ifndef SCREEN_H
#define SCREEN_H

#define UNIT 30
#define PRECISION 3

#include <QWidget>
#include <QLabel>
#include <QPixmap>
#include <QPainter>
#include <QMouseEvent>
#include <QPaintEvent>
#include <QWheelEvent>
#include "board.h"

class Screen : public QWidget
{
    Q_OBJECT

public:
    enum OPERATION {MOVE, ZOOM_IN, ZOOM_OUT, FLIP};

private:
    QLabel *dataInfoLabel;
    QLabel *viewInfoLabel;
    OPERATION operation;
    Board *board;
    QPixmap canvas;
    double scale;
    QPoint pos;
    QPoint pressPosition;
    QPoint movePosition;
    bool showLines;

signals:
    void cell_flipped(int row, int column);

public:
    Screen(QWidget *parent = nullptr);
    void setBoard(Board *b);
    void setSize(QSize size);
    void setOperation(OPERATION op);
    void setScale(double f);
    void setScale(double f, QPoint center);
    void setShowLines(bool b);
    void update();

private:
    void updateInfo();

protected:
    void paintEvent(QPaintEvent *);
    void mousePressEvent(QMouseEvent *);
    void mouseMoveEvent(QMouseEvent *);
    void mouseReleaseEvent(QMouseEvent *);
    void wheelEvent(QWheelEvent *);
};

#endif // SCREEN_H

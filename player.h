#ifndef PLAYER_H
#define PLAYER_H

#define WINDOW_WIDTH 1800
#define WINDOW_HEIGHT 1200

#define ROWS 20
#define COLUMNS 20

#include <QMainWindow>
#include <QToolBar>
#include <QAction>
#include <QTimer>
#include <QInputDialog>
#include "screen.h"

class Player : public QMainWindow
{
    Q_OBJECT

private:
    Board *board;
    Screen *screen;
    int speed; // evolutions per second
    QTimer *timer;
    QAction *moveAction;
    QAction *flipAction;
    QAction *zoomInAction;
    QAction *zoomOutAction;
    QAction *playAction;
    QAction *nextAction;
    QAction *prevAction;
    QAction *speedUpAction;
    QAction *speedDownAction;
    QAction *clearAction;
    QAction *reloadAction;
    QAction *showLineAction;


public:
    Player(QWidget *parent = 0);
    void update();

private slots:
    void on_screen_cell_flipped(int r, int c);
    void on_timer_timeout();
    void on_moveAction_triggered();
    void on_flipAction_triggered();
    void on_zoomInAction_triggered();
    void on_zoomOutAction_triggered();
    void on_playAction_toggled(bool checked);
    void on_nextAction_triggered();
    void on_prevAction_triggered();
    void on_speedUpAction_triggered();
    void on_speedDownAction_triggered();
    void on_clearAction_triggered();
    void on_reloadAction_triggered();
    void on_showLineAction_triggered(bool checked);
};

#endif // PLAYER_H

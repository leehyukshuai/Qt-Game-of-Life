#include "player.h"
#include <QDebug>

Player::Player(QWidget *parent)
    : QMainWindow(parent)
{
    setFixedSize(WINDOW_WIDTH, WINDOW_HEIGHT);
    setFont(QFont("Microsoft Yahei", 10));
    setWindowIcon(QIcon(":/image/icons/tree.png"));
    setWindowTitle("Life Game");

    // board and screen
    board = new Board(ROWS, COLUMNS);
    screen = new Screen(this);
    connect(screen, SIGNAL(cell_flipped(int,int)), this, SLOT(on_screen_cell_flipped(int,int)));
    screen->setSize(size());
    screen->setBoard(board);
    screen->update();

    // user interface
    // new
    moveAction = new QAction(QIcon(":/image/icons/move.png"), QString("move"), this);
    moveAction->setCheckable(true);
    flipAction = new QAction(QIcon(":/image/icons/adjust.png"), QString("flip"), this);
    flipAction->setCheckable(true);
    zoomInAction = new QAction(QIcon(":/image/icons/zoom-in.png"), QString("zoom in"), this);
    zoomOutAction = new QAction(QIcon(":/image/icons/zoom-out.png"), QString("zoom out"), this);
    playAction = new QAction(QIcon(":/image/icons/play-circle.png"), QString("play"), this);
    playAction->setCheckable(true);
    nextAction = new QAction(QIcon(":/image/icons/chevron-right.png"), QString("next"), this);
    prevAction = new QAction(QIcon(":/image/icons/chevron-left.png"), QString("prev"), this);
    speedUpAction = new QAction(QIcon(":/image/icons/forward.png"), QString("speed up"), this);
    speedDownAction = new QAction(QIcon(":/image/icons/backward.png"), QString("speed down"), this);
    clearAction = new QAction(QIcon(":/image/icons/delete.png"), QString("clear"), this);
    reloadAction = new QAction(QIcon(":/image/icons/refresh.png"), QString("reload"), this);
    showLineAction = new QAction(QIcon(":/image/icons/grid.png"), QString("show line"), this);
    showLineAction->setCheckable(true);
    showLineAction->setChecked(true);
    // connect
    connect(moveAction, SIGNAL(triggered(bool)), this, SLOT(on_moveAction_triggered()));
    connect(flipAction, SIGNAL(triggered(bool)), this, SLOT(on_flipAction_triggered()));
    connect(zoomInAction, SIGNAL(triggered(bool)), this, SLOT(on_zoomInAction_triggered()));
    connect(zoomOutAction, SIGNAL(triggered(bool)), this, SLOT(on_zoomOutAction_triggered()));
    connect(playAction, SIGNAL(toggled(bool)), this, SLOT(on_playAction_toggled(bool)));
    connect(nextAction, SIGNAL(triggered(bool)), this, SLOT(on_nextAction_triggered()));
    connect(prevAction, SIGNAL(triggered(bool)), this, SLOT(on_prevAction_triggered()));
    connect(speedUpAction, SIGNAL(triggered(bool)), this, SLOT(on_speedUpAction_triggered()));
    connect(speedDownAction, SIGNAL(triggered(bool)), this, SLOT(on_speedDownAction_triggered()));
    connect(clearAction, SIGNAL(triggered(bool)), this, SLOT(on_clearAction_triggered()));
    connect(reloadAction, SIGNAL(triggered(bool)), this, SLOT(on_reloadAction_triggered()));
    connect(showLineAction, SIGNAL(triggered(bool)), this, SLOT(on_showLineAction_triggered(bool)));

    // toolbar
    QToolBar *toolBar = new QToolBar("&Tools");
    addToolBar(Qt::RightToolBarArea, toolBar);
    toolBar->addAction(moveAction);
    toolBar->addAction(flipAction);
    toolBar->addSeparator();
    toolBar->addAction(zoomInAction);
    toolBar->addAction(zoomOutAction);
    toolBar->addSeparator();
    toolBar->addAction(playAction);
    toolBar->addAction(speedUpAction);
    toolBar->addAction(speedDownAction);
    toolBar->addSeparator();
    toolBar->addAction(nextAction);
    toolBar->addAction(prevAction);
    toolBar->addSeparator();
    toolBar->addAction(clearAction);
    toolBar->addAction(reloadAction);
    toolBar->addSeparator();
    toolBar->addAction(showLineAction);

    // timer and notifier
    speed = 1;
    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(on_timer_timeout()));
}

void Player::update()
{
    screen->update();
    QMainWindow::update();
}

void Player::on_screen_cell_flipped(int r, int c)
{
    if (playAction->isChecked())
        playAction->setChecked(false);
    board->flip(r, c);
    update();
}

void Player::on_timer_timeout()
{
    board->evolve();
    update();
}

void Player::on_moveAction_triggered()
{
    screen->setOperation(Screen::MOVE);
    flipAction->setChecked(false);
}

void Player::on_flipAction_triggered()
{
    screen->setOperation(Screen::FLIP);
    moveAction->setChecked(false);
}

void Player::on_zoomInAction_triggered()
{
    screen->setScale(1.2);
}

void Player::on_zoomOutAction_triggered()
{
    screen->setScale(1 / 1.2);
}

void Player::on_playAction_toggled(bool checked)
{
    if (checked) {
        timer->start(1000 / speed);
    } else {
        timer->stop();
    }
}

void Player::on_nextAction_triggered()
{
    board->evolve();
    update();
}

void Player::on_prevAction_triggered()
{
    board->decline();
    update();
}

void Player::on_speedUpAction_triggered()
{
    if (speed < 8)
        speed += 1;
    if (playAction->isChecked()) {
        timer->start(1000 / speed);
    }
}

void Player::on_speedDownAction_triggered()
{
    if (speed > 1)
        speed -= 1;
    if (playAction->isChecked()) {
        timer->start(1000 / speed);
    }
}

void Player::on_clearAction_triggered()
{
    if (playAction->isChecked())
        playAction->setChecked(false);
    board->empty();
    update();
}

void Player::on_reloadAction_triggered()
{
    if (playAction->isChecked())
        playAction->setChecked(false);
    unsigned seed = QInputDialog::getInt(this, "Seed", "Input the seed for generation.", 0, 0);
    board->randomize(seed);
    update();
}

void Player::on_showLineAction_triggered(bool checked)
{
    screen->setShowLines(checked);
    update();
}


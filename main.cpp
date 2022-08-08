#include <QApplication>
#include "player.h"
#include "board.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    Player player;
    player.show();

    return a.exec();
}

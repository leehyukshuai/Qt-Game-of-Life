#ifndef BOARD_H
#define BOARD_H

#define MAX_WIDTH 200
#define MAX_HEIGHT 200
#define MAX_HISTORY 10

#include <bitset>

class Board
{
private:
    int width, height;
    std::bitset<MAX_WIDTH> data[MAX_HISTORY][MAX_HEIGHT];
    int cur_index, olddest_index, newest_index;
    int new_borns, new_deads;
    unsigned seed;
    int rounds;

public:
    enum RETURN_VALUE {RET_ERROR = -1, RET_OK};
    enum CELL_STATE {NEW_BORN, NEW_DEAD, STILL_NULL, STILL_ALIVE};

    Board(int w, int h);

    // basic funcs
    int Rounds();
    int Width();
    int Height();
    unsigned Seed();
    int initialize();
    CELL_STATE single_evolve(int row, int column);
    CELL_STATE single_state(int row, int column);

    // index operation
    int set_forward(int &index);
    int set_backward(int &index);
    int next_index(int index);
    int prev_index(int index);
    int is_legal(int index);

    // evolution operation
    int evolve();
    int decline();

    // content operation
    int flip(int row, int column);
    int empty();
    int randomize(unsigned _seed);

    // calculation operation
    int cell_amount();
    int cell_amount(int index);
    int increment();
    int decrement();
};

#endif // BOARD_H

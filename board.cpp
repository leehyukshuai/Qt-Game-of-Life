#include "board.h"
#include <qglobal.h>

Board::Board(int w, int h)
    : width(w), height(h)
{
    initialize();
}

int Board::Rounds()
{
    return rounds;
}

int Board::Width()
{
    return width;
}

int Board::Height()
{
    return height;
}

unsigned Board::Seed()
{
    return seed;
}

int Board::initialize()
{
    newest_index = olddest_index = cur_index = 0;
    new_borns = new_deads = 0;
    rounds = 0;
    return RET_OK;
}

int Board::set_forward(int &index)
{
    index = (index + 1) % MAX_HISTORY;
    return RET_OK;
}

int Board::set_backward(int &index)
{
    index = (index - 1 + MAX_HISTORY) % MAX_HISTORY;
    return RET_OK;
}

int Board::next_index(int index)
{
    return (index + 1) % MAX_HISTORY;
}

int Board::prev_index(int index)
{
    return (index - 1 + MAX_HISTORY) % MAX_HISTORY;
}

int Board::is_legal(int index)
{
    int ret = 0;
    if (0 <= index && index < MAX_HISTORY) {
        int d0 = newest_index - olddest_index;
        int d1 = index - olddest_index;
        if (d0 < 0) {
            ret = (d1 >= 0 || d1 <= d0);
        } else {
            ret = (d1 >= 0 && d1 <= d0);
        }
    }
    return ret;
}

int Board::evolve()
{
    rounds += 1;
    new_borns = 0;
    new_deads = 0;
    if (cur_index == newest_index) {
        set_forward(newest_index);
        if (olddest_index == newest_index) {
            set_forward(olddest_index);
        }
        for (int r = 0; r < height; ++r) {
            for (int c = 0; c < width; ++c) {
                CELL_STATE state = single_evolve(r, c);
                new_borns += (state == NEW_BORN);
                new_deads += (state == NEW_DEAD);
                data[newest_index][r][c] = ((state == STILL_ALIVE) || (state == NEW_BORN));
            }
        }
    } else {
        for (int r = 0; r < height; ++r) {
            for (int c = 0; c < width; ++c) {
                CELL_STATE state = single_evolve(r, c);
                new_borns += (state == NEW_BORN);
                new_deads += (state == NEW_DEAD);
            }
        }
    }
    set_forward(cur_index);
    return RET_OK;
}

inline Board::CELL_STATE Board::single_evolve(int row, int column)
{
    CELL_STATE ret;
    int dirs[8][2] {{-1, -1},{-1, 0},{-1, 1},{0, -1},{0, 1},{1, -1},{1, 0},{1, 1}};
    int count = 0;
    for (int i = 0; i < 8; ++i) {
        int r = row + dirs[i][0];
        int c = column + dirs[i][1];
        if (0 <= r && r < height && 0 <= c && c < width && data[cur_index][r][c]) {
            count += 1;
        }
    }
    if (data[cur_index][row][column]) {
        if (count < 2 || 3 < count) {
            ret = NEW_DEAD;
        } else {
            ret = STILL_ALIVE;
        }
    } else {
        if (count == 3) {
            ret = NEW_BORN;
        } else {
            ret = STILL_NULL;
        }
    }
    return ret;
}

Board::CELL_STATE Board::single_state(int row, int column)
{
    if (cur_index != olddest_index) {
        int pre = data[prev_index(cur_index)][row][column];
        int cur = data[cur_index][row][column];
        if (pre == 0) {
            if (cur == 1) {
                return NEW_BORN;
            } else {
                return STILL_NULL;
            }
        } else {
            if (cur == 1) {
                return STILL_ALIVE;
            } else {
                return NEW_DEAD;
            }
        }
    } else {
        int cur = data[cur_index][row][column];
        if (cur == 1) {
            return NEW_BORN;
        } else {
            return STILL_NULL;
        }
    }
}

int Board::decline()
{
    if (cur_index != olddest_index && prev_index(cur_index) != olddest_index) {
        new_borns = 0;
        new_deads = 0;
        set_backward(cur_index);
        for (int r = 0; r < height; ++r) {
            for (int c = 0; c < width; ++c) {
                CELL_STATE state = single_state(r, c);
                new_borns += (state == NEW_BORN);
                new_deads += (state == NEW_DEAD);
            }
        }
        rounds -= 1;
        return RET_OK;
    } else {
        return RET_ERROR;
    }
}

int Board::flip(int row, int column)
{
    data[cur_index][row][column] = !data[cur_index][row][column];
    return RET_OK;
}

int Board::empty()
{
    initialize();
    for (int r = 0; r < height; ++r) {
        data[cur_index][r].reset();
    }
    return RET_OK;
}

int Board::randomize(unsigned _seed)
{
    seed = _seed;
    initialize();
    qsrand(seed);
    for (int r = 0; r < height; ++r) {
        for (int c = 0; c < width; ++c) {
            data[cur_index][r][c] = qrand() % 2;
        }
    }
    return RET_OK;
}

int Board::cell_amount()
{
    return cell_amount(cur_index);
}

int Board::cell_amount(int index)
{
    if (is_legal(index)) {
        int cnt = 0;
        for (int r = 0; r < height; ++r) {
            for (int c = 0; c < width; ++c) {
                cnt += data[index][r][c];
            }
        }
        return cnt;
    } else {
        return RET_ERROR;
    }
}

int Board::increment()
{
    return new_borns;
}

int Board::decrement()
{
    return new_deads;
}

#ifndef BLOCK_HPP_INCLUDED
#define BLOCK_HPP_INCLUDED

#include <windows.h>
#include <ctime>

class Block
{
private :
    enum class Direction {left, right};
    Direction direction;
    static const int max_height = 12;
    static const int max_width = 9;
    CHAR_INFO display[max_height * max_width];
    int level;
    int nBlocks;
    time_t time;
    time_t holder;
    int pos;
public :
    Block();
    void run();
    void print();
};

#endif // BLOCK_HPP_INCLUDED

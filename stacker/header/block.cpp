#include "block.hpp"
#include <conio.h>
#include <iostream>
#include <limits>

void ClearScreen()
  {
  HANDLE                     hStdOut;
  CONSOLE_SCREEN_BUFFER_INFO csbi;
  DWORD                      count;
  DWORD                      cellCount;
  COORD                      homeCoords = { 0, 0 };

  hStdOut = GetStdHandle( STD_OUTPUT_HANDLE );
  if (hStdOut == INVALID_HANDLE_VALUE) return;

  /* Get the number of cells in the current buffer */
  if (!GetConsoleScreenBufferInfo( hStdOut, &csbi )) return;
  cellCount = csbi.dwSize.X *csbi.dwSize.Y;

  /* Fill the entire buffer with spaces */
  if (!FillConsoleOutputCharacter(
    hStdOut,
    (TCHAR) ' ',
    cellCount,
    homeCoords,
    &count
    )) return;

  /* Fill the entire buffer with the current colors and attributes */
  if (!FillConsoleOutputAttribute(
    hStdOut,
    csbi.wAttributes,
    cellCount,
    homeCoords,
    &count
    )) return;

  /* Move the cursor home */
  SetConsoleCursorPosition( hStdOut, homeCoords );
  }

Block::Block() :
    level(1),
    nBlocks(5),
    time(clock())
{
    for (int i = 0; i < this->max_width; ++i)
    {
        for (int j = 0; j < this->max_height; ++j)
        {
            this->display[i + (this->max_width * j)].Char.AsciiChar = 'X';
            this->display[i + (this->max_width * j)].Attributes = FOREGROUND_INTENSITY;
        }
    }
    for (int i = 1; i < this->max_width - 1; ++i)
    {
        for (int j = 1; j < this->max_height - 1; ++j)
        {
            this->display[i + (this->max_width * j)].Char.AsciiChar = ' ';
            this->display[i + (this->max_width * j)].Attributes = FOREGROUND_RED;
        }
    }
    for (int i = 0; i < 5; ++i)
    {
        pos = 2;
        this->display[(pos + i) + (this->max_height * (this->max_height - level))].Char.AsciiChar = '@';
    }
    holder = time + 100;
    direction = Direction::right;
}

void Block::print()
{
    COORD CharBuffer = {this->max_width, this->max_height};
    COORD StartFrom = {0, 0};
    SMALL_RECT WriteArea = {0, 0, this->max_width, this->max_height};
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    WriteConsoleOutputA(hOut, this->display, CharBuffer, StartFrom, &WriteArea);
}

void Block::run()
{
    this->time = clock();
    if (this->time >= this->holder)
    {
        this->time = clock();
        this->holder = this->time + 100;
        if (this->direction == Direction::right)
        {
            for (int i = this->nBlocks - 1; i >= 0; ++i)
            {
                if (this->display[(pos + i + 1) * (this->max_height * (this->max_height - level))].Char.AsciiChar == 'X')
                {
                    this->direction = Direction::left;
                    break;
                }
                this->display[(pos + i + 1) + (this->max_height * (this->max_height - level))].Char.AsciiChar = '@';
                this->display[(pos + i) + (this->max_height * (this->max_height - level))].Char.AsciiChar = ' ';
            }
        }
        else if (this->direction == Direction::left)
        {
            for (int i = 0; i < nBlocks; ++i)
            {
                if (this->display[(pos - 1) + (this->max_height * (this->max_height - level))].Char.AsciiChar = 'X')
                {
                    this->direction = Direction::right;
                    break;
                }
                this->display[((pos + i) - 1) + (this->max_height * (this->max_height - level))].Char.AsciiChar = '@';
                this->display[(pos + i) + (this->max_height * (this->max_height - level))].Char.AsciiChar =' ';
            }
        }
    }
    if (kbhit())
    {
        char c = getch();
        if (c == 32) //32 is number for space button
        {
            ++level;
            for (int i = 0; i < this->nBlocks; ++i)
            {
                if (this->display[(pos + i) + (this->max_height * (this->max_height - (level - 1)))].Char.AsciiChar == ' ')
                {
                    this->display[(pos + i) + (this->max_height * (this->max_height - level))].Char.AsciiChar = ' ';
                    this->nBlocks--;
                }
            }
            this->pos = this->max_width / 2;
            for (int i = 0 ; i < this->nBlocks; ++i)
            {
                this->display[(pos + i) + (this->max_height * (this->max_height - level))].Char.AsciiChar = '@';
            }
            if ( (nBlocks > 0) && (level == 10) )//10 is the max level
            {
                ClearScreen();
                std::cout << "you win\n";
                std::cin.ignore( std::numeric_limits<std::streamsize>::max(), '\n' );
                exit(1);
            }
        }
    }
}

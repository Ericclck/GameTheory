#include "board.h"

// Initialize cells as all empty, score with the given parameter, curPlayer as X and id as 0.
Board::Board(const int score[][BOARD_SIZE])
    :curPlayer(X), id(0)
{
    for (int i = 0; i < BOARD_SIZE; ++i)
    {
        for (int j = 0; j < BOARD_SIZE; ++j)
        {
            cells[i][j] = EMPTY;
            this->score[i][j] = score[i][j];
        }
    }
}

// Return true if all cells are not EMPTY.
bool Board::isFull() const
{
    for (int i = 0; i < BOARD_SIZE; ++i)
        for (int j = 0; j < BOARD_SIZE; ++j)
            if (cells[i][j] == EMPTY)
                return false;
    return true;
}

// Return true if the game has finished (a match is found or board is full).
bool Board::isFinished() const
{
    // if(isFull()) 
    //    return true;
    Cell lastPlayer = (curPlayer == X) ? O : X;
    //Check rows
    for (int row = 0; row < BOARD_SIZE; ++row)
    {
        if (cells[row][0] != lastPlayer) 
            continue;
        bool won = true;
        for (int col = 1; col < BOARD_SIZE; ++col)
        {
            if (cells[row][col] != lastPlayer)
                won = false;
        }
        if (won) 
            return won;
    }

    //Check cols
    for (int col = 0; col < BOARD_SIZE; ++col)
    {
        if (cells[0][col] != lastPlayer)
            continue;
        bool won = true;
        for (int row = 1; row < BOARD_SIZE; ++row)
        {
            if (cells[row][col] != lastPlayer)
                won = false;
        }
        if (won)
            return won;
    }

    //Check diag
    if (cells[0][0] != lastPlayer && cells[0][BOARD_SIZE-1] != lastPlayer) 
        return false;
    bool won = true;
    for (int i = 0; i < BOARD_SIZE; ++i)
        if (cells[i][i] != lastPlayer) 
            won = false;
    if (won) 
        return won;
    won = true;
    for (int i = 0; i < BOARD_SIZE; ++i)
        if (cells[i][BOARD_SIZE-1-i] != lastPlayer)
            won = false;
    if (won)
        return won;

    return false;
}

// Calculate the board's current score.
int Board::getBoardScore() const
{
    if (isFinished())
        return curPlayer*-WIN_SCORE;

    int currentScore = 0;
    for (int i = 0; i < BOARD_SIZE; ++i)
        for (int j = 0; j < BOARD_SIZE; ++j)
            currentScore += cells[i][j] * score[i][j] * getCellWeight(cells[i][j]);
    return currentScore;

}

// Play the next move at coords. If successful, update the data members and return true. Otherwise return false.
bool Board::play(const BoardCoordinate& coords)
{
    int row = coords.row;
    int col = coords.col;
    
    if (!coords.isValid())
        return false;
    if (cells[row][col] != EMPTY)
        return false;
    
    int cellValue;
    switch (curPlayer)
    {
        case X:
            cellValue = 1;
            break;
        case O:
            cellValue = 2;
            break;
    }
    cells[row][col] = curPlayer;
    id += cellValue * pow(3, (row * BOARD_SIZE + col));
    curPlayer = (curPlayer == X) ? O : X;
    return true;
}
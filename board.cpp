#include "board.h"
#include <iostream>

// The constructor initializes the cells board as all empty, the score grid using the provided parameter, the current player curPlayer as X since X starts first, and the id as 0.
Board::Board(const int score[][BOARD_SIZE]){
    for(int i = 0; i < BOARD_SIZE; i++){
        for(int j = 0; j < BOARD_SIZE; j++){
            cells[i][j] = EMPTY;
            this->score[i][j] = score[i][j];
        }
    }
    curPlayer = X;
    id = 0;
}

// Returns true if all cells of the board are non-empty, and false otherwise.
bool Board::isFull() const{
    for(int i = 0; i < BOARD_SIZE; i++){
        for(int j = 0; j < BOARD_SIZE; j++){
            if(cells[i][j] == EMPTY){
                return false;
            }
        }
    }
    return true;
}

// Returns true if all cells of the board are non-empty or a player has gotten a match.
bool Board::isFinished() const{
    if (isFull()) return true;
    Cell lastPlayer = (curPlayer == X) ? O : X;
    for (int i = 0; i < BOARD_SIZE; i++){
        bool rowMatch = true;
        if (cells[i][0] != lastPlayer) continue;
        for(int j = 0; j < BOARD_SIZE; j++){
            if(cells[i][j] != cells[i][0]){
                rowMatch = false;
                break;
            }
        }
        if(rowMatch) return true;
    }
    for (int i = 0; i < BOARD_SIZE; i++){
        bool colMatch = true;
        if (cells[0][i] != lastPlayer) continue;
        for(int j = 0; j < BOARD_SIZE; j++){
            if(cells[j][i] != lastPlayer){
                colMatch = false;
                break;
            }
        }
        if(colMatch) return true;
    }
    
    bool diagMatch = true;
    for(int i = 0; i < BOARD_SIZE; i++){
        if(cells[i][i] != lastPlayer){
            diagMatch = false;
            break;
        }
    }
    if(diagMatch) return true;
    bool antiDiagMatch = true;
    for(int i = 0; i < BOARD_SIZE; i++){
        if(cells[i][BOARD_SIZE - i - 1] != lastPlayer){
            antiDiagMatch = false;
            break;
        }
    }
    if(antiDiagMatch) return true;
    return false;
}


int Board::getBoardScore() const{
    //Returns the board's score. You need to first check if any player has gotten a match: a row, column or diagonal with all cells of that player. If yes, return WIN_SCORE if player X has a match, or -WIN_SCORE if player O has a match.
    Cell lastPlayer = (curPlayer == X) ? O : X;
    for (int i = 0; i < BOARD_SIZE; i++){
        bool rowMatch = true;
        if (cells[i][0] != lastPlayer) continue;
        for(int j = 0; j < BOARD_SIZE; j++){
            if(cells[i][j] != cells[i][0]){
                rowMatch = false;
                break;
            }
        }
        if(rowMatch){
            if(cells[i][0] == X) return WIN_SCORE;
            else return -WIN_SCORE;
        }
    }
    for (int i = 0; i < BOARD_SIZE; i++){
        bool colMatch = true;
        if (cells[0][i] != lastPlayer) continue;
        for(int j = 0; j < BOARD_SIZE; j++){
            if(cells[j][i] != cells[0][i]){
                colMatch = false;
                break;
            }
        }
        if(colMatch){
            if(cells[0][i] == X) return WIN_SCORE;
            else return -WIN_SCORE;
        }
    }
    bool diagMatch = true;
    for(int i = 0; i < BOARD_SIZE; i++){
        if(cells[i][i] != lastPlayer){
            diagMatch = false;
            break;
        }
    }
    if(diagMatch){
        if(cells[0][0] == X) return WIN_SCORE;
        else return -WIN_SCORE;
    }
    bool antiDiagMatch = true;
    for(int i = 0; i < BOARD_SIZE; i++){
        if(cells[i][BOARD_SIZE - i - 1] != lastPlayer){
            antiDiagMatch = false;
            break;
        }
    }
    if(antiDiagMatch){
        if(cells[0][BOARD_SIZE - 1] == X) return WIN_SCORE;
        else return -WIN_SCORE;
    }
    //Otherwise, calculate the score based on the current cells on the board.
    int score = 0;
    for(int i = 0; i < BOARD_SIZE; i++){
        for(int j = 0; j < BOARD_SIZE; j++){
            if(cells[i][j] == X) score += this->score[i][j] * getCellWeight(cells[i][j]);
            else if(cells[i][j] == O) score -= this->score[i][j] * getCellWeight(cells[i][j]);
        }
    }
    return score;
}

// If the move is not valid (out of bounds or the cell is filled), return false. Otherwise, update the corresponding cells array element, change curPlayer to the other player and update id according to the algorithm described above.
bool Board::play(const BoardCoordinate& coords){
    if(coords.row < 0 || coords.row >= BOARD_SIZE || coords.col < 0 || coords.col >= BOARD_SIZE || cells[coords.row][coords.col] != EMPTY) return false;
    cells[coords.row][coords.col] = curPlayer;
    if(curPlayer == X)curPlayer = O;
    else curPlayer = X;
        
    if (id == 0){
        for(int i = 0; i < BOARD_SIZE; i++){
            for(int j = 0; j < BOARD_SIZE; j++){
                int cellValue = 0;
                if(cells[i][j] == X) cellValue = 1;
                else if(cells[i][j] == O) cellValue = 2;
                id += cellValue * pow(3,(i * BOARD_SIZE + j));
            }
        }
    }
    else{
        int cellValue = 0;
        if(cells[coords.row][coords.col] == X) cellValue = 1;
        else if(cells[coords.row][coords.col] == O) cellValue = 2;
        id += cellValue * pow(3,(coords.row * BOARD_SIZE + coords.col));
    }

    return true;
}
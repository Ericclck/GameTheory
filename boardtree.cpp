#include <iostream>
#include "board.h"
#include "boardtree.h"
#include "hashtable.h"

//The constructor initializes the root BoardNode with the provided board. This constructor is used to create the actual "root" of the search tree, with the children nodes constructed via default constructor and will be properly initialized as the tree is explored.
BoardTree::BoardTree(const Board& board){
    root = new BoardNode(board);
}

//Free any dynamic memory accordingly. Hint: BoardTree is similar to BT/BST introduced in the course materials.
BoardTree::~BoardTree(){
    delete root;
}

// Returns a pointer to the sub-tree indicated by the given coordinates. This represents the child node corresponding to the move with the given coordinates. If the sub-tree is empty, you should try to build the sub-tree first (hint: if play() returns true, the new board is valid and can be build) by initializing its root data member accordingly.
BoardTree* BoardTree::getSubTree(const BoardCoordinate &coords){
    if (root == nullptr){
        return nullptr;
    }
    if(root->subTree[coords.row][coords.col].isEmpty()){
        Board newBoard = root->board;
        if(newBoard.play(coords)){
            root->subTree[coords.row][coords.col].root = new BoardNode(newBoard);
        }
    }
    return &(root->subTree[coords.row][coords.col]);
}



BoardOptimalMove BoardTree::getOptimalMove(const unsigned int depth){
    if (isEmpty()) {
        return BoardOptimalMove(); // Returns a dummy illegal move
    }
    

    if (depth == 0 || root->board.isFinished()) {
        // If depth is 0 or if the game has already finished, we cannot search further
        // return the score of this board with any move coordinate since we will not use it
        return BoardOptimalMove(root->board.getBoardScore(), BoardCoordinate(0, 0));
    }

    
    // ADDED: check if the board is already calculated to this depth
    /*if (board and depth is stored in table) {
    return the optimal move stored in table;
    }*/
    if (BoardHashTable::getInstance().getHashedMove( root->board.getID() , depth).score != ILLEGAL) {
        return BoardHashTable::getInstance().getHashedMove( root->board.getID() , depth);
    }
    
    // Else, we find the estimated score and optimal move of this node by calculating the score of each children node
    // Player X is trying to maximize the score, so the estimated score is the maximum of children scores
    // Vice versa, player O is trying to minimize the score
    int estimatedScore = root->board.getCurPlayer() == X ? -WIN_SCORE-1 : WIN_SCORE+1;
    BoardOptimalMove bestMove;
    for (int i = 0; i < BOARD_SIZE; i++) {
        for (int j = 0; j < BOARD_SIZE; j++){
            //build the subtree if it is empty;
            BoardTree* subtree = getSubTree(BoardCoordinate(i, j));
            BoardOptimalMove childMove = subtree->getOptimalMove(depth - 1);


            if (childMove.score == ILLEGAL) {
                // If the move is illegal, the subtree corresponds to an invalid move/board, simply skip to the next subtree
                continue;
            }

            bool isBetter = root->board.getCurPlayer() == X ? childMove.score > estimatedScore : childMove.score < estimatedScore;
            if (isBetter) {
                estimatedScore = childMove.score;
                bestMove = BoardOptimalMove(estimatedScore, BoardCoordinate(i, j));
            }
        }
    }


    // updateTable(board id, depth, bestMove);
    BoardHashTable::getInstance().updateTable(root->board.getID(), depth, bestMove);
    
    return bestMove;
}

BoardOptimalMove BoardTree::getOptimalMoveAlphaBeta(const unsigned int depth, int alpha, int beta){
    if(isEmpty()){
        return BoardOptimalMove();
    }
    if(depth == 0 || root->board.isFinished()){
        return BoardOptimalMove(root->board.getBoardScore(), BoardCoordinate(0, 0));
    }
    int estimatedScore = root->board.getCurPlayer() == X ? -WIN_SCORE-1 : WIN_SCORE+1;
    BoardOptimalMove bestMove;
    for(int i = 0; i < BOARD_SIZE; i++){
        for(int j = 0; j < BOARD_SIZE; j++){
            BoardTree* subtree = getSubTree(BoardCoordinate(i, j));
            BoardOptimalMove childMove = subtree->getOptimalMoveAlphaBeta(depth - 1, alpha, beta);
            if(childMove.score == ILLEGAL){
                continue;
            }
            if(root->board.getCurPlayer() == X){
                if(childMove.score > estimatedScore){
                    estimatedScore = childMove.score;
                    bestMove = BoardOptimalMove(estimatedScore, BoardCoordinate(i, j));
                }
                if(estimatedScore > beta){
                    return bestMove;
                }
                alpha = fmax(estimatedScore, alpha);
            }else{
                if(childMove.score < estimatedScore){
                    estimatedScore = childMove.score;
                    bestMove = BoardOptimalMove(estimatedScore, BoardCoordinate(i, j));
                }
                if(estimatedScore < alpha){
                    return bestMove;
                }
                beta = fmin(estimatedScore, beta);
            }
        }
    }
    return bestMove;
}
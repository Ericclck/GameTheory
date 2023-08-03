#include "boardtree.h"

BoardTree::BoardTree(const Board& board)
    : root(new BoardNode(board)) {}

BoardTree::~BoardTree()
{
    delete root;
}

BoardTree* BoardTree::getSubTree(const BoardCoordinate& coords)
{
    if (root == nullptr)
        return nullptr;

    int row = coords.row;
    int col = coords.col;
    if (root->subTree[row][col].root == nullptr)
    {
        Board copy = root->board;
        if (copy.play(coords))
            root->subTree[row][col].root = new BoardNode(copy);
    }
    return &(root->subTree[row][col]);
}   

BoardOptimalMove BoardTree::getOptimalMove(const unsigned int depth)
{
    if (root == nullptr)
        return BoardOptimalMove();
    if (depth == 0 || root->board.isFinished() || root->board.isFull())
        return BoardOptimalMove(root->board.getBoardScore(), BoardCoordinate(0,0));
    
    if (BoardHashTable::getInstance().getHashedMove(root->board.getID(), depth).score != ILLEGAL)
    {
        return BoardHashTable::getInstance().getHashedMove(root->board.getID(), depth);
    }

    int estimatedScore = (root->board.getCurPlayer() == X) ? -WIN_SCORE-1 : WIN_SCORE+1;
    BoardOptimalMove bestMove;

    for (int row = 0; row < BOARD_SIZE; ++row)
    {
        for (int col = 0; col < BOARD_SIZE; ++col)
        {
            BoardCoordinate coords = BoardCoordinate(row, col);

            BoardTree *currentSubTree = getSubTree(coords);

            BoardOptimalMove childMove = currentSubTree->getOptimalMove(depth - 1);

            if (childMove.score == ILLEGAL)
                continue;
            if (root->board.getCurPlayer() == X)
            {
                if (childMove.score > estimatedScore)
                {
                    estimatedScore = childMove.score;
                    bestMove = BoardOptimalMove(estimatedScore, coords);
                }
            }
            else if (root->board.getCurPlayer() == O)
            {
                if (childMove.score < estimatedScore)
                {
                    estimatedScore = childMove.score;
                    bestMove = BoardOptimalMove(estimatedScore, coords);
                }
            }
        }
    }
    BoardHashTable::getInstance().updateTable(root->board.getID(), depth, bestMove);
    return bestMove; 
}

BoardOptimalMove BoardTree::getOptimalMoveAlphaBeta(const unsigned int depth, int alpha, int beta)
{
    if (root == nullptr)
        return BoardOptimalMove();
    if (depth == 0 || root->board.isFinished() || root->board.isFull())
        return BoardOptimalMove(root->board.getBoardScore(), BoardCoordinate(0,0));

    int estimatedScore = (root->board.getCurPlayer() == X) ? -WIN_SCORE-1 : WIN_SCORE+1;
    BoardOptimalMove bestMove;

    for (int row = 0; row < BOARD_SIZE; ++row)
    {
        for (int col = 0; col < BOARD_SIZE; ++col)
        {
            BoardCoordinate coords = BoardCoordinate(row, col);

            BoardTree *currentSubTree = getSubTree(coords);

            BoardOptimalMove childMove = currentSubTree->getOptimalMoveAlphaBeta(depth - 1, alpha, beta);

            if (childMove.score == ILLEGAL)
                continue;
            if (root->board.getCurPlayer() == X)
            {
                if (childMove.score > estimatedScore)
                {
                    estimatedScore = childMove.score;
                    bestMove = BoardOptimalMove(estimatedScore, coords);
                }
                
                if (estimatedScore > beta)
                {
                    return bestMove;
                }
                alpha = fmax(estimatedScore, alpha);
            }
            else
            {
                if (childMove.score < estimatedScore)
                {
                    estimatedScore = childMove.score;
                    bestMove = BoardOptimalMove(estimatedScore, coords);
                }
                if (estimatedScore < alpha)
                {
                    return bestMove;
                }
                beta = fmin(estimatedScore, beta);
            }
        }
    }
    
    return bestMove; 
}
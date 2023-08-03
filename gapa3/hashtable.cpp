#include "hashtable.h"

BoardOptimalMove BoardHashTable::getHashedMove(const unsigned long long id, const int depth)
{
    int index = id % TABLE_SIZE;
    if (table[index] != nullptr)
    {
        for (BoardHashNode *current = table[index]; current != nullptr; current = current->next)
            if (current->id == id && current->depth >= depth)
                return current->optimalMove;
    }
    return BoardOptimalMove(ILLEGAL, BoardCoordinate(0,0));
}

void BoardHashTable::updateTable(const unsigned long long id, const int depth, const BoardOptimalMove& optimalMove)
{
    int index = id % TABLE_SIZE;
    BoardOptimalMove exist = getHashedMove(id, depth);
    if (exist.score == ILLEGAL)
    {
        BoardHashNode* current = table[index];
        if (current == nullptr)
        {
            table[index] = new BoardHashNode(id, depth, optimalMove);
            return;
        }

        BoardHashNode* previous = nullptr;
        for (; current != nullptr; current = current->next)
        {
            if (current->id == id)
            {
                current->depth = depth;
                current->optimalMove = optimalMove;
                return;
            }
            previous = current;
        }
        previous->next = new BoardHashNode(id, depth, optimalMove);
    }
}

void BoardHashTable::clearTable()
{
    for (int i = 0; i < TABLE_SIZE; ++i)
    {
        BoardHashNode* current = table[i];
        while (current != nullptr)
        {
            if (current->next == nullptr && current == table[i])
            {
                delete table[i];
                table[i] = nullptr;
                break;
            }
            BoardHashNode* next = current->next;
            delete current;
            current = next;
        }
    }
}
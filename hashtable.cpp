#include "hashtable.h"
#include "const.h"
#include "board.h"
#include "boardtree.h"
#include <iostream>


//Returns the BoardOptimalMove stored in the hash table for the given board ID and search depth. If the board ID does not exist in the table, or the ID is stored but the search depth is lower than the parameter, then return an illegal move indicating that the table does not contain the queried board ID or the stored calculation is not sufficient (if the search depth parameter is equal to or lower than the stored search depth, return the stored move).
BoardOptimalMove BoardHashTable::getHashedMove(const unsigned long long id, const int depth){
    int index = id % TABLE_SIZE;
    BoardHashNode* node = table[index];
    if(node == nullptr){
        return BoardOptimalMove();
    }
    else{
        while(node->next != nullptr){
            if(node->id == id){
                if(node->depth < depth){
                    return BoardOptimalMove();
                }
                else{
                    return node->optimalMove;
                }
            }
            node = node->next;
        }
        if(node->id == id){
            if(node->depth < depth){
                return BoardOptimalMove();
            }
            else{
                return node->optimalMove;
            }
        }
        return BoardOptimalMove();
    }
}

//Updates the table with the optimal move for the given board ID and search depth. If the board ID does not exist in the table, create a new node at the corresponding linked list (the node can be inserted anywhere in the list). Otherwise, if the stored depth of the board ID is lower than the depth parameter, update the node's depth and optimalMove.
void BoardHashTable::updateTable(const unsigned long long id, const int depth, const BoardOptimalMove &optimalMove){
    int index = id % TABLE_SIZE;
    BoardHashNode* node = table[index];
    if(node == nullptr){
        table[index] = new BoardHashNode(id, depth, optimalMove);
    }
    else{
        while(node->next != nullptr){
            if(node->id == id){
                if(node->depth < depth){
                    node->depth = depth;
                    node->optimalMove = optimalMove;
                }
                return;
            }
            node = node->next;
        }
        if(node->id == id){
            if(node->depth < depth){
                node->depth = depth;
                node->optimalMove = optimalMove;
            }
            return;
        }
        node->next = new BoardHashNode(id, depth, optimalMove);
    }
}

void BoardHashTable::clearTable(){
    for(int i = 0; i < TABLE_SIZE; i++){
        BoardHashNode* node = table[i];
        while(node != nullptr){
            BoardHashNode* temp = node;
            node = node->next;
            delete temp;
        }
        table[i] = nullptr;
    }
}


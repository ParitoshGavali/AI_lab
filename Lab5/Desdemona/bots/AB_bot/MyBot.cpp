/*
* @file botTemplate.cpp
* @author Arun Tejasvi Chaganty <arunchaganty@gmail.com>
* @date 2010-02-04
* Template for users to create their own bots
*/

#include "Othello.h"
#include "OthelloBoard.h"
#include "OthelloPlayer.h"
#include <cstdlib>
#include <vector>
#include <queue>
#include <stack>

#include <iostream>

#define MIN -1'000'000'000
#define MAX 1'000'000'000

using namespace std;
using namespace Desdemona;

const int lookahead = 4;

struct MinMaxNode
{
    int heuristic;
    OthelloBoard board;
    //vector<MinMaxNode*> children;
    MinMaxNode* optimal_child;
    Move transition;
    int depth;
    //int alpha = MIN;
    //int beta = MAX;
    //Turn turn;
    MinMaxNode(const OthelloBoard& other):transition(0,0), board(other) {}

    friend ostream& operator<< (ostream& out, const MinMaxNode& n);
};

ostream& operator<< (ostream& out, const MinMaxNode& n)
{
    cout << "Board: \n";
    n.board.print();
    cout << "Depth: " << n.depth << endl;
    cout << "\n\n\n";
    return out;
}

//Black is maximizer and red is minimizer
int heuristic(OthelloBoard& board)
{
    //cout << "Entered heuristic\n";
    return board.getBlackCount() - board.getRedCount();
}

class MyBot: public OthelloPlayer
{
    public:
        /**
         * Initialisation routines here
         * This could do anything from open up a cache of "best moves" to
         * spawning a background processing thread. 
         */
        MyBot( Turn turn );

        /**
         * Play something 
         */
        virtual Move play( const OthelloBoard& board );
    private:
};

MyBot::MyBot( Turn turn )
    : OthelloPlayer( turn )
{
}

int alphabeta(MinMaxNode* node, int depth, int alpha, int beta, Turn turn)
{
    //Turn tt = (((node->depth) % 2) == 0) ? turn : other(turn);
    //cout << "Current turn: ";
    //if (tt == RED) cout << "RED\n";
    //else cout << "BLACK\n";
    //cout << *node;
    //cout << "alpha: " << alpha << endl;
    //cout << "beta: " << beta << endl;
    //cout << "first turn: " << turn << endl;
    list<Move> moves = node->board.getValidMoves((((node->depth) % 2) == 0) ? turn : other(turn));
    if (depth == lookahead || moves.empty())
    {
        return heuristic(node->board);
    }

    bool maximizer = 0;
    if (node->depth % 2 == 0) maximizer = 1;
    else maximizer = 0;
    if (turn == RED) maximizer = !maximizer;

    if (maximizer)
    {
        int value = MIN;
        //MinMaxNode* local_max = new MinMaxNode(node->board);
        for (Move move : moves)
        {
            MinMaxNode* child = new MinMaxNode(node->board);
            child->board.makeMove((node->depth % 2 == 0) ? turn : other(turn), move);
            child->depth = node->depth + 1;
            child->transition = move;
            //value = max(value, alphabeta(child, depth + 1, alpha, beta, turn));
            int child_value = alphabeta(child, depth + 1, alpha, beta, turn);
            if (child_value > value)
            {
                value = child_value;
                //local_max = child;
                node->optimal_child = child;
            }
            if (value > alpha)
            {
                alpha = value;
                node->optimal_child = child;
            }
            if (alpha >= beta) break;
        }
        return value;

    }

    else
    {
        int value = MAX;
        for (Move move : moves)
        {
            MinMaxNode* child = new MinMaxNode(node->board);
            child->board.makeMove((node->depth % 2 == 0) ? turn : other(turn), move);
            child->depth = node->depth + 1;
            child->transition = move;
            //value = max(value, alphabeta(child, depth + 1, alpha, beta, turn));
            int child_value = alphabeta(child, depth + 1, alpha, beta, turn);
            if (child_value < value)
            {
                value = child_value;
                node->optimal_child = child;
                //local_max = child;
            }
            if (value < beta)
            {
                beta = value;
                node->optimal_child = child;
            }
            if (alpha >= beta) break;
        }
        return value;
    }



}


Move MyBot::play( const OthelloBoard& board )
{
    MinMaxNode* r=new MinMaxNode(board);
    r->depth = 0;
    r->heuristic = alphabeta(r,0,MIN,MAX,turn);
    Move res_move = r->optimal_child->transition;
    //cout << "Final Move: " << res_move.x << ' ' << res_move.y << endl;
    return r->optimal_child->transition;
    //return *it;
}

// The following lines are _very_ important to create a bot module for Desdemona

extern "C" {
    OthelloPlayer* createBot( Turn turn )
    {
        return new MyBot( turn );
    }

    void destroyBot( OthelloPlayer* bot )
    {
        delete bot;
    }
}



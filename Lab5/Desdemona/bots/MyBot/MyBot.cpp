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

#include <iostream>

#define MIN -1'000'000'000
#define MAX 1'000'000'000

using namespace std;
using namespace Desdemona;

const int lookahead = 5;

struct MinMaxNode
{
    int heuristic;
    OthelloBoard board;
    vector<MinMaxNode*> children;
    MinMaxNode* optimal_child;
    Move transition;
    int depth;
    //Turn turn;
    MinMaxNode(const OthelloBoard& other):transition(0,0), board(other) {}
};

//MinMaxNode* root;

//Black is maximizer and red is minimizer
int heuristic(OthelloBoard& board)
{
    //cout << "Entered heuristic\n";
    return board.getBlackCount() - board.getRedCount();
}

int minimax(MinMaxNode* node, Turn turn)
{
    //cout << "Entered minimax\n";
    if (node->depth == lookahead) return node->heuristic;
    bool maximizer=0;
    if (node->depth % 2 == 0) maximizer = 1;
    else maximizer = 0;
    if (turn == RED) maximizer = !maximizer;
    if (maximizer)
    {
        int max_val = MIN;
        for (MinMaxNode* child : node->children)
        {
            int val = minimax(child, turn);
            //max_val = max(val, max_val);
            if (val > max_val)
            {
                max_val = val;
                node->optimal_child = child;
            }
        }
        return max_val;
    }
    else
    {
        int min_val = MAX;
        for (MinMaxNode* child : node->children)
        {
            int val = minimax(child, turn);
            //min_val = min(val, min_val);
            if (val < min_val)
            {
                min_val = val;
                node->optimal_child = child;
            }
        }
        return min_val;
    }

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



Move MyBot::play( const OthelloBoard& board )
{
    //list<Move> moves = board.getValidMoves( turn );
//    int randNo = rand() % moves.size();
//    list<Move>::iterator it = moves.begin();
   // cout << "Got till here\n Original Turn:";

    //if (turn == RED) cout << "RED\n";
    //if (turn == BLACK) cout << "BLACK\n";

    MinMaxNode* r=new MinMaxNode(board);
    //r.heuristic = heuristic(board);
    //r.board = board;
    r->depth = 0;
    queue<MinMaxNode*> q;
    //root = r;
    q.push(r);
    
    //cout << "Root pushed to queue\n";
    while (!q.empty())
    {
       // cout << "Loop started\n";
        MinMaxNode* parent = q.front();
        q.pop();
        //cout << "Pooped\n";
        if (parent->depth == lookahead)
        {
            parent->heuristic = heuristic(parent->board);
            continue;
        }
        //cout << "Temp depth: " << parent->depth << endl;
        list<Move> moves = parent->board.getValidMoves((((parent->depth)%2)==0) ? turn : other(turn));
        //cout << "Valid Moves:" << endl;
        //for (auto move : moves)
        //{
        //    cout << move.x << ' ' << move.y << endl;
        //}

        //cout << "Temp borad outside for loop:\n";
        //parent->board.print();
        if (moves.empty())
        {
            parent->depth = lookahead;
            continue;
        }
        
        for (auto move : moves)
        {
           // cout << "Inside move loop\n";
            MinMaxNode *child=new MinMaxNode(parent->board);
            //cout << "Temp borad:\n";
            //parent->board.print();
            
            //auto child_board = parent->board;
           
            //cout << "Child board:\n";
            //child->board.print();
            //cout << "Before move\n";
            //try
            //{
            //cout << "Parent Depth : " << parent->depth << endl;
           
            //int tt = (parent->depth % 2 == 0) ? turn : other(turn);
            //if (tt == RED) cout << "RED\n";
            //if (tt == BLACK) cout << "BLACK\n";
            child->board.makeMove((parent->depth % 2 == 0) ? turn : other(turn), move);
            //}
            //catch(InvalidMoveException e)
            //{
            //    cout << "Fucked "<<e.move.x<<' '<<e.move.y<<endl;
            //}
            child->depth = parent->depth + 1;
            //cout << "After move\n";
            //child->board.print();
            //child.heuristic = heuristic(child_board);
            //child.board = child_board;
            
            child->transition = move;
            parent->children.emplace_back(child);
           // cout << "Child pushed\n";
            q.push(child);
        }
    }

    r->heuristic = minimax(r, turn);

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



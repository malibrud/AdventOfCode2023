// Solution to part 1 of https://adventofcode.com/2023/day/17
// Mark Brudnak
// Dec 28, 2023

// Inspired by 2021, Day 15 solution: https://github.com/malibrud/AdventOfCode2021/blob/master/15/main.cpp

#include <cassert>
#include <climits>

import std;
import std.compat;
using namespace std;

bool tryParseFileContents( char *fileName, vector<string>& outMap )
{
    try
    {
        // Open the file for reading
        ifstream file( fileName );
        if ( !file.is_open() ) return false;

        string line;
        while ( getline( file, line ) ) outMap.push_back( line );
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
        return false;
    }
    return true;
}

enum EnterDirection
{
    E,
    N,
    W,
    S,
    O
};

struct Node
{
    int f, g, h;
    int loss;
    EnterDirection d;
    int r, c;
    bool inOpenSet;
    Node *prev;

    Node()
    {
        loss = 0;
        r  = c = 0;
        f = g = h = INT_MAX;
        inOpenSet = false;
        prev = nullptr;
    }
};

struct Compare
{
    bool operator()(const Node *n1, const Node *n2)
    {
        return n1->f > n2->f;
    }
};

int main( int argc, char *argv[] )
{
    // Make sure we have an argument
    if ( argc != 2 ) return 1;

    vector<string> costMap;
    if ( !tryParseFileContents( argv[1], costMap ) ) return 1;
    int R = costMap.size();
    int C = costMap[0].size();
    
    // Populate the A* grids
    vector< vector< Node > > egrid( R, vector<Node>( C ) );
    vector< vector< Node > > ngrid( R, vector<Node>( C ) );
    vector< vector< Node > > wgrid( R, vector<Node>( C ) );
    vector< vector< Node > > sgrid( R, vector<Node>( C ) );
    for ( int r = 0 ; r < R ; r++ )
    for ( int c = 0 ; c < C ; c++ )
    {
        auto& n = egrid[r][c];
        n.loss = costMap[r][c] - '0';
        n.r = r;
        n.c = c;
        ngrid[r][c] = n;
        wgrid[r][c] = n;
        sgrid[r][c] = n;

        egrid[r][c].d = E;
        ngrid[r][c].d = N;
        wgrid[r][c].d = W;
        sgrid[r][c].d = S;
    }
    egrid[0][0].d = O;
    ngrid[0][0].d = O;
    wgrid[0][0].d = O;
    sgrid[0][0].d = O;

    auto& n = egrid[0][0];
    n.g = 0;
    n.h = R - 1 + C - 1;
    n.f = n.g + n.h;

    priority_queue< Node*, vector<Node*>, Compare > openSet;
    openSet.push( &egrid[0][0] );

    int directions[4][3] = 
    {
        {-1,  0, E}, 
        { 0,  1, N},
        { 1,  0, W}, 
        { 0, -1, S} 
    };

    Node *current;
    while ( !openSet.empty() )
    {
        current = openSet.top();
        openSet.pop();
        current->inOpenSet = false;

        if ( current->r == R-1 && current->c == C-1 ) break;

        for ( auto d = 0 ; d < 4 ; d++)
        {
            auto cc = current->c;
            auto cr = current->r;
            auto di = directions[d][2];
            auto dc = directions[d][1];
            auto dr = directions[d][0];
            auto nc = cc + dc;
            auto nr = cr + dr;

            // limit linear runs to no more than 3.
            if ( 
                // Look one back
                current->prev && 
                current->prev->d == di
                &&
                // Look two back
                current->prev->prev && 
                current->prev->prev->d == di
                // &&
                // Look three back
                // current->prev->prev->prev && 
                // current->prev->prev->prev->d == di
            ) continue;

            // Prevent backtracking
            if ( di == E && current->d == W ) continue;
            if ( di == N && current->d == S ) continue;
            if ( di == W && current->d == E ) continue;
            if ( di == S && current->d == N ) continue;

            if ( nr < 0 || nr >= R ) continue;
            if ( nc < 0 || nc >= C ) continue;

            Node *neighbor;
            switch (di)
            {
            case E: neighbor = &egrid[nr][nc]; break;
            case N: neighbor = &ngrid[nr][nc]; break;
            case W: neighbor = &wgrid[nr][nc]; break;
            case S: neighbor = &sgrid[nr][nc]; break;
            }
            auto g = current->g + neighbor->loss;
            auto h = R-nr-1 + C-nc-1;
            auto f = g + h;
            if ( f < neighbor->f )
            {
                neighbor->g = g;
                neighbor->h = h;
                neighbor->f = f;
                neighbor->prev = current;
                if ( !neighbor->inOpenSet )
                {
                    openSet.push( neighbor );
                    neighbor->inOpenSet = true;
                }
            }
        }
    }

    vector<string> figure( R, string( C, '.' ) );
    auto prev = current;
    int loss = 0;
    while (prev != nullptr )
    {
        figure[prev->r][prev->c] = '#';
        printf( "(%d, %d) => %d\n", prev->r, prev->c, prev->g );
        loss += prev->loss;
        prev = prev->prev;
    }

    for ( auto& row : figure )
    {
        cout << row << endl;
    }

    auto totalLoss = current->g;
    printf( "%d\n", totalLoss );
}
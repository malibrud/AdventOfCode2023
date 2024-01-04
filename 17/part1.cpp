// Solution to part 1 of https://adventofcode.com/2023/day/17
// Mark Brudnak
// Dec 28, 2023

// Inspired by 2021, Day 15 solution: https://github.com/malibrud/AdventOfCode2021/blob/master/15/main.cpp
// Aided by https://www.youtube.com/watch?v=2pDSooPLLkI

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

static int g_R = 0;
static int g_C = 0;

struct Node
{
    int r, c;
    int dr, dc;
    int g;
    int ndir;

    const int f() const { return g + ( g_R - 1 - r) + (g_C - 1 - c); }

    bool same( const Node& v ) const
    {
        return 
            r == v.r &&
            c == v.c &&
            dr == v.dr &&
            dc == v.dc &&
            ndir == v.ndir
        ;
    }

    bool isInBounds() { return r >= 0 && r < g_R && c >= 0 && c < g_C; }

    Node() : Node( 0, 0, 0, 0, 0, 0 ) {}
    Node(int _r, int _c, int _dr, int _dc, int _g, int _ndir )
    {
        r = _r;
        c = _c;
        dr = _dr;
        dc = _dc;
        g = _g;
        ndir = _ndir;
    }
};

struct Compare
{
    bool operator()(const Node& n1, const Node& n2) const
    {
        return n1.f() > n2.f();
    }
};

int main( int argc, char *argv[] )
{
    // Make sure we have an argument
    if ( argc != 2 ) return 1;

    vector<string> costMap;
    if ( !tryParseFileContents( argv[1], costMap ) ) return 1;
    g_R = costMap.size();
    g_C = costMap[0].size();
    
    set<Node, Compare> seen;
    priority_queue< Node, vector<Node>, Compare > openSet;

    openSet.emplace( 0, 0, 0, 0, 0, 0 );

    int directions[4][2] = 
    {
        {-1,  0 }, 
        { 0,  1 },
        { 1,  0 }, 
        { 0, -1 } 
    };

    Node current;
    while ( !openSet.empty() )
    {
        current = openSet.top();
        openSet.pop();

        if ( current.r == g_R-1 && current.c == g_C-1 ) break;

        if ( seen.end() != find_if( seen.begin(), seen.end(), [current]( const Node& v ) { return current.same(v); } ) 
        ) continue;

        seen.insert(current);

        // Try straight path
        if ( current.ndir < 3 )
        {
            int nr = current.r + current.dr;
            int nc = current.c + current.dc;
            int ng = current.g + costMap[nr][nc];
            Node neighbor(
                nr, nc,
                current.dr, current.dc,
                ng,
                current.ndir + 1
            );
            if ( neighbor.f() < current.f() && neighbor.isInBounds() ) openSet.push( neighbor );
        }

        // Try left turn
        {

        int ndr = +current.dc;
        int ndc = -current.dr;
        int nr = current.r + ndr;
        int nc = current.c + ndc;
        int ng = current.g + costMap[nr][nc];
        Node neighbor( nr, nc, ndr, ndc, ng, 1 );
        if ( neighbor.f() < current.f() && neighbor.isInBounds() ) openSet.push( neighbor );
        }

        {
        // Try right turn
        int ndr = -current.dc;
        int ndc = +current.dr;
        int nr = current.r + ndr;
        int nc = current.c + ndc;
        int ng = current.g + costMap[nr][nc];
        Node neighbor( nr, nc, ndr, ndc, ng, 1 );
        if ( neighbor.f() < current.f() && neighbor.isInBounds() ) openSet.push( neighbor );
        }
    }

    auto totalLoss = current.g;
    printf( "%d\n", totalLoss );
}
// Solution to part 1 of https://adventofcode.com/2023/day/10
// Mark Brudnak
// Dec 15, 2023

#include <cassert>

import std;
import std.compat;
using namespace std;

bool tryParseFileContents( char *fileName, vector<string>& outPipes )
{
    try
    {
        // Open the file for reading
        ifstream file( fileName );
        if ( !file.is_open() ) return false;

        string line;
        while( getline( file, line ) ) outPipes.push_back( line );
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
        return false;
    }
    return true;
}

int main( int argc, char *argv[] )
{
    // Make sure we have an argument
    if ( argc != 2 ) return 1;

    vector<string> pipes;
    if ( !tryParseFileContents( argv[1], pipes ) ) return 1;
    int R = pipes.size();
    int C = pipes[0].size();

    // Find the start location
    int r, c;
    int startRow, startCol;
    for ( r = 0 ; r < R ; r++ )
    for ( c = 0 ; c < C ; c++ )
    {
        if ( pipes[r][c] == 'S' )
        {
            startRow = r;
            startCol = c;
            goto DONE;
        }
    }
    DONE:

    // Walk the pipes counting our steps.
    char dir = 'A'; // Any direction
    int steps = 0;
    do
    {
        char P = pipes[r][c];

        if ( dir == 'A' )
        {
            char N, E, S, W;
            W = c > 0     ? pipes[r][c-1] : '.';
            E = c < C - 1 ? pipes[r][c+1] : '.';
            N = r > 0     ? pipes[r-1][c] : '.';
            S = r < R - 1 ? pipes[r+1][c] : '.';

            if ( W == '-' || W == 'L' || W == 'F' ) c--, dir = '<';    else
            if ( E == '-' || E == '7' || W == 'J' ) c++, dir = '>';    else
            if ( N == '|' || N == '7' || N == 'F' ) r--, dir = '^';    else
            if ( S == '|' || S == 'L' || S == 'J' ) r++, dir = 'v';    else
            assert(false);
        } else

        if ( dir == '<' && P == '-' ) c--, dir = '<'; else
        if ( dir == '<' && P == 'L' ) r--, dir = '^'; else
        if ( dir == '<' && P == 'F' ) r++, dir = 'v'; else

        if ( dir == '>' && P == '-' ) c++, dir = '>'; else
        if ( dir == '>' && P == 'J' ) r--, dir = '^'; else
        if ( dir == '>' && P == '7' ) r++, dir = 'v'; else

        if ( dir == '^' && P == '|' ) r--, dir = '^'; else
        if ( dir == '^' && P == '7' ) c--, dir = '<'; else
        if ( dir == '^' && P == 'F' ) c++, dir = '>'; else

        if ( dir == 'v' && P == '|' ) r++, dir = 'v'; else
        if ( dir == 'v' && P == 'J' ) c--, dir = '<'; else
        if ( dir == 'v' && P == 'L' ) c++, dir = '>'; else
        assert(false);

        steps++;
    } while ( r != startRow || c != startCol );
    
    printf( "%d\n", steps/2 );
    return 0;
}
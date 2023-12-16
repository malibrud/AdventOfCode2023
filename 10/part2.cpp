// Solution to part 2 of https://adventofcode.com/2023/day/10
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
    for ( r = 0 ; r < R ; r++ )
    for ( c = 0 ; c < C ; c++ )
    {
        if ( pipes[r][c] == 'S' ) goto DONE;
    }
    DONE:
    int startRow = r;
    int startCol = c;

    // Create a second copy of the map for annotation
    auto markers = pipes;
    for ( auto& mrow : markers ) mrow.assign( C, '.');

    // Walk the pipes marking the path with X
    char dir = 'A'; // Any direction
    do
    {
        char P = pipes[r][c];
        markers[r][c] = P;

        // We are at the starting location.  Figure out which way to go and place the correct marker.
        if ( dir == 'A' )
        {
            char N, E, S, W;
            W = c > 0     ? pipes[r][c-1] : '.';
            E = c < C - 1 ? pipes[r][c+1] : '.';
            N = r > 0     ? pipes[r-1][c] : '.';
            S = r < R - 1 ? pipes[r+1][c] : '.';

            bool Wexit = false; 
            bool Eexit = false; 
            bool Nexit = false; 
            bool Sexit = false;
            if ( W == '-' || W == 'L' || W == 'F' ) Wexit = true;
            if ( E == '-' || E == '7' || W == 'J' ) Eexit = true;
            if ( N == '|' || N == '7' || N == 'F' ) Nexit = true;
            if ( S == '|' || S == 'L' || S == 'J' ) Sexit = true;

            if ( Nexit && Sexit) P = '|';
            if ( Wexit && Eexit) P = '-';
            if ( Nexit && Eexit) P = 'L';
            if ( Nexit && Wexit) P = 'J';
            if ( Sexit && Wexit) P = '7';
            if ( Sexit && Eexit) P = 'F';
            markers[r][c] = P;

            if ( Nexit ) r--, dir = '^'; else
            if ( Sexit ) r++, dir = 'v'; else
            if ( Wexit ) c--, dir = '<'; else
            if ( Eexit ) c++, dir = '>'; else
            assert(false);
        } else

        // We entered the cell from the east
        if ( dir == '<' && P == '-' ) c--, dir = '<'; else
        if ( dir == '<' && P == 'L' ) r--, dir = '^'; else
        if ( dir == '<' && P == 'F' ) r++, dir = 'v'; else

        // We entered the cell from the west
        if ( dir == '>' && P == '-' ) c++, dir = '>'; else
        if ( dir == '>' && P == 'J' ) r--, dir = '^'; else
        if ( dir == '>' && P == '7' ) r++, dir = 'v'; else

        // We entered the cell from the south
        if ( dir == '^' && P == '|' ) r--, dir = '^'; else
        if ( dir == '^' && P == '7' ) c--, dir = '<'; else
        if ( dir == '^' && P == 'F' ) c++, dir = '>'; else

        // We entered the cell from the north
        if ( dir == 'v' && P == '|' ) r++, dir = 'v'; else
        if ( dir == 'v' && P == 'J' ) c--, dir = '<'; else
        if ( dir == 'v' && P == 'L' ) c++, dir = '>'; else
        assert(false);
    } while ( r != startRow || c != startCol );
    
    // Scan for interior points using odd boundary crossing rule:
    // Inside state switches when:
    //   1. We cross a '|'
    //   2. We encounter a pattern like 'FJ', 'F-J', 'F--J', etc.
    //   2. We encounter a pattern like 'L7', 'L-7', 'L--7', etc.
    int insideCount = 0;
    for ( r = 0 ; r < R ; r++ )
    {
        string& row = markers[r];
        bool inside = false;
        for ( c = 0 ; c < C ; c++ )
        {
            switch ( row[c] )
            {
            case '.':
                if ( inside ) insideCount++;
                break;
            case '|':
                inside = !inside;
                break;
            case 'F':
                while ( row[++c] == '-' );
                if ( row[c] == 'J') inside = !inside;
                break;
            case 'L':
                while ( row[++c] == '-' );
                if ( row[c] == '7') inside = !inside;
                break;
            default:
                assert( false );
            }
        }
    }

    printf( "%d\n", insideCount);
    return 0;
}
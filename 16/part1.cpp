// Solution to part 1 of https://adventofcode.com/2023/day/16
// Mark Brudnak
// Dec 28, 2023

#include <cassert>

import std;
import std.compat;
using namespace std;

bool tryParseFileContents( char *fileName, vector<string>& outContraption )
{
    try
    {
        // Open the file for reading
        ifstream file( fileName );
        if ( !file.is_open() ) return false;

        string line;
        while ( getline( file, line ) ) outContraption.push_back( line );
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
        return false;
    }
    return true;
}

struct Contraption
{
    int R;
    int C;
    vector<string> grid;
    vector<vector<int>> visitCounts;
    vector<vector<bool>> visitedFromLeft;
    vector<vector<bool>> visitedFromRight;
    vector<vector<bool>> visitedFromTop;
    vector<vector<bool>> visitedFromBottom;

    void init()
    {
        R = grid.size();
        C = grid[0].size();
        visitCounts.assign( R, vector<int>( C, 0 ) );
        vector<bool> row( C, false );
        visitedFromLeft.assign( R, row );
        visitedFromRight.assign( R, row );
        visitedFromTop.assign( R, row );
        visitedFromBottom.assign( R, row );
    }

    void doBeam( int r, int c, int dr, int dc )
    {
        while ( true )
        {
            if ( r < 0 || r >= R ) return;
            if ( c < 0 || c >= C ) return;

            visitCounts[r][c]++;

            if ( dr == 0 && dc == 1 )
            {
                if ( visitedFromLeft[r][c] ) return;
                visitedFromLeft[r][c] = true;
            } else
            if ( dr == 0 && dc == -1 )
            {
                if ( visitedFromRight[r][c] ) return;
                visitedFromRight[r][c] = true;
            } else
            if ( dr == 1 && dc == 0 )
            {
                if ( visitedFromTop[r][c] ) return;
                visitedFromTop[r][c] = true;
            } else
            if ( dr == -1 && dc == 0 )
            {
                if ( visitedFromBottom[r][c] ) return;
                visitedFromBottom[r][c] = true;
            }

            switch ( grid[r][c] )
            {
            case '.':
                {
                    r += dr;
                    c += dc;
                } break;
            case '/':
                {
                    int odr = dr;
                    int odc = dc;
                    if ( odr == 0 )
                    {
                        dr = -odc;
                        dc = odr;
                    } else
                    {
                        dr = odc;
                        dc = -odr;
                    }
                    r += dr;
                    c += dc;
                } break;
            case '\\':
                {
                    int odr = dr;
                    int odc = dc;
                    dr = odc;
                    dc = odr;
                    r += dr;
                    c += dc;
                } break;
            case '-':
                {
                    if ( dr == 0 )
                    {
                        r += dr;
                        c += dc;
                    } else
                    {
                        doBeam( r, c-1, 0, -1 );
                        doBeam( r, c+1, 0, +1 );
                        return;
                    }
                } break;
            case '|':
                {
                    if ( dc == 0 )
                    {
                        r += dr;
                        c += dc;
                    } else
                    {
                        doBeam( r-1, c, -1, 0 );
                        doBeam( r+1, c, +1, 0 );
                        return;
                    }
                } break;
            
            default:
                break;
            }
        }
    }
};

int main( int argc, char *argv[] )
{
    // Make sure we have an argument
    if ( argc != 2 ) return 1;

    Contraption contraption;
    if ( !tryParseFileContents( argv[1], contraption.grid ) ) return 1;
    contraption.init();

    contraption.doBeam( 0, 0, 0, 1 );
    int R = contraption.R;
    int C = contraption.C;

    int count = 0;
    for ( int r = 0 ; r < R ; r++ )
    for ( int c = 0 ; c < C ; c++ )
    {
        if ( contraption.visitCounts[r][c] > 0 ) count++;
    }

    printf( "%d\n", count );
}
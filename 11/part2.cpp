// Solution to part 2 of https://adventofcode.com/2023/day/11
// Mark Brudnak
// Dec 16, 2023

#include <cassert>
#include <inttypes.h>

import std;
import std.compat;
using namespace std;

const int64_t EXPANSION_FACTOR = 1000 * 1000;

bool tryParseFileContents( char *fileName, vector<string>& outUniverse )
{
    try
    {
        // Open the file for reading
        ifstream file( fileName );
        if ( !file.is_open() ) return false;

        string line;
        while( getline( file, line ) ) outUniverse.push_back( line );
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
        return false;
    }
    return true;
}

struct Galaxy
{
    int r, c;
    Galaxy( int _r, int _c ): r(_r), c(_c) {}
};

int main( int argc, char *argv[] )
{
    // Make sure we have an argument
    if ( argc != 2 ) return 1;

    vector<string> universe;
    if ( !tryParseFileContents( argv[1], universe ) ) return 1;
    int R = universe.size();
    int C = universe[0].size();

    // Find the galaxies
    vector<Galaxy> galaxies;
    for ( int r = 0 ; r < R ; r++ )
    for ( int c = 0 ; c < C ; c++ )
    {
        if ( universe[r][c] == '#' ) galaxies.emplace_back( r, c );
    }
    int G = galaxies.size();

    // Find the expansion rows
    vector<int> expandedRows;
    for ( int r = 0 ; r < R ; r++ )
    {
        bool galaxyFound = false;
        for ( int c = 0 ; c < C ; c++ ) 
        {
            if ( universe[r][c] == '#' ) galaxyFound = true;
        }
        if ( !galaxyFound ) expandedRows.push_back( r );
    }

    // Find the expansion cols
    vector<int> expandedCols;
    for ( int c = 0 ; c < C ; c++ )
    {
        bool galaxyFound = false;
        for ( int r = 0 ; r < R ; r++ ) 
        {
            if ( universe[r][c] == '#' ) galaxyFound = true;
        }
        if ( !galaxyFound ) expandedCols.push_back( c );
    }

    uint64_t sum = 0;
    for ( int i = 0   ; i < G ; i++ )
    for ( int j = i+1 ; j < G ; j++ )
    {
        auto& g1 = galaxies[i];
        auto& g2 = galaxies[j];

        uint64_t dist = 0;
        // Get the unexpanded distance.
        dist += abs( g2.r - g1.r );
        dist += abs( g2.c - g1.c );

        // Get the column expansions.
        int c1 = min( g1.c, g2.c );
        int c2 = max( g1.c, g2.c );
        for ( auto c : expandedCols ) if ( c1 < c && c < c2 ) dist += EXPANSION_FACTOR-1;

        // Get the row expansions.
        int r1 = min( g1.r, g2.r );
        int r2 = max( g1.r, g2.r );
        for ( auto r : expandedRows ) if ( r1 < r && r < r2 ) dist += EXPANSION_FACTOR-1;

        // Accumulate the sum
        sum += dist;
    }

    printf( "%lld\n", sum );
    return 0;
}
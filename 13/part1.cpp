// Solution to part 1 of https://adventofcode.com/2023/day/13
// Mark Brudnak
// Dec 21, 2023

#include <cassert>

import std;
import std.compat;
using namespace std;

bool tryParseFileContents( char *fileName, vector<vector<string>>& outPatterns )
{
    try
    {
        // Open the file for reading
        ifstream file( fileName );
        if ( !file.is_open() ) return false;

        // Parse the each line
        string line;
        while( !file.eof() )
        {
            vector<string> pattern;
            while ( getline( file, line ) ) 
            {
                if ( line == "" ) break;
                pattern.emplace_back( move( line ) );
            }
            outPatterns.emplace_back( move( pattern ) );
        }
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

    vector<vector<string>> patterns;
    if ( !tryParseFileContents( argv[1], patterns ) ) return 1;

    int sum = 0;

    for ( auto& pattern : patterns )
    {
        int W = pattern[0].size();
        int H = pattern.size();

        // Check for L-R symmetry
        for ( int ic = 0 ; ic < W-1 ; ic++ )
        {
            bool isMirror = true;
            for ( int il = 0 ; il < H ; il++ )
            {
                auto& line = pattern[il];
                int d = min( ic+1, W-ic-1 );
                for ( int j = 0 ; j < d ; j++ )
                {
                    int lIdx = ic-j;
                    int rIdx = ic+j+1;
                    isMirror = isMirror && ( line[lIdx] == line[rIdx] );
                    if ( !isMirror ) goto NOT_MIRROR;
                }
            }
            NOT_MIRROR:
            if ( isMirror ) 
                sum += ic + 1;
        }

        // Check for T-B symmetry
        for ( int ic = 0 ; ic < H-1 ; ic++ )
        {
            bool isMirror = true;
            int d = min( ic+1, H-ic-1 );
            for ( int j = 0 ; j < d ; j++ )
            {
                int tIdx = ic-j;
                int bIdx = ic+j+1;
                isMirror = isMirror && ( pattern[tIdx] == pattern[bIdx] );
            }
            if ( isMirror ) 
                sum += 100 * ( ic + 1 );
        }
    }

    printf( "%d\n", sum );
}
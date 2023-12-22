// Solution to part 2 of https://adventofcode.com/2023/day/13
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

vector<string> transposePattern( vector<string>& inPattern )
{
    int Wi = inPattern[0].size();
    int Hi = inPattern.size();
    int Wo = Hi;
    int Ho = Wi;

    string newRow( Wo, ' ' );
    vector<string> outPattern;
    for ( int ir = 0 ; ir < Ho ; ir++ )
    {
        for (int ic = 0 ; ic < Wo ; ic++ )
        {
            newRow[ic] = inPattern[ic][ir];
        }
        outPattern.push_back( newRow );
    }
    return outPattern;
}

bool isTopBottomSymmetricWithSmudge( vector<string>& pattern, int rowAboveReflectionLine )
{
    int W = pattern[0].size();
    int H = pattern.size();

    int diffCount = 0;
    int ic = rowAboveReflectionLine;
    int d = min( ic+1, H-ic-1 );
    for ( int j = 0 ; j < d ; j++ )
    {
        int tIdx = ic-j;
        int bIdx = ic+j+1;
        for ( int ic = 0 ; ic < W ; ic++ ) if ( pattern[tIdx][ic] != pattern[bIdx][ic] ) diffCount++;
    }
    return diffCount == 1;
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

        // Check for T-B symmetry
        for ( int ic = 0 ; ic < H-1 ; ic++ )
        {
            if ( isTopBottomSymmetricWithSmudge( pattern, ic  ) ) 
                sum += 100 * ( ic + 1 );
        }

        // Check for L-R symmetry by transposing and using the T-B algorithm
        auto tPattern = transposePattern( pattern );
        for ( int ic = 0 ; ic < H-1 ; ic++ )
        {
            if ( isTopBottomSymmetricWithSmudge( pattern, ic  ) ) 
                sum += 1 * ( ic + 1 );
        }
    }

    printf( "%d\n", sum );
}
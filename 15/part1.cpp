// Solution to part 1 of https://adventofcode.com/2023/day/15
// Mark Brudnak
// Dec 25, 2023

#include <cassert>

import std;
import std.compat;
using namespace std;

bool tryParseFileContents( char *fileName, string& outLine )
{
    try
    {
        // Open the file for reading
        ifstream file( fileName );
        if ( !file.is_open() ) return false;

        getline( file, outLine );
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

    string sequence;
    if ( !tryParseFileContents( argv[1], sequence ) ) return 1;
    int N = sequence.size();

    int sum = 0;
    int hash = 0;
    for ( auto c : sequence )
    {
        if ( c == ',' )
        {
            sum += hash;
            hash = 0;
            continue;
        }
        hash += c;
        hash *= 17;
        hash &= 0xff;
    }
    sum += hash;
    printf( "%d\n", sum );
}
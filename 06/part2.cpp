// Solution to part 1 of https://adventofcode.com/2023/day/6
// Mark Brudnak
// Dec 8, 2023

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
using namespace std;

bool tryParseFileContents( char *fileName, uint64_t& outTime, uint64_t& outDistance )
{
    try
    {
        // Open the file for reading
        ifstream file( fileName );
        if ( !file.is_open() ) return false;
        
        string line;

        // Read the first line containing the times.
        getline( file, line );
        outTime = 0;
        for ( auto c : line )
        {
            if ( c == ' ' ) continue;
            if ( c == ':' ) continue;
            if ( isalpha( c) ) continue;
            uint64_t place = c - '0';
            outTime = 10 * outTime + place;
        }

        // Read the second line containing the distances.
        getline( file, line );
        outDistance = 0;
        for ( auto c : line )
        {
            if ( c == ' ' ) continue;
            if ( c == ':' ) continue;
            if ( isalpha( c) ) continue;
            uint64_t place = c - '0';
            outDistance = 10 * outDistance + place;
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

    uint64_t tf; // Final time
    uint64_t dr; // Record distance
    if ( !tryParseFileContents( argv[1], tf, dr ) );

    auto root = sqrt( (double)tf * tf - 4.0 * dr );
    auto tc1 = (uint64_t)ceil( ( tf - root ) / 2 );
    auto tc2 = (uint64_t)ceil( ( tf + root ) / 2 );

    // Handle the cases where there is an exact tie.
    // We are only counting cases where we beat the record.
    if ( dr == tc1 * ( tf - tc1) ) tc1++;

    auto count = tc2 - tc1;

    printf( "%llu\n", count );
    return 0;
}
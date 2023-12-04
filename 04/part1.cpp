// Solution to part 1 of https://adventofcode.com/2023/day/4
// Mark Brudnak
// Dec 4, 2023

#include <fstream>
#include <string>
#include <set>
using namespace std;

int main( int argc, char *argv[] )
{
    // Make sure we have an argument
    if ( argc != 2 ) return 1;

    // Open the file for reading
    ifstream file( argv[1]);
    if ( !file.is_open() ) return 1;
    
    // process the lines (i.e. cards) one by one.
    int sum = 0;
    string line;
    while ( getline( file, line ) )
    {
        set<int> winningNumbers;
        int len = line.size();
        int colonIdx = line.find(':');
        int pipeIdx = line.find('|');
        
        // Put winning numbers in a set.
        for ( int i = colonIdx + 2 ; i < pipeIdx ; i +=3 )
        {
            winningNumbers.insert( atoi( &line[i] ) );
        }

        // Look to see if my numbers are in the set
        int matchCount = 0;
        for ( int i = pipeIdx + 2 ; i < len ; i +=3 )
        {
            int guess = atoi( &line[i] );
            if ( winningNumbers.find( guess ) != winningNumbers.end() ) matchCount++;
        }

        // if matches are found, sum up the points.
        if ( matchCount > 0 ) sum += 1 << ( matchCount - 1 );
    }

    printf( "%d\n", sum );
    return 0;
}
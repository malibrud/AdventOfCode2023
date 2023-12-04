// Solution to part 2 of https://adventofcode.com/2023/day/4
// Mark Brudnak
// Dec 4, 2023

#include <fstream>
#include <string>
#include <set>
#include <vector>
using namespace std;

int main( int argc, char *argv[] )
{
    // Make sure we have an argument
    if ( argc != 2 ) return 1;

    // Open the file for reading
    ifstream file( argv[1]);
    if ( !file.is_open() ) return 1;
    
    // Remember the card count
    vector<int> cardCounts;
    cardCounts.assign( 200, 1 ); // enough to hold the 193 cards in the data.

    // process the lines (i.e. cards) one by one.
    int currentCard = 0;
    int sum = 0;
    string line;
    while ( getline( file, line ) )
    {
        // Find key delimiters.
        int colonIdx = line.find(':');
        int pipeIdx = line.find('|');

        // Put winning numbers in a set.
        set<int> winningNumbers;
        for ( int i = colonIdx + 2 ; i < pipeIdx ; i +=3 )
        {
            winningNumbers.insert( atoi( &line[i] ) );
        }

        // Look to see if my numbers are in the set.
        int matchCount = 0;
        int len = line.size();
        for ( int i = pipeIdx + 2 ; i < len ; i +=3 )
        {
            int guess = atoi( &line[i] );
            if ( winningNumbers.find( guess ) != winningNumbers.end() ) matchCount++;
        }

        // Generate card copies.
        for (int i = 0 ; i <= matchCount ; i++ )
        {
            cardCounts[ currentCard + 1 + i ] += cardCounts[ currentCard ];
        }

        // Accumulate the total
        sum += cardCounts[ currentCard++ ];
    }

    // Print the result.
    printf( "%d\n", sum );
    return 0;
}
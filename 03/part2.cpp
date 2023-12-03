// Solution to part 2 of https://adventofcode.com/2023/day/3
// Mark Brudnak
// Dec 3, 2023

#include <fstream>
#include <string>
#include <vector>
#include <cassert>
using namespace std;

int main( int argc, char *argv[] )
{
    // Make sure we have an argument
    if ( argc != 2 ) return 1;

    // Open the file for reading
    ifstream file( argv[1]);
    if ( !file.is_open() ) return 1;
    
    // Read the lines into a vector of strings.
    vector<string> array;
    string line;
    while ( getline( file, line ) ) array.push_back( line );

    // Get the array extents
    int C = array[0].size();
    int R = array.size();

    // Scan for 'part numbers' meeting the conditions.
    int sum = 0;
    for ( int ir = 0 ; ir < R ; ir++ )
    for ( int ic = 0 ; ic < R ; ic++ )
    {
        if ( '*' != array[ir][ic] ) continue;

        // We found a 'gear', now look for part numbers around the 'gear'
        // First define the box to search.
        int rStart = max( 0, ir - 1 );
        int rEnd   = min( R, ir + 2 );
        int cStart = max( 0, ic - 1 );
        int cEnd   = min( C, ic + 2 );

        // Print out information about the region around the candidate gear.
        printf( "* found at [%d, %d]\n", ir, ic );
        for ( int i = rStart ; i < rEnd ; i++ )
        {
            printf( "    %.*s\n", cEnd - cStart, &array[i][cStart] );
        }
        printf( "   ==> " );

        // Scan the box for a 'part number' and count parts and calculate ratio.
        int nParts = 0;
        int ratio = 1;
        for ( int i = rStart ; i < rEnd ; i++ )
        for ( int j = cStart ; j < cEnd ; )
        {
            // if not a part number keep moving...
            if ( !isdigit( array[i][j] ) ) 
            {
                j++;
                continue;
            }

            // We found a part number!
            int istart = j;
            // Find the first digit and get the value.
            while ( istart > 0 && isdigit( array[i][istart-1] ) ) istart--;
            int val = atoi( &array[i][istart] );
            printf( "%d ", val );
            nParts++;
            ratio *= val;

            // Scan past the current part.
            while ( ++j < cEnd && isdigit( array[i][j] ) ) /*empty*/;
        }

        // If we found a gear increment the sum.
        assert( nParts <= 2 );
        if ( nParts == 2 )
        {
            printf( ": ratio = %d\n\n", ratio );
            sum += ratio;
        }
        else
        {
            printf( ": Not a gear!\n\n");
        }
    }

    // Report the answer.
    printf( "%d\n", sum );
    return 0;
}
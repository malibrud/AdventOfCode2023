// Solution to part 1 of https://adventofcode.com/2023/day/3
// Mark Brudnak
// Dec 3, 2023

#include <fstream>
#include <string>
#include <vector>
using namespace std;

// Test if the caracter is a 'symbol' according to the rules, i.e. '.' is not a symbol.
bool isSymbol( char c)
{
    if ( c == '.'   ) return false; // not a 'symbol' as defined by the game.
    if ( c <= ' '   ) return false;
    if ( isdigit(c) ) return false;
    if ( isalpha(c) ) return false;
    return true;
}

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
    for ( int ic = 0 ; ic < R ;      )
    {
        if ( !isdigit( array[ir][ic] ) ) 
        {
            ic++;
            continue;
        }

        // We found a 'part number', note its start and end;
        int start = ic;
        while ( isdigit( array[ir][ic] ) ) ic++;
        int end = ic;
        printf( "%.*s", end - start, &array[ir][start] );

        // Define the box around the digit, keeping careful not to fall off of an edge.
        int rStart = max( 0, ir - 1 );
        int rEnd   = min( R, ir + 2 );
        int cStart = max( 0, start - 1 );
        int cEnd   = min( C, end   + 1 );

        // Scan the box for a 'symbol' and increment sum if found.
        for ( int i = rStart ; i < rEnd ; i++ )
        for ( int j = cStart ; j < cEnd ; j++ )
        {
            if ( isSymbol( array[i][j] ) ) 
            {
                sum += atoi( &array[ir][start] );
                printf( " ==> yes\n" );
                goto DoneScanningForSymbols;
            }
        }
        printf( " ==> no\n" );
    DoneScanningForSymbols:
        /* empty */;
    }
    printf( "%d\n", sum );
    return 0;
}
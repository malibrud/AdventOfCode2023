// Solution to part 1 of https://adventofcode.com/2023/day/6
// Mark Brudnak
// Dec 8, 2023

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
using namespace std;

bool tryParseFileContents( char *fileName, vector<int>& outTimes, vector<int>& outDistances )
{
    try
    {
        // Open the file for reading
        ifstream file( fileName );
        if ( !file.is_open() ) return false;
        
        // Read the first line containing the times.
        file.seekg( 11, ios_base::cur );
        while( '\n' != file.peek() )
        {
            int val;
            file >> val;
            outTimes.push_back( val );
        }

        // Read the second line containing the distances.
        file.seekg( 12, ios_base::cur );
        while( !file.eof() )
        {
            int val;
            file >> val;
            outDistances.push_back( val );
        }
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
        return false;
    }

    if ( outTimes.size() != outDistances.size() ) return false;

    return true;
}

int main( int argc, char *argv[] )
{
    // Make sure we have an argument
    if ( argc != 2 ) return 1;

    vector<int> times;
    vector<int> dists;
    if ( !tryParseFileContents( argv[1], times, dists ) );

    int product = 1;
    for ( int i = 0 ; i < times.size() ; i++ )
    {
        auto tf = times[i]; // Final time
        auto dr = dists[i]; // Record distance

        // Distance equation:
        //
        // d = tc * ( tf - tc )
        //
        // where 0 <= tc <= tf is the charge time.
        // The points where d == dr are solutions to the equation
        //
        // tc^2 - tf*tc + dr = 0
        //
        // the quadratic equation solution for tc is:
        //
        //       tf ± √(tf² - 4*dr)
        // tc = --------------------
        //             2
        //
        // Compute both values of this equation and round up
        // for the first and down for the second so that the difference
        // is the count we need.

        auto root = sqrt( tf * tf - 4.0 * dr );
        auto tc1 = (int)ceil( ( tf - root ) / 2 );
        auto tc2 = (int)ceil( ( tf + root ) / 2 );

        // Handle the cases where there is an exact tie.
        // We are only counting cases where we beat the record.
        if ( dr == tc1 * ( tf - tc1) ) tc1++;

        auto count = tc2 - tc1;
        product *= count;
    }

    printf( "%d\n", product );
    return 0;
}
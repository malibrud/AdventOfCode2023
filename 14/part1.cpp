// Solution to part 1 of https://adventofcode.com/2023/day/14
// Mark Brudnak
// Dec 22, 2023

#include <cassert>

import std;
import std.compat;
using namespace std;

bool tryParseFileContents( char *fileName, vector<string>& outLines )
{
    try
    {
        // Open the file for reading
        ifstream file( fileName );
        if ( !file.is_open() ) return false;

        // Parse the each line
        string line;
        while ( getline( file, line ) ) outLines.emplace_back( move( line ) ); 
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
        return false;
    }
    return true;
}

vector<string> transpose( vector<string>& inData )
{
    int Wi = inData[0].size();
    int Hi = inData.size();
    int Wo = Hi;
    int Ho = Wi;

    string newRow( Wo, ' ' );
    vector<string> outData;
    for ( int ir = 0 ; ir < Ho ; ir++ )
    {
        for (int ic = 0 ; ic < Wo ; ic++ )
        {
            newRow[ic] = inData[ic][ir];
        }
        outData.push_back( newRow );
    }
    return outData;
}

int main( int argc, char *argv[] )
{
    // Make sure we have an argument
    if ( argc != 2 ) return 1;

    vector<string> platform;
    if ( !tryParseFileContents( argv[1], platform ) ) return 1;

    int sum = 0;

    // Process by row rather than by column.
    auto platformByRow = transpose( platform );
    for ( auto row : platformByRow )
    {
        int N = row.size();

        for ( int i = 0 ; i < N ; i++ )
        {
            switch ( row[ i ] )
            {
            case 'O':
                sum += N - i;
                break;
            case '#':
                break;
            case '.':
                for ( int j = i + 1 ; j < N ; j++ )
                {
                    if ( row[j] == 'O' )
                    {
                        row[i] = 'O';
                        row[j] = '.';
                        sum += N - i;
                        break;
                    } else
                    if ( row[j] == '#') break;
                }
                break;
            }
        }
    }

    printf( "%d\n", sum );
}
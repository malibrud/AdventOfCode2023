// Solution to part 1 of https://adventofcode.com/2023/day/14
// Mark Brudnak
// Dec 22, 2023

#include <cassert>

const int N_CYCLES = 1'000'000'000;
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

void rollNorth( vector<string>& inoutPlatform )
{
    auto& plt = inoutPlatform;
    int R = inoutPlatform.size();
    int C = inoutPlatform[0].size();

    for ( int c = 0 ; c < C ; c++ )
    for ( int r = 0 ; r < R ; r++ )
    {
        if ( plt[r][c] != '.' ) continue;
        for ( int j = r + 1 ; j < R ; j++ )
        {
            if ( plt[j][c] == 'O' )
            {
                plt[r][c] = 'O';
                plt[j][c] = '.';
                break;
            } else
            if ( plt[j][c] == '#') break;
        }
    }
}

void rollSouth( vector<string>& inoutPlatform )
{
    auto& plt = inoutPlatform;
    int R = inoutPlatform.size();
    int C = inoutPlatform[0].size();

    for ( int c = 0   ; c < C  ; c++ )
    for ( int r = R-1 ; r >= 0 ; r-- )
    {
        if ( plt[r][c] != '.' ) continue;
        for ( int j = r - 1 ; j >= 0 ; j-- )
        {
            if ( plt[j][c] == 'O' )
            {
                plt[r][c] = 'O';
                plt[j][c] = '.';
                break;
            } else
            if ( plt[j][c] == '#') break;
        }
    }
}

void rollEast( vector<string>& inoutPlatform )
{
    auto& plt = inoutPlatform;
    int R = inoutPlatform.size();
    int C = inoutPlatform[0].size();

    for ( int r = 0   ; r < R  ; r++ )
    for ( int c = C-1 ; c >= 0 ; c-- )
    {
        if ( plt[r][c] != '.' ) continue;
        for ( int j = c - 1 ; j >= 0 ; j-- )
        {
            if ( plt[r][j] == 'O' )
            {
                plt[r][c] = 'O';
                plt[r][j] = '.';
                break;
            } else
            if ( plt[r][j] == '#') break;
        }
    }
}

void rollWest( vector<string>& inoutPlatform )
{
    auto& plt = inoutPlatform;
    int R = inoutPlatform.size();
    int C = inoutPlatform[0].size();

    for ( int r = 0 ; r < R ; r++ )
    for ( int c = 0 ; c < C ; c++ )
    {
        if ( plt[r][c] != '.' ) continue;
        for ( int j = c + 1 ; j < C ; j++ )
        {
            if ( plt[r][j] == 'O' )
            {
                plt[r][c] = 'O';
                plt[r][j] = '.';
                break;
            } else
            if ( plt[r][j] == '#') break;
        }
    }
}

int main( int argc, char *argv[] )
{
    // Make sure we have an argument
    if ( argc != 2 ) return 1;

    vector<string> platform;
    if ( !tryParseFileContents( argv[1], platform ) ) return 1;

    // Look for periodic structure in the cycles.
    int periodLen;
    int periodStart;
    map< vector<string>, int > resultToCycle;
    for ( int i = 1 ; i <= N_CYCLES ; i++ )
    {
        rollNorth( platform );
        rollWest(  platform );
        rollSouth( platform );
        rollEast(  platform );

        if ( resultToCycle.find( platform ) == resultToCycle.end() )
        {
            resultToCycle[ platform ] = i;
        }
        else
        {
            periodStart = resultToCycle[ platform ];
            periodLen = i - periodStart;
            printf( "cycle found %d and %d\n",  periodStart, periodLen ); 
            break;
        }
    }

    // Use period to find the map at 1,000,000,000 cycles.
    int endIdx = ( N_CYCLES - periodStart ) % periodLen + periodStart;
    for ( auto& item : resultToCycle )
    {
        if ( item.second == endIdx ) 
            platform = item.first;
    }

    // Sum up the weights.
    int R = platform.size();
    int C = platform[0].size();
    int sum = 0;
    for ( int r = 0 ; r < R ; r++ )
    for ( int c = 0 ; c < C ; c++ )
    {
        if ( platform[r][c] == 'O' ) sum += R - r;
    }
    printf( "%d\n", sum );
}
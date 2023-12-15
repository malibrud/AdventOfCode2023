// Solution to part 1 of https://adventofcode.com/2023/day/9
// Mark Brudnak
// Dec 15, 2023

import std;
import std.compat;
using namespace std;

bool tryParseFileContents( char *fileName, vector<vector<int>>& outHistories )
{
    try
    {
        // Open the file for reading
        ifstream file( fileName );
        if ( !file.is_open() ) return false;

        regex numPattern( R"((-?\d+))" );
        string line;
        while( getline( file, line ) )
        {
            vector<int> values;
            for (
                auto it = sregex_iterator( line.begin(), line.end(), numPattern ) ;
                it != sregex_iterator() ;
                ++it
            ) {
                auto match = *it;
                int val = stoi( match[1]);
                values.push_back( val );
            }
            outHistories.push_back( move( values ) );
        }
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
        return false;
    }
    return true;
}

int getNextVal( vector<int> hist )
{
    if ( std::equal( hist.begin() + 1, hist.end(), hist.begin() ) ) return hist[0];
    int N = hist.size();
    int last = hist[ N - 1 ];
    for ( int i = 0 ; i < N-1 ; i++ )
    {
        hist[ i ] = hist[ i + 1 ] - hist[ i ];
    }
    hist.resize( N-1 );
    return last + getNextVal( hist );
}

int main( int argc, char *argv[] )
{
    // Make sure we have an argument
    if ( argc != 2 ) return 1;

    vector< vector<int> > histories;
    if ( !tryParseFileContents( argv[1], histories ) ) return 1;

    int sum = 0;
    for ( int i = 0 ; i < histories.size() ; i++ ) sum += getNextVal( histories[ i ] );

    printf( "%d\n", sum );
    return 0;
}
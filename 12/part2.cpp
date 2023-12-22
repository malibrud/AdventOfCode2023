// Solution to part 2 of https://adventofcode.com/2023/day/12
// Mark Brudnak
// Dec 18, 2023
// Inspired by:
//   [1] https://www.reddit.com/r/adventofcode/comments/18hbjdi/2023_day_12_part_2_this_image_helped_a_few_people/
//   [2] https://www.reddit.com/r/adventofcode/comments/18ge41g/2023_day_12_solutions/

#include <cassert>
#include <inttypes.h>

import std;
import std.compat;
using namespace std;

const int64_t INVALID = -1;

class Cache
{
    vector<int64_t> results;
    int rows;
    int cols;
    int idx( int r, int c )
    {
        int i = r * cols + c;
        assert( 0 <= i && i < results.size() );
        return i;
    }
public:
    Cache()
    {
        resize( 0, 0 );
    }
    void resize( int r, int c )
    {
        rows = r;
        cols = c;
        int N = rows * cols;
        results.resize( N );
        results.assign( N, INVALID );
    }
    int64_t get( int r, int c )
    {
        return results[ idx( r, c ) ];
    }
    void set( int r, int c, int64_t val )
    {
        results[ idx( r, c ) ] = val;
    }
};

struct SpringRecord
{
    Cache resultsCache;
    string record;      // e.g. "?.???.??????#??#???"
    vector<int> runs;   // e.g. 2,2,6

    int N;  // Length of record
    int R;  // Length of runs

    SpringRecord( string&& _record, vector<int>&& _runs)
        : record( _record )
        , runs( _runs )
        , N( record.size() )
        , R( runs.size() )
    {
        resultsCache.resize( N+1, R+1 );
    }

    void unfold( int nCopies )
    {
        if ( nCopies == 1 ) return;

        auto rec = record;
        record.reserve( nCopies*N + nCopies - 1 );

        // build up the record.
        for ( int i = 0 ; i < nCopies - 1 ; i++ )
        {
            record += '?';
            record += rec;
        }

        auto oldruns = runs;
        runs.reserve( nCopies * runs.size() );
        for ( int i = 0 ; i < nCopies - 1 ; i++ )
        {
            runs.insert( runs.end(), oldruns.begin(), oldruns.end() );
        }
        N = record.size();
        R = runs.size();
        resultsCache.resize( N+1, R+1 );
    }

    int64_t getArrangementCount( int recIdx, int runIdx )
    {
        // If indices exceed cache, pass call to actual calculation
        if ( recIdx > N || runIdx > R ) return _getArrangementCount( recIdx, runIdx );

        // Check the results cache
        int64_t result = resultsCache.get( recIdx, runIdx );
        if ( result != INVALID ) return result;

        // Compute and store
        result = _getArrangementCount( recIdx, runIdx );
        resultsCache.set( recIdx, runIdx, result );
        return result;
    }

private:
    int64_t _getArrangementCount( int recIdx, int runIdx )
    {
        // End condition 1.
        // Check to see if we are out of runs.
        // if we are then there should be no remaining '#'
        if ( runIdx == R )
        {
            for ( int i = recIdx ; i < N ; i++ ) if ( record[ i ] == '#' ) return 0;
            return 1;
        }

        // End condition 2.
        // Check to see if we are out of record entries
        if ( recIdx >= N) return 0;

        // Nornal condition
        int64_t count = 0;
        int runLen = runs[ runIdx ];
        switch ( record[ recIdx ] )
        {
        case '.': // Skip to next character
            return getArrangementCount( recIdx + 1, runIdx );

        case '#': // We are at the start of a run.  Try to greedily consume it up to the run length.
            if ( !isRunOfLength( recIdx, runLen ) ) return 0;
            return getArrangementCount( recIdx + runLen + 1, runIdx + 1 );

        case '?': // We could be at the start of run or not
            // as if it were a '.'
            count = getArrangementCount( recIdx + 1, runIdx );

            // as if it were a '#'
            if ( !isRunOfLength( recIdx, runLen ) ) return count;
            return count + getArrangementCount( recIdx + runLen + 1, runIdx + 1 );
        default:
            assert(false);
        }
        return 0;
    }

    bool isRunOfLength( int recIdx, int runLen )
    {
        // Not enough characters left to complete the run.
        if ( runLen > N - recIdx ) return false;

        // Look for an interruption of the expected sequence of # and ?
        for ( int i = recIdx ; i < recIdx + runLen ; i++ ) if ( record[ i ] == '.' ) return false;

        // Make sure the run is terminated.
        if ( recIdx + runLen == N ) return true; // Terminated by the end of the string
        return record[ recIdx + runLen ] != '#'; // Terminated by either a '.' or '?'
    }
};

bool tryParseFileContents( char *fileName, vector<SpringRecord>& outRecords )
{
    try
    {
        // Open the file for reading
        ifstream file( fileName );
        if ( !file.is_open() ) return false;

        // Parse the each line
        regex linePattern(R"(([\.#?]+) (.+))");
        string line;
        while( getline( file, line ) ) 
        {
            smatch match;
            regex_match( line, match, linePattern );
            auto record = match[1];
            auto runstr = match[2];
            stringstream ss( runstr );
            vector<int> runs;
            int val;
            while ( ss >> val )
            {
                runs.push_back( val );
                if (ss.peek() == ',') ss.ignore();
            }
            outRecords.emplace_back( move(record), move(runs) );
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

    try
    {
        vector<SpringRecord> records;
        if ( !tryParseFileContents( argv[1], records ) ) return 1;

        int64_t sum = 0;
        for ( auto& record : records )
        {
            record.unfold( 5 );
            printf(".\n");
            sum += record.getArrangementCount( 0, 0 );
        }

        printf( "%lld\n", sum );
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
    }
    
    return 0;
}
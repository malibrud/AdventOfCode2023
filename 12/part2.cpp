// Solution to part 2 of https://adventofcode.com/2023/day/12
// Mark Brudnak
// Dec 18, 2023

#include <cassert>
#include <inttypes.h>

import std;
import std.compat;
using namespace std;

struct SpringRecord
{
    string record;
    vector<int> runs;
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
            outRecords.emplace_back( record, runs );
        }
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
        return false;
    }
    return true;
}

// Gets the number of valid arrangements for a permutation of the '?'
// Should be either 0 or 1
uint64_t getArrangementCountForPermutation( SpringRecord& record, uint64_t permute )
{
    auto& rec = record.record;
    auto& runs = record.runs;
    int runLen = 0;

    int runIdx = 0;
    uint64_t wcMask = 1;
    for ( int i = 0; i < rec.size() ; i++ )
    {
        char health = rec[i];

        // Set the wildcard according to the permutation.
        if ( health == '?' )
        {
            health = ( permute & wcMask ) != 0 ? '#' : '.';
            wcMask <<= 1;
        }

        // Do the run counting and comparison
        switch ( health )
        {
        case '.':
            if ( runLen == 0 ) break;
            if ( runLen != runs[ runIdx++ ] ) return 0;
            runLen = 0;
            break;
        case '#':
            if ( runIdx >= runs.size() ) return 0;
            runLen++;
            break;
        default:
            assert(false);
        }
    }

    // Do final check in case the string ends with a '#'
    if ( runLen > 0 )
    {
        if ( runLen != runs[ runIdx++ ] ) return 0;
        runLen = 0;
    }

    // Final check to see if the count of actual runs equals what we expected.
    if ( runIdx < runs.size() ) return 0;

    return 1;
}

bool permuteBitOffsets( vector<int>& bitOffsets, int startIdx, int maxOffset )
{
    auto N = bitOffsets.size();

    // Test for the end of the recursion
    // Check to see if everything is packed to the right.
    if ( maxOffset - bitOffsets[ startIdx ] <= N - startIdx ) return false;

    // If we can permute bits down stream, then we are returning a new permutation.
    if ( permuteBitOffsets( bitOffsets, startIdx + 1, maxOffset ) ) return true;

    // We need to increment the start index and collapse the down stream bits to the left.
    int followCount = N - startIdx - 1;
    int val = bitOffsets[ startIdx ] + 1;
    for ( int i = startIdx ; i < N ; i++ )
    {
        bitOffsets[ i ] = val++;
    }
    return true;
}

// Gets the number of valid arrangements for all permutations of the '?'s
uint64_t getArrangementCount( SpringRecord& record )
{
    auto& rec = record.record;
    auto& runs = record.runs;

    // Determine how many of the total '?' could be '#' to reduce search space.
    int Nq = count( rec.begin(), rec.end(), '?' );
    int Np = count( rec.begin(), rec.end(), '#' );
    int Ntotal = reduce( runs.begin(), runs.end() );
    int Nones = Ntotal - Np;
    
    // Form an array which contains offsets to the bits which must be set.
    vector<int> bitOffsets( Nones );
    for ( int i = 0 ; i < Nones ; i++ ) bitOffsets[i] = i;

    uint64_t arrCount = 0;
    while ( permuteBitOffsets( bitOffsets, 0, Nq ) )
    {
        int N = bitOffsets.size();
        uint64_t permute = 0;
        for ( int i = 0 ; i < N ; i++ ) permute |= 1 << bitOffsets[ i ];
        arrCount += getArrangementCountForPermutation( record, permute );
    }
    return arrCount;
}

void expandRecord( SpringRecord& record )
{
    auto rec = record.record;
    auto len = rec.size();
    record.record.reserve( 5*len + 4 );

    // build up the record.
    for ( int i = 0 ; i < 4 ; i++ )
    {
        record.record += '?';
        record.record += rec;
    }

    auto runs = record.runs;
    record.runs.reserve( 5*runs.size() );
    for ( int i = 0 ; i < 4 ; i++ )
    {
        record.runs.insert( record.runs.end(), runs.begin(), runs.end() );
    }
}

int main( int argc, char *argv[] )
{
    // Make sure we have an argument
    if ( argc != 2 ) return 1;

    vector<SpringRecord> records;
    if ( !tryParseFileContents( argv[1], records ) ) return 1;

    uint64_t sum = 0;
    for ( auto& record : records )
    {
        expandRecord( record );
        sum += getArrangementCount( record );
    }

    printf( "%llu\n", sum );
    return 0;
}
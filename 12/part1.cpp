// Solution to part 1 of https://adventofcode.com/2023/day/12
// Mark Brudnak
// Dec 18, 2023

#include <cassert>

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
int getArrangementCountForPermutation( SpringRecord& record, int permute )
{
    auto& rec = record.record;
    auto& runs = record.runs;
    int runLen = 0;

    int runIdx = 0;
    int wcMask = 1;
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

// Gets the number of valid arrangements for all permutations of the '?'s
int getArrangementCount( SpringRecord& record )
{
    auto& rec = record.record;
    int Nq = count( rec.begin(), rec.end(), '?' );
    int Npermute = 1 << Nq;
    
    int arrCount = 0;
    for (int permute = 0 ; permute < Npermute ; permute++ )
    {
        arrCount += getArrangementCountForPermutation( record, permute );
    }
    return arrCount;
}

int main( int argc, char *argv[] )
{
    // Make sure we have an argument
    if ( argc != 2 ) return 1;

    vector<SpringRecord> records;
    if ( !tryParseFileContents( argv[1], records ) ) return 1;

    int sum = 0;
    for ( auto& record : records )
    {
        sum += getArrangementCount( record );
    }

    printf( "%d\n", sum );
    return 0;
}
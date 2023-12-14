// Solution to part 2 of https://adventofcode.com/2023/day/8
// Mark Brudnak
// Dec 12, 2023

import std;
import std.compat;
using namespace std;

bool tryParseFileContents( char *fileName, vector<int>& outInstructions, map<string, vector<string> >& outNodes )
{
    try
    {
        // Open the file for reading
        ifstream file( fileName );
        if ( !file.is_open() ) return false;

        string line;
        getline( file, line );
        int N = line.size();
        outInstructions.resize( N );
        for ( int i = 0 ; i < N ; i++ ) outInstructions[i] = line[i] == 'L' ? 0 : 1;

        getline( file, line ) ; // Blank line

          while ( getline( file, line ) )
        {
            auto node = line.substr( 0, 3 );
            auto left = line.substr( 7, 3 );
            auto right = line.substr( 12, 3 );
            vector<string> elements(2);
            elements[0] = left;
            elements[1] = right;
            outNodes[ node ] = elements;
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

    vector<int> instructions;
    map< string, vector<string> > mapNodes;
    if ( !tryParseFileContents( argv[1], instructions, mapNodes ) ) return 1;

    int Ni = instructions.size();

    // Determine the starting point of all of our threads
    vector<string> threadNodes;
    for ( auto& item : mapNodes )
    {
        if ( item.first[2] == 'A' ) threadNodes.push_back( item.first );
    }
    int Nt = threadNodes.size();

    // Through external experimentation I determined that each cycle is independent and fixed in length.
    // The cycles furthermore do not reutn to the start node.
    // Find the length of each cycle.
    vector<uint64_t> cycleLengths( threadNodes.size() );
    for ( int it = 0 ; it < Nt ; it++ )
    {
        auto node = threadNodes[ it ];
        int steps = 0;
        int i = 0;
        while ( node[2] != 'Z' )
        {
            node = mapNodes[ node][ instructions[ i ] ];
            i = (i+1) % Ni;
            steps++;
        }
        cycleLengths[it] = steps;
    }

    // Get the LCM of all of the values in cycleLengths.
    uint64_t lcc = 1;
    for ( auto v : cycleLengths )
    {
        lcc = std::lcm( lcc, v );
    }

    printf( "%llu\n", lcc );
    return 0;
}
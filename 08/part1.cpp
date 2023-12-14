// Solution to part 1 of https://adventofcode.com/2023/day/8
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
    map< string, vector<string> > nodes;
    if ( !tryParseFileContents( argv[1], instructions, nodes ) ) return 1;

    int Ni = instructions.size();
    int steps = 0;
    int i = 0;
    string node = "AAA";
    while ( node != "ZZZ" )
    {
        node = nodes[ node][ instructions[ i ] ];
        i = (i+1) % Ni;
        steps++;
    }

    printf( "%d\n", steps );
    return 0;
}
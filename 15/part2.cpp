// Solution to part 2 of https://adventofcode.com/2023/day/15
// Mark Brudnak
// Dec 26, 2023

#include <cassert>

import std;
import std.compat;
using namespace std;

bool tryParseFileContents( char *fileName, string& outLine )
{
    try
    {
        // Open the file for reading
        ifstream file( fileName );
        if ( !file.is_open() ) return false;

        getline( file, outLine );
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
        return false;
    }
    return true;
}

struct Lens
{
    string label;
    int focalLen;

    Lens(): label(""), focalLen(0) {}
    Lens( string l, int f) : label(l), focalLen(f) {}

    bool operator==( const Lens& RHS ) const
    {
        return label == RHS.label;
    }
};

int getHash( string label )
{
    int hash = 0;
    for ( auto c : label )
    {
        hash += c;
        hash *= 17;
        hash &= 0xff;
    }
    return hash;
}

int main( int argc, char *argv[] )
{
    // Make sure we have an argument
    if ( argc != 2 ) return 1;

    string sequence;
    if ( !tryParseFileContents( argv[1], sequence ) ) return 1;
    int N = sequence.size();

    regex lensPattern(R"(([a-z]+)(-|=\d))");

    const int N_BOXES = 256;
    vector<list<Lens>> boxes( N_BOXES );

    for (
        auto lens = sregex_iterator( sequence.begin(), sequence.end(), lensPattern) ;
        lens != sregex_iterator() ;
        ++lens
    )
    {
        smatch match = *lens;
        string label( match[1] );
        string operation( match[2] );
        auto hash = getHash( label );
        auto& box = boxes[hash];

        switch (operation[0])
        {
        case '=':
            {
                auto focalLen = operation[1] - '0';
                auto myLens = find( box.begin(), box.end(), Lens( label, 0 ) );
                if ( myLens == box.end() )
                {
                    box.emplace_back( label, focalLen );
                } else
                {
                    myLens->focalLen = focalLen;
                }
            } break;
        case '-':
            {
                while ( true )
                {
                    auto myLens = find( box.begin(), box.end(), Lens( label, 0 ) );
                    if ( myLens == box.end() ) break;
                    box.remove( *myLens );
                }
            } break;
        }
    }

    int sum = 0;
    for ( int i = 0 ; i < N_BOXES ; i++ )
    {
        int j = 1;
        for ( auto& slot : boxes[ i ] )
        {
            sum += ( i + 1 ) * j++ * slot.focalLen;
        }
    }
    printf( "%d\n", sum );
}
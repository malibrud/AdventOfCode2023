// Solution to part 1 of https://adventofcode.com/2023/day/5
// Mark Brudnak
// Dec 5, 2023

#include <fstream>
#include <string>
#include <vector>
#include <regex>
#include <algorithm>
#include <inttypes.h>
using namespace std;

struct Map
{
    uint64_t s0;
    uint64_t d0;
    uint64_t len;
};

int main( int argc, char *argv[] )
{
    // Make sure we have an argument
    if ( argc != 2 ) return 1;

    // Open the file for reading
    ifstream file( argv[1]);
    if ( !file.is_open() ) return 1;
    
    regex numPat(R"((\d+))");

    string line;
    vector<uint64_t> seeds;

    // Get the seeds
    getline( file, line );
    for (
        auto it = sregex_iterator( line.begin(), line.end(), numPat ) ;
        it != sregex_iterator() ;
        it++
    ){
        auto match = *it;
        uint64_t seed = stoull( match[1] );
        seeds.push_back( seed );
    }

    // iterate through all of the maps in sequence.
    getline( file, line );
    while ( !file.eof() )
    {
        getline( file, line ); // map line

        // Get the next map
        vector<Map> maps;
        while ( !file.eof() && ( getline( file, line ), line != "" ) )
        {
            Map m;
            sscanf( line.c_str(), "%llu %llu %llu", &m.d0, &m.s0, &m.len );
            maps.push_back(m);
        }

        // Map the seeds through
        for ( auto& seed : seeds )
        {
            for ( auto& map : maps )
            {
                if ( seed >= map.s0 && seed < map.s0 + map.len )
                {
                    seed = seed - map.s0 + map.d0;
                    break;
                }
            }
        }
    }

    uint64_t val = *std::ranges::min_element( seeds );

    printf( "%llu\n", val );
    return 0;
}
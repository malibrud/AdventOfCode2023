// Solution to part 2 of https://adventofcode.com/2023/day/5
// Mark Brudnak
// Dec 5, 2023

#include <fstream>
#include <string>
#include <vector>
#include <regex>
#include <algorithm>
#include <inttypes.h>
using namespace std;

struct SeedRange
{
    int64_t s0;
    int64_t s1;
    SeedRange( int64_t s0, int64_t s1 ) : s0(s0), s1(s1) {}
    bool operator <( SeedRange& lval) { return s0 < lval.s0; }
};

struct Map
{
    int64_t s0;
    int64_t s1;
    int64_t d0;
    int64_t d1;
    bool operator <( Map& lval) { return s0 < lval.s0; }
};

int main( int argc, char *argv[] )
{
    // Make sure we have an argument
    if ( argc != 2 ) return 1;

    // Open the file for reading
    ifstream file( argv[1]);
    if ( !file.is_open() ) return 1;
    
    regex numPat(R"((\d+)\s(\d+))");

    string line;
    vector<SeedRange> seeds;

    // Get the seed ranges
    getline( file, line );
    for (
        auto it = sregex_iterator( line.begin(), line.end(), numPat ) ;
        it != sregex_iterator() ;
        it++
    ){
        auto match = *it;
        auto s0 = stoull( match[1] );
        auto len = stoull( match[2] );
        seeds.emplace_back( s0, s0 + len );
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
            int64_t len;
            sscanf( line.c_str(), "%llu %llu %llu", &m.d0, &m.s0, &len );
            m.s1 = m.s0 + len;
            m.d1 = m.d0 + len;
            maps.push_back(m);
        }

        // Sort the maps so that we can keep track of ranges of seeds 
        // which may overlap with two or more maps.
        sort( maps.begin(), maps.end() );

        // Map the seeds through
        vector<SeedRange> images;
        while( !seeds.empty() )
        {
            auto seed = seeds.back();
            seeds.pop_back();
            for ( auto& map : maps )
            {
                // Check if there is anything to map.
                if ( seed.s1 == seed.s0 ) break;

                // Check if map is prior to seed range, then there is nothing to do for this map... keep looking.
                if ( map.s1 <= seed.s0 ) continue;

                // Check if this map and all subsequent maps are beyond seed range.... we are done.
                if ( map.s0 >= seed.s1 ) break; 

                // Get overlaping region
                auto s0 = max( seed.s0, map.s0 );
                auto s1 = min( seed.s1, map.s1 );

                // Check for seed region before map
                if ( seed.s0 < s0 ) 
                {
                    images.emplace_back( seed.s0, s0 );
                    seed.s0 = s1;
                }

                // Add the overlapping region.
                auto shift = map.d0 - map.s0;
                images.emplace_back( s0 + shift, s1 + shift );

                // If there is any remaining after the map, truncate the seed to after the map.
                seed.s0 = s1;
            }

            if ( seed.s0 < seed.s1 ) images.push_back( seed );
        }
        seeds = images;
    }

    sort( seeds.begin(), seeds.end() );
    printf( "%lld\n", seeds[0].s0 );
    return 0;
}
// Solution to part 1 of https://adventofcode.com/2023/day/2
// Mark Brudnak
// Dec 2, 2023

#include <fstream>
#include <string>
#include <regex>
#include <iterator>

using namespace std;

int main( int argc, char *argv[] )
{
    char num[3] = {};
    string line;
    ifstream file( argv[1]);
    if ( !file.is_open() ) return 1;

    regex pattern(R"((\d+) (red|green|blue))");

    int sum = 0;
    while ( getline( file, line ) )
    {
        int gameNo = atoi( &line[5] );

        for (
            auto it = sregex_iterator( line.begin(), line.end(), pattern ) ;
            it != sregex_iterator();
            it++
        ){
            auto match = *it;
            auto count = stoi( match[1] );
            auto color = match[2];
            if ( color == "red"   && count > 12 ) goto Next;
            if ( color == "green" && count > 13 ) goto Next;
            if ( color == "blue"  && count > 14 ) goto Next;
        }
        sum += gameNo; 
    Next:
        /*empty*/;
    }
    printf( "%d\n", sum );
    return 0;
}
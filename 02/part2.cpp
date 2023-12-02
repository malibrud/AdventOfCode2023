#include <fstream>
#include <string>
#include <regex>
#include <iterator>
#include <algorithm>

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
        auto rounds = line.substr( line.find(":")+1 );
        size_t start = 0;
        size_t end = rounds.find(';', start );
        int maxRed   = 0;
        int maxGreen = 0;
        int maxBlue  = 0;
        do
        {
            auto round = rounds.substr(start, end-start);
            for (
                auto it = sregex_iterator( round.begin(), round.end(), pattern ) ;
                it != sregex_iterator();
                it++
            ){
                auto match = *it;
                auto count = stoi( match[1] );
                auto color = match[2];
                if ( color == "red"   ) maxRed   = max( maxRed,   count );
                if ( color == "green" ) maxGreen = max( maxGreen, count );
                if ( color == "blue"  ) maxBlue  = max( maxBlue,  count );
            }

            start = end + 1;
            end = rounds.find(';', start );
             if ( end == string::npos ) end = rounds.size();
        } while ( start < rounds.size() );
        int power = maxRed * maxGreen * maxBlue;
        sum += power;
    }
    printf( "%d\n", sum );
    return 0;
}
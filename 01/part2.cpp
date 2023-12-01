#include <fstream>
#include <string>
#include <map>

using namespace std;


map<string, int> numbers =
{
    {"1", 1},
    {"2", 2},
    {"3", 3},
    {"4", 4},
    {"5", 5},
    {"6", 6},
    {"7", 7},
    {"8", 8},
    {"9", 9},
    {"one", 1},
    {"two", 2},
    {"three", 3},
    {"four", 4},
    {"five", 5},
    {"six", 6},
    {"seven", 7},
    {"eight", 8},
    {"nine", 9}
};

int main( int argc, char *argv[] )
{
    char num[3] = {};
    string line;
    ifstream file( argv[1]);
    if ( !file.is_open() ) return 1;

    int sum = 0;
    while ( getline( file, line ) )
    {
        int tensDigit;
        int onesDigit;

        // Find the first digit
        auto firstPos = line.size();
        for ( auto& number: numbers)
        {
            auto pos = line.find( number.first );
            if (pos == string::npos ) continue;
            if ( pos < firstPos ) 
            {
                firstPos = pos;
                tensDigit = number.second ;
            }
        }
        // Find the last digit
        auto lastPos = 0;
        for ( auto& number: numbers)
        {
            auto pos = line.rfind( number.first );
            if (pos == string::npos ) continue;
            if ( pos >= lastPos )
            {
                lastPos = pos;
                onesDigit = number.second;
            }
        }

        int val = tensDigit * 10 + onesDigit;
        // printf( "%d - %s\n", val, line.c_str() );
        sum += val;
    }
    printf( "%d\n", sum );
    return 0;
}
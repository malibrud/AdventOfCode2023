// Solution to part 1 of https://adventofcode.com/2023/day/1
// Mark Brudnak
// Dec 1, 2023

#include <fstream>
#include <string>

using namespace std;

int main( int argc, char *argv[] )
{
    char num[3] = {};
    string line;
    ifstream file( argv[1]);
    if ( !file.is_open() ) return 1;

    int sum = 0;
    while ( getline( file, line ) )
    {
        auto firstPos = find_if(line.begin(), line.end(), isdigit );
        auto lastPos = find_if(line.rbegin(), line.rend(), isdigit );
        num[0] = *firstPos;
        num[1] = *lastPos;
        num[2] = '\0';

        int val = atoi(num);
        sum += val;
    }
    printf( "%d\n", sum );
    return 0;
}
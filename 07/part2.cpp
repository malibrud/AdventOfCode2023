// Solution to part 1 of https://adventofcode.com/2023/day/7
// Mark Brudnak
// Dec 10, 2023

#include <limits.h>
#include <cassert>

import std;
import std.compat;
using namespace std;

enum Strength
{
    INVALID_STRENGTH = -1,
    HIGH_CARD = 0,
    ONE_PAIR,
    TWO_PAIR,
    THREE_OF_A_KIND,
    FULL_HOUSE,
    FOUR_OF_A_KIND,
    FIVE_OF_A_KIND
};

int cardToIndex( char card )
{
    if ( card <= '9' ) return card - '1';
    switch ( card )
    {
    case 'T': return 9;
    case 'J': return 0;
    case 'Q': return 10;
    case 'K': return 11;
    case 'A': return 12;
    default: assert( false );
    }
    return INT_MAX;
}

Strength cardsToStrength( string& cards )
{
    static vector<int> counts( 13, 0 );
    counts.assign( 13, 0 );

    for ( auto c : cards ) counts[ cardToIndex( c ) ]++;
    int jokerIdx = cardToIndex( 'J' );
    int jokers = counts[ jokerIdx ];
    counts[ jokerIdx ] = 0;

    sort( counts.begin(), counts.end(), greater<int>() );

    switch ( counts[0] )
    {
        case 0: return FIVE_OF_A_KIND;
        case 1: 
            if ( jokers == 1 ) return ONE_PAIR;
            if ( jokers == 2 ) return THREE_OF_A_KIND;
            if ( jokers == 3 ) return FOUR_OF_A_KIND;
            if ( jokers == 4 ) return FIVE_OF_A_KIND;
            return HIGH_CARD;
        case 2: 
            if ( jokers == 3 ) return FIVE_OF_A_KIND;
            if ( counts[1] == 1 ) 
            {
                if ( jokers == 1 ) return THREE_OF_A_KIND;
                if ( jokers == 2 ) return FOUR_OF_A_KIND;
                return ONE_PAIR;
            }
            if ( counts[1] == 2 )
            {
                if ( jokers == 1 ) return FULL_HOUSE;
                return TWO_PAIR;
            }
        case 3: 
            if ( jokers == 2 ) return FIVE_OF_A_KIND;
            if ( counts[1] == 1 ) 
            {
                if ( jokers == 1 ) return FOUR_OF_A_KIND;
                return THREE_OF_A_KIND;
            }
            if ( counts[1] == 2 ) return FULL_HOUSE;
        case 4: 
            if ( jokers == 1 ) return FIVE_OF_A_KIND;
            return FOUR_OF_A_KIND;
        case 5: return FIVE_OF_A_KIND;
        default: assert( false );
    }
    return INVALID_STRENGTH;
}

int cardsToValue( string& cards )
{
    int value = 0;
    for ( auto c : cards ) value = value * 16 + cardToIndex( c );
    return value;
}

struct Hand
{
    Strength stren;
    string cards;
    int value;

    Hand( string& cards ) 
        : cards(cards) 
        , stren( cardsToStrength( cards ) ) 
        , value( cardsToValue( cards ) ) 
    {}
};

struct Game
{
    Hand hand;
    int bid;
    Game ( string& cards, int bid ) : hand( cards ), bid( bid ) {}
    bool operator < ( Game& lhs )
    {
        auto compare = hand.stren <=> lhs.hand.stren;
        if ( compare == 0 ) return hand.value < lhs.hand.value;
        return compare < 0;
    }
};

bool tryParseFileContents( char *fileName, vector<Game>& outGames )
{
    try
    {
        // Open the file for reading
        ifstream file( fileName );
        if ( !file.is_open() ) return false;

        string line;
        int i = 0;
        while ( getline( file, line ) )
        {
            auto cards = line.substr( 0, 5 );
            auto bid = stoi( line.substr( 6 ) );
            outGames.emplace_back( cards, bid );
            // printf( "%s %d\n", cards.c_str(), outGames[i++].hand.stren );
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

    vector<Game> games;
    if ( !tryParseFileContents( argv[1], games ) ) return 1;

    sort( games.begin(), games.end() );

    uint64_t rank = 1;
    uint64_t score = 0;
    for ( auto& g : games) score += g.bid * rank++;

    printf( "%llu\n", score );
    return 0;
}
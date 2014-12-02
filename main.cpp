#include <string>
#include <vector>
#include <stdexcept>
#include <iostream>
#include <cassert>
void process(const std::string & program)
{
    std::vector< char > array { 0 };
    size_t ptr = 0;
    auto it = program.begin( );
    while ( it != program.end( ) )
    {
        switch ( * it )
        {
            case '>':
                ++ptr;
                if ( ptr >= array.size( ) ) { array.resize( array.size( ) * 2, 0 ); }
                break;
            case '<':
                assert( ptr > 0 );
                --ptr;
                break;
            case '+':
                ++array[ptr];
                break;
            case '-':
                --array[ptr];
                break;
            case ',':
                array[ptr] = std::getchar( );
                break;
            case '.':
                std::cout << array[ptr];
                break;
            case '[':
                {
                    size_t depth = 1;
                    while ( depth != 0 )
                    {
                        ++it;
                        if ( * it == '[' ) { ++depth; }
                        else if ( * it == ']' ) { --depth; }
                    }
                    --it;
                }
                break;
            case ']':
                if ( array[ptr] != 0 )
                {
                    size_t depth = 1;
                    while ( depth != 0 )
                    {
                        assert( it != program.begin( ) );
                        --it;
                        if ( * it == '[' ) { --depth; }
                        else if ( * it == ']' ) { ++depth; }
                    }
                }
                break;
        }
        ++it;
    }
}

int main( )
{
    process( "++++++++++[>+++++++>++++++++++>+++>+<<<<-]>++.>+.+++++++..+++.>++.<<+++++++++++++++.>.+++.------.--------.>+.>." );
}

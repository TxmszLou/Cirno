#include <string>
#include <iostream>
#include <../algebraic_data_type/algebraic_data_type.hpp>
using namespace algebraic_data_type;
typedef algebraic_data_type< std::string, std::tuple< recursive_indicator, recursive_indicator > > expr;
DECLARE_CONSTRUCTOR( expr, 0, print, x );
DECLARE_CONSTRUCTOR( expr, 1, seq, x );
void execute(const expr & s)
{
    s.match< print< arg >, seq< arg, arg > >( common::make_expansion(
        []( const std::string & str ){ std::cout << str; },
        []( const expr & l, const expr & r ){ execute( l ); execute( r ); } ) );
}
int main( ) { execute( print< >( )( "hello world" ) ); }

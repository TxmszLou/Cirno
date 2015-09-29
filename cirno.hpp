#ifndef CIRNO_HPP
#define CIRNO_HPP
#include <string>
#include <iostream>
#include <../algebraic_data_type/algebraic_data_type.hpp>
using namespace algebraic_data_type;
typedef algebraic_data_type< std::string, std::tuple< recursive_indicator, recursive_indicator >, unit, unit > expr;
DECLARE_CONSTRUCTOR( expr, 0, print, x )
DECLARE_CONSTRUCTOR( expr, 1, seq, x )
DECLARE_CONSTRUCTOR( expr, 2, UNIT, x )
DECLARE_CONSTRUCTOR( expr, 3, True, x )
expr execute(const expr & s)
{
    return
        s.match(
            with( print( arg ), []( const std::string & str ) { std::cout << str; return UNIT( ); } ),
            with( seq( arg, arg ), []( const expr & l, const expr & r ) { execute( l ); return execute( r ); } ),
            with( UNIT( uim ), []( ){ return UNIT( ); } ),
            with( True( uim ), []( ){ return True( ); } ) );
}
#endif // CIRNO_HPP


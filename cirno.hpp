#ifndef CIRNO_HPP
#define CIRNO_HPP
#include <string>
#include <iostream>
#include <../algebraic_data_type/algebraic_data_type.hpp>
using namespace algebraic_data_type;
typedef
algebraic_data_type
<
    std::string,
    std::tuple< recursive_indicator, recursive_indicator >,
    unit,
    unit,
    unit,
    std::tuple< recursive_indicator, recursive_indicator, recursive_indicator >
> expr;
DECLARE_CONSTRUCTOR( expr, 0, Print, x )
DECLARE_CONSTRUCTOR( expr, 1, Seq, x )
DECLARE_CONSTRUCTOR( expr, 2, Unit, x )
DECLARE_CONSTRUCTOR( expr, 3, True, x )
DECLARE_CONSTRUCTOR( expr, 4, False, x )
DECLARE_CONSTRUCTOR( expr, 5, If, x )

bool value_to_bool( const expr & e )
{ return e.match( with( True( uim ), []( ){ return true; } ), with( False( uim ), []( ){ return false; } ) ); }

expr execute( const expr & e )
{
    return
        e.match(
            with( Print( arg ), []( const std::string & str ) { std::cout << str; return Unit( ); } ),
            with( Seq( arg, arg ), []( const expr & l, const expr & r ) { execute( l ); return execute( r ); } ),
            with( Unit( uim ), []( ){ return Unit( ); } ),
            with( True( uim ), []( ){ return True( ); } ),
            with( False( uim ), []( ){ return False( ); } ),
            with(
                If( arg, arg, arg ),
                []( const expr & i, const expr & t, const expr & e )
                { return value_to_bool( execute( i ) ) ? execute( t ) : execute( e ); } ) );
}
#endif // CIRNO_HPP


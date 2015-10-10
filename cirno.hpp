#ifndef CIRNO_HPP
#define CIRNO_HPP
#include <string>
#include <iostream>
#include <../algebraic_data_type/algebraic_data_type.hpp>
using namespace algebraic_data_type;
typedef recursive_indicator ri;

DECLARE_ADT( expr,
         ( (Unit),
           (True),
           (False),
           (Print,      ri),
           (Seq,        ri, ri),
           (If,         ri, ri, ri),
           (String,     std::string),
           (Show,       ri)
         ), X )

expr When( const expr & con, const expr & act ) { return If( con, act, Unit( ) ); }
expr Unless( const expr & con, const expr & act ) { return If( con, Unit( ), act ); }

bool value_to_bool( const expr & e )
{
    return e.match( with( True( uim ),  []( ){ return true; } ),
                    with( False( uim ), []( ){ return false; } ) );

}

std::string show( const expr & e )
{
    return e.match(
                with( Unit( uim ), []( ) { return "tt"; } ),
                with( True( uim ), []( ) { return "true"; } ),
                with( False( uim ), []( ) { return "false"; } ),
                with( String( arg ), []( const std::string & str ) { return str; } ) );
}

expr execute( const expr & e )
{
    return
        e.match(
            with( Print( arg ), []( const expr & exp ) { std::cout << show( execute( exp ) ); return Unit( ); } ),
            with( Seq( arg, arg ), []( const expr & l, const expr & r ) { execute( l ); return execute( r ); } ),
            with( Unit( uim ), []( ){ return Unit( ); } ),
            with( True( uim ), []( ){ return True( ); } ),
            with( False( uim ), []( ){ return False( ); } ),
            with(
                If( arg, arg, arg ),
                []( const expr & i, const expr & t, const expr & e )
                { return value_to_bool( execute( i ) ) ? execute( t ) : execute( e ); } ),
            with( String( arg ), []( const std::string & str ) { return String( str ); } ),
            with( Show( arg ), []( const expr & e ) { return String( show( execute( e ) ) ); } ) );
}
#endif // CIRNO_HPP


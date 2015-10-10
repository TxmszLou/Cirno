#ifndef CIRNO_HPP
#define CIRNO_HPP
#include <string>
#include <iostream>
#include <../algebraic_data_type/algebraic_data_type.hpp>
#include <map>
#include <utility>
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
           (Show,       ri),
           (Concat,     ri, ri),
           (Set,        ri, ri),
           (Get,        ri),
           (IsDefined,  ri)
         ), X )

typedef std::map< std::string, expr > symbol_table;

expr When( const expr & con, const expr & act ) { return If( con, act, Unit( ) ); }
expr Unless( const expr & con, const expr & act ) { return If( con, Unit( ), act ); }

bool value_to_bool( const expr & e )
{
    return e.match( with( True( uim ),  []( ){ return true; } ),
                    with( False( uim ), []( ){ return false; } ) );
}

std::string value_to_string( const expr & e ) { return e.match( with( String( arg ), []( const std::string & str ){ return str; } ) ); }

std::string show( const expr & e )
{
    return e.match(
                with( Unit( uim ), []( ) { return "tt"; } ),
                with( True( uim ), []( ) { return "true"; } ),
                with( False( uim ), []( ) { return "false"; } ),
                with( String( arg ), []( const std::string & str ) { return str; } ) );
}

expr execute( symbol_table & st, const expr & e )
{
    return
        e.match(
            with( Print( arg ), [&]( const expr & exp ) { std::cout << show( execute( st, exp ) ); return Unit( ); } ),
            with( Seq( arg, arg ), [&]( const expr & l, const expr & r ) { execute( st, l ); return execute( st ,r ); } ),
            with( Unit( uim ), []( ){ return Unit( ); } ),
            with( True( uim ), []( ){ return True( ); } ),
            with( False( uim ), []( ){ return False( ); } ),
            with(
                If( arg, arg, arg ),
                [&]( const expr & i, const expr & t, const expr & e )
                { return value_to_bool( execute( st, i ) ) ? execute( st, t ) : execute( st, e ); } ),
            with( String( arg ), []( const std::string & str ) { return String( str ); } ),
            with( Show( arg ), [&]( const expr & e ) { return String( show( execute( st, e ) ) ); } ),
            with(
                Concat( arg, arg ),
                [&]( const expr & l, const expr & r )
                { return String( value_to_string( execute( st, l ) ) + value_to_string( execute( st, r ) ) ); } ),
            with(
                IsDefined( arg ),
                [&]( const expr & exp ) { return st.count( value_to_string( execute( st, exp ) ) ) != 0 ? True( ) : False( ); } ),
            with( Get( arg ), [&]( const expr & e ) { return st.find( value_to_string( execute( st, e ) ) )->second; } ),
            with(
                Set( arg, arg ),
                [&]( const expr & l, const expr & r )
                {
                    auto it = st.insert( std::make_pair( value_to_string( execute( st, l ) ), r ) );
                    if ( ! it.second ) { it.first->second = r; }
                    return Unit( );
                } ) );
}

expr execute( std::tuple< symbol_table > st, const expr & e ) { return execute( std::get< 0 >( st ), e ); }
expr execute( const expr & e ) { return execute( std::make_tuple( symbol_table( ) ), e ); }

#endif // CIRNO_HPP

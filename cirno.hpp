#ifndef CIRNO_HPP
#define CIRNO_HPP
#include <string>
#include <iostream>
#include <../algebraic_data_type/algebraic_data_type.hpp>
#include <map>
#include <utility>
#include <experimental/optional>
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
           (IsDefined,  ri),
           (Define,     ri, ri),
           (Scope,      ri)
         ), X )

typedef std::map< std::string, expr > symbol_table;

struct environment
{
    symbol_table st;
    std::experimental::optional< std::reference_wrapper< environment > > parent;
    typedef std::experimental::optional< expr > value;
    value get( const std::string & str )
    {
        auto it = st.find( str );
        return it != st.end( ) ? it->second : ( parent ? parent->get( ).get( str ) : value{ } );
    }
    void define( const std::string & str, const expr & e )
    {
        auto res = st.insert( std::make_pair( str ,e ) );
        assert( res.second );
    }
    bool assign( const std::string & str, const expr & e )
    {
        auto it = st.find( str );
        return it != st.end( ) ? ( it->second = e, true ) : ( parent ? parent->get( ).assign( str, e ) : false );
    }
    void set( const std::string & str, const expr & e ) { if ( ! assign( str, e ) ) { st.insert( std::make_pair( str, e ) ); } }
};

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

expr execute( std::tuple< environment > st, const expr & e );
expr execute( environment & env, const expr & e )
{
    return
        e.match(
            with( Print( arg ), [&]( const expr & exp ) { std::cout << show( execute( env, exp ) ); return Unit( ); } ),
            with( Seq( arg, arg ), [&]( const expr & l, const expr & r ) { execute( env, l ); return execute( env,r ); } ),
            with( Unit( uim ), []( ){ return Unit( ); } ),
            with( True( uim ), []( ){ return True( ); } ),
            with( False( uim ), []( ){ return False( ); } ),
            with(
                If( arg, arg, arg ),
                [&]( const expr & i, const expr & t, const expr & e )
                { return value_to_bool( execute( env, i ) ) ? execute( env, t ) : execute( env, e ); } ),
            with( String( arg ), []( const std::string & str ) { return String( str ); } ),
            with( Show( arg ), [&]( const expr & e ) { return String( show( execute( env, e ) ) ); } ),
            with(
                Concat( arg, arg ),
                [&]( const expr & l, const expr & r )
                { return String( value_to_string( execute( env, l ) ) + value_to_string( execute( env, r ) ) ); } ),
            with(
                IsDefined( arg ),
                [&]( const expr & exp ) { return env.get( value_to_string( execute( env, exp ) ) ) ? True( ) : False( ); } ),
            with(
                Define( arg, arg ),
                [&]( const expr & l, const expr & r ) { env.define( value_to_string( execute( env, l ) ), r ); return Unit( ); } ),
            with( Get( arg ), [&]( const expr & e ) { return *env.get( value_to_string( execute( env, e ) ) ); } ),
            with(
                Set( arg, arg ),
                [&]( const expr & l, const expr & r )
                {
                    env.set( value_to_string( execute( env, l ) ), r );
                    return Unit( );
                } ),
            with( Scope( arg ), [&]( const expr & e ) { return execute( std::make_tuple( environment{ symbol_table{ }, { env } } ), e ); } ) );
}

expr execute(std::tuple< environment > st, const expr & e ) { return execute( std::get< 0 >( st ), e ); }
expr execute( const expr & e ) { return execute( std::make_tuple( environment{ } ), e ); }

#endif // CIRNO_HPP

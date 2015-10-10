#include <cirno.hpp>
#include <cassert>
int main( )
{
    execute( Seq(
        If( True( ), Print( String( "hello " ) ), Print( True( ) ) ),
        If( False( ), Print( Unit( ) ), Print( String( "world!\n" ) ) ) ) );
    execute( Seq(
        When( True( ), Print( String( "hello " ) ) ),
        Unless( False( ), Print( String( "world!\n" ) ) ) ) );
    execute( Print( Print( Seq( Print( True( ) ), Print( False( ) ) ) ) ) );
    execute( Print( Concat( String( "4" ), Show( True( ) ) ) ) );
    assert( value_to_bool( execute( IsDefined( String( "not defined" ) ) ) ) == false );
    execute(
        Seq( Set( String( "SomeVar" ), String( "12" ) ),
             Seq( Print( Get( String( "SomeVar" ) ) ),
                  Seq( Set( Concat( String( "S" ), String( "omeVar" ) ), String( "345" ) ),
                       Print( Get( Concat( String( "Some" ), String( "Var" ) ) ) ) ) ) ) );
}

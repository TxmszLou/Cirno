#include <cirno.hpp>
int main( )
{
    execute( Seq(
        If( True( ), Print( String( "hello " ) ), Print( True( ) ) ),
        If( False( ), Print( Unit( ) ), Print( String( "world!\n" ) ) ) ) );
    execute( Seq(
        When( True( ), Print( String( "hello " ) ) ),
        Unless( False( ), Print( String( "world!\n" ) ) ) ) );
    execute( Print( Print( Seq( Print( True( ) ), Print( False( ) ) ) ) ) );
}

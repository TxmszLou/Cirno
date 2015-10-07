#include <cirno.hpp>
int main( )
{
    execute( Seq(
        If( True( ), Print( String( "hello " ) ), Print( True( ) ) ),
        If( False( ), Print( Unit( ) ), Print( String( "world!" ) ) ) ) );
    execute( Print( Print( Seq( Print( True( ) ), Print( False( ) ) ) ) ) );
}

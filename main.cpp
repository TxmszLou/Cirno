#include <cirno.hpp>
int main( ) { execute( Seq( If( True( ), Print( "hello " ), Print( "Error" ) ), If( False( ), Print( "Error" ), Print( "world!" ) ) ) ); }

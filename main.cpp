#include <string>
#include <iostream>
#include <boost/variant.hpp>
void execute(const std::string & s) { std::cout << s << std::endl; }
int main( ) { execute( "hello world" ); }

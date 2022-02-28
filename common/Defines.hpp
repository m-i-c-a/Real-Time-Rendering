#ifndef DEFINES_HPP
#define DEFINES_HPP

#include <iostream>
#include <stdlib.h>
#include <string>

#define EXIT( msg ) do { \
    std::cerr << __FUNCTION__ << " : " << msg << '\n'; \
    exit(EXIT_FAILURE) ; \
} while( 0 ) \

#endif // DEFINES_HPP
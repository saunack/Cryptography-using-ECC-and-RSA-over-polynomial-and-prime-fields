#ifndef GF2Ecc
#define GF2Ecc
#include "GF2.h"
#include <iostream>
#include <gmpxx.h>
class G2E
{
	public:
		GF2 x, y;
		G2E operator*( G2E );
		bool operator==( G2E );
		bool operator!=( G2E );
		
		G2E inverse();
		static mpz_class get();		//output prime
		static void set(GF2, GF2);		//Input a, b
		
		G2E( GF2 , GF2 );
		G2E( GF2 );
		G2E();					//Create identity element
		G2E( std::istream& );
		G2E( const G2E& );
		
	protected:
		static GF2 a, b;
};

std::ostream& operator<<( std::ostream&, G2E);
std::istream& operator>>( std::istream&, G2E&);
#endif
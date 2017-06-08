#ifndef PECC
#define PECC
#include "PrimeField.h"
#include<iostream>
#include <gmpxx.h>
class PE
{
	public:
		PF x, y;
		PE operator*( PE A );
		bool operator==( PE A );
		bool operator!=( PE A );
		PE operator=( PE A );
		PE operator-( PE A );
		
		PE inverse();
		static mpz_class get();			//output prime
		static void set(PF, PF);		//Input a, b
		
		PE( PF , PF );
		PE( PF );				//Input only X
		PE();					//Create identity element
		PE( std::istream& );
		PE( const PE& );
		
	protected:
		static PF a, b;
};

std::ostream& operator<<( std::ostream&, PE);
std::istream& operator>>( std::istream&, PE&);
#endif
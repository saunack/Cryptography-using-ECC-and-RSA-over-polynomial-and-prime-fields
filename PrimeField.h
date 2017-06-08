#ifndef PrimeField_H
#define PrimeField_H
#include<iostream>
#include<gmpxx.h>
void swap( mpz_class&, mpz_class& );

class PF{
	public:
		mpz_class x;
		PF operator+( PF );
		PF operator-( PF );
		PF operator*( PF );
		PF operator*( mpz_class );
		PF operator/( PF );
		PF operator=( PF );
		bool operator==( PF );
		bool operator!=( PF );
		PF operator-();				//unary
		
		static mpz_class set(mpz_class);			//set Prime
		static mpz_class get();						//get Prime/order
		void input();
		static PF discreteLog(PF, PF);
		static PF exponentiation(PF, PF);
		PF inverse();
		
		PF(mpz_class);
		PF(std::istream &);
		PF();						//Create identity element (of *)
		PF( const PF& );
	protected:
		static mpz_class Prime;
};

std::istream& operator>> (std::istream& , PF&);
std::ostream& operator<< (std::ostream& , PF);
#endif
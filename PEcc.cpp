#include <iostream>
#include "PEcc.h"
#include "RSA.h"
#include "PrimeField.h"
#include <gmpxx.h>
PF PE::a(0), PE::b(0);

bool PE::operator==( PE A ){
	if( A.x == x && A.y == y )	return true;
	return false;
}

bool PE::operator!=( PE A ){
	if( A.x == x && A.y == y )	return false;
	return true;
}

PE PE::operator=( PE A ){
	x = A.x;
	y = A.y;
	return A;
}

PE PE::operator*( PE A ){								//Template won't work since both ECCs have different equations
	if( A == PE() ){	return *this;}
	if( *this == PE() )	return A;
	
	if( y + A.y == PF(0) && x == A.x )	return PE();
	
	PF slope;
	if( A == *this ){
		slope = ( PF(3) * x * x + a ) / ( PF(2) * y );
	}
	else{
		slope = ( y - A.y )/( x - A.x );
	
	}
	PE temp;
	temp.x = slope * slope - x - A.x;
	temp.y = - (slope * ( temp.x - x ) + y);
	return temp;
}

PE PE::inverse(){
	return PE( x, -y );
}

PE::PE( PF m, PF n ){
	if( n * n != m * m * m + a * m + b ){
		throw "Invalid inputs";
	}
	x = m;
	y = n;
}

PE::PE( PF m ){
	PF buff(1), sum = m * m * m + a * m + b;
	for( buff = PF(1); buff != PF(0); buff = buff + PF(1) )
		if( buff * buff == sum )	break;
	if( buff == PF(0) ){
		x = PF(0);
		y = PF(0);
	}
	else{
		x = m;
		y = buff;
	}
}

PE::PE( std::istream& is ){
	PF X(is), Y(is);
	x = X;
	y = Y;
}

PE::PE(){
	x = PF(0);
	y = PF(0);
};

PE::PE( const PE& A ){
	x = A.x;
	y = A.y;
}

mpz_class PE::get(){
	mpz_class Prime = PF::get();
	return Prime + 3 + 2 * sqrt(Prime);
}

void PE::set(PF A, PF B){
	a = A;
	b = B;
}

std::ostream& operator<<( std::ostream& os, PE A ){
	os << A.x << " " << A.y ;
	return os;
}

std::istream& operator>>( std::istream& is, PE& A){
	is >> A.x >> A.y;
	return is;
}
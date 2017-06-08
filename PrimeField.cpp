/*
Input value of Prime (static mpz_class of class PF) in main()
*/

#include <iostream>
#include <gmpxx.h>
#include <algorithm>
#include "PrimeField.h"

mpz_class PF::Prime = 0;						//input this value

PF PF::operator+( PF a ){
	PF buff;
	buff.x = ( this->x + a.x ) % Prime;
	return buff;
}

PF PF::operator-( PF a ){
	PF buff;
	buff.x = ( this->x - a.x ) % Prime;
	if(buff.x < 0)	buff.x = Prime + buff.x;
	return buff;
}

PF PF::operator-( ){
	PF buff;
	buff.x = Prime - x;
	return buff;
}

PF PF::operator*( PF a ){
	PF buff;
	buff.x = ( this->x * a.x ) % Prime;
	if( buff.x < 0 )	buff.x = Prime + buff.x;
	return buff;
}

PF PF::operator*( mpz_class a ){
	PF buff;
	buff.x = ( this->x * a ) % Prime;
	return buff;
}

PF PF::operator/( PF a ){
	PF buff;
	buff.x = ( this->x * a.inverse().x ) % Prime;
	return buff;
}

PF PF::operator=( PF a ){
	x = a.x;
	return a;
}

bool PF::operator==( PF a ){
	if( x == a.x )	return true;
	else			return false;
}
bool PF::operator!=( PF a ){
	if( x != a.x )	return true;
	else			return false;
}

mpz_class PF::set(mpz_class x){
	Prime = x;
	return Prime;
}

mpz_class PF::get(){
	return Prime;
}
	
void PF::input(){
	try{
		std::cin >> x;
		x = x % Prime;
	if( x == 0 )	throw "Bad input. Try again";
	}catch( const char* msg ){
		std::cerr << msg << std::endl;
	}
}
	
PF::PF(mpz_class s){
	x = s;
};

PF::PF(std::istream& c){
	c >> x;
};

PF::PF(){
	x = 1;
};

PF::PF( const PF& a ){
	x = a.x;
};

PF PF::inverse(){
	PF output;
	mpz_class buff = x, pbuff = Prime, coeff[2] = { 1 , 0 };
	while( buff != 1 ){
		coeff[1] = coeff[1] - (pbuff / buff) * coeff[0];
		pbuff = pbuff % buff;
		std::swap( pbuff, buff );
		std::swap( coeff[0], coeff[1]);
	}
	output.x = coeff[0] % Prime;
	if( output.x < 0 )	output.x = Prime + output.x;
	return output;
};

/*
PF PF::exponentiation(PF Base, PF Exponent){							//compute g^A (mod p) using binary representation of A
	mpz_class remainder, term = Base.x, buff = 1, exponent = Exponent.x, modulo = Prime;

	while( exponent != 0 ){
		remainder = exponent % 2;
		buff = buff * ( remainder != 0 ? term : 1 );				//implemented using STL in MEXP.cpp
		buff %= modulo;
		term = (term * term) % modulo;
		exponent /= 2;
	}
	return PF(buff);
}
*/
/*PF PF::discreteLog( PF base, PF number ){							//Straight-forward implementation of Discrete Logarithm. Baby-Step Giant-Step algorithm is implemented in BSGS.cpp
	PF buff = base * base;
	mpz_class count = 1;
	if( number == base )	return PF(1);
	while( buff != base ){
		count++;
		if( buff == number )	break;
		buff = buff * base;
	}
	if( buff == number )	return PF(count);
	return PF(0);
}*/

std::ostream& operator<< (std::ostream& os, PF X){
	os << X.x;
	return os;
}

std::istream& operator>> (std::istream& is, PF& X){
	is >> X.x;
	return is;
}
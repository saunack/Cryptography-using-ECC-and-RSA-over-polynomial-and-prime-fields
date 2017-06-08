#include "crypto.h"
#include <iostream>
#include <fstream>
#include <string>
#include "PrimeField.h"
#include "PEcc.h"
#include "Functions.h"
#include "GF2.h"
#include "GF2Ecc.h"
#include "RSA.h"
#include <gmpxx.h>

#define PrimeMin 10000
#define PrimeMax 100000
#define ECCMin 10
#define ECCMax 30

/*--------------- Prime Field ----------------*/

bool isProper( PF M ){
	if( M == PF() )	return 0;
	mpz_class x = PF::get() - 1, y = 2;
	while( x % 2 == 0 && x != 1){
		if( exponentiation( M, y ) == PF() )	return false;
		x = x / 2;
		y = y * 2;
	}
	if( exponentiation( M, 2 ) == PF() )	return false;
	if( exponentiation( M, 3 ) == PF() )	return false;
	return true;
}

void PrimeField_key( std::ostream& Y, mpz_class& priv, PF& g, PF& publ ){
	mpz_class p(0);
	g = PF();
	while( p == 0 )	p = gPrime(PrimeMin, PrimeMax);
	PF::set( p );
	priv = random(1, p - 2);
	while( !isProper(g) ) g = PF( random(1, p - 2) );
	publ = exponentiation( g, priv );
	
	Y << p << std::endl << g << std::endl << publ << std::endl;
}

mpz_class PrimeField_enc( std::istream& keyInp, std::istream& plaintxt, std::ostream& ciphertxt, PF& publ, mpz_class& priv, PF& share ){
	mpz_class p;
	PF g, pKey;
	keyInp >> p >> g >> pKey;
	//std::cout << "input : " << p << g.x << pKey << std::endl;
	priv = random(1, p - 2);
	share = exponentiation( pKey, priv );
	publ = exponentiation( g, priv );
	
	mpz_class c;
	ciphertxt << publ << std::endl;
	while( !plaintxt.eof() ){
		plaintxt >> c;
		ciphertxt << PF(c) * share << std::endl;
	}
	
	return ( PF(c) * share ).x;
}

void PrimeField_sign( std::ofstream& sign, mpz_class Hash, PF& gen, mpz_class& priv ){
	mpz_class k, p = PF::get();
	while( gcd(k, p - 1) != 1 )	k = random( 2, p - 2);
	PF r = exponentiation( gen, k );
	mpz_class s = ( Hash - priv * r.x ) * inverse( k, p - 1 );
	s = s % (p - 1);
	if( s < 0 )	s = p - 1 + s;
	sign << r << std::endl << s;
}

mpz_class PrimeField_dec( std::istream& in, std::ostream& out, mpz_class& privateKey, PF& generator, PF& pKey, PF& sharedKey ){
	PF inverse;
	in >> pKey;
	sharedKey = exponentiation( pKey, privateKey );
	inverse = sharedKey.inverse();
	//std::cout << "Inverse: " << inverse << std::endl;
	PF buff;
	while( !in.eof() ){
		in >> buff;
		out << (buff * inverse) << std::endl;
	}
	return buff.x;
}

void PrimeField_verify( std::istream& in, mpz_class H, PF g, PF& key ){
	PF r, s;
	in >> r >> s;
	PF LHS = exponentiation( g, H );
	PF RHS = exponentiation(key, r.x) * exponentiation(r, s.x);
	if(LHS == RHS) std::cout << "Verified" << std::endl;
	else	std::cout << "Not verified" << std::endl;
}

void primefield( std::string M, std::string& Y ){
	PF generator, publicKey[2], sharedKey[2];
	mpz_class privateKey[2];
	
	while( publicKey[0] == PF() ){										//key generation
		std::ofstream out(Y + ".key");
		PrimeField_key( out, privateKey[0], generator, publicKey[0] );
		out.close();
	}
	
	mpz_class Hash(0);
	while( sharedKey[1] == PF(0) || sharedKey[1] == PF() || Hash == 0){										//encryption
		std::ifstream X(M);
		std::ifstream in(Y + ".key");
		std::ofstream cipher( Y + ".enc" );
		Hash = PrimeField_enc( in, X, cipher, publicKey[1], privateKey[1], sharedKey[1] );
		in.close();
		X.close();
		cipher.close();
		//std::cout << "Shared Key: " << sharedKey << std::endl;
		//std::cout << privateKey[0] << std::endl << privateKey[1] << std::endl;
	}
	
	std::ofstream sign( Y + ".sign" );									//Signing
	PrimeField_sign( sign, Hash, generator, privateKey[1] );
	sign.close();
	
	std::ifstream fin( Y + ".enc" );									//decryption
	std::ofstream fot( Y + ".dec" );
	Hash = PrimeField_dec( fin, fot, privateKey[0], generator, publicKey[1], sharedKey[0] );
	fin.close();
	fot.close();
	
	std::ifstream fsi( Y + ".sign" );
	PrimeField_verify( fsi, Hash, generator, publicKey[1] );
	fsi.close();
}

/*------------ ECC over Prime Field -------------*/

bool isProper( PE M ){
	if( M == PE() )	return 0;
	mpz_class x = PE::get() - 1, y = 2;
	while( x % 2 == 0 && x != 1 && x >= y){
		if( exponentiation( M, y ) == PE() )	return false;
		x = x / 2;
		y = y * 2;
	}
	if( exponentiation( M, 2 ) == PE() )	return false;
	if( exponentiation( M, 3 ) == PE() )	return false;
	return true;
}

void PEcc_key( std::ostream& Y, mpz_class& priv, PE& g, PE& publ ){
	g = PE();
	mpz_class p(0);
	while( p == 0 )	p = gPrime(ECCMin, ECCMax);
	PF::set( p );
	PF a( random(1, p - 1)), b(random(1, p - 1));
	while( PF(4) * a * a * a + PF(27) * b * b == PF(0) ){
		a = PF( random(1, p - 1));
		b = PF( random(1, p - 1));
	}
	PE::set(a, b);
	priv = random(2, p - 2);
	
	while( !isProper(g) ){
		PF buff( random(2, p - 2 ));
		g = PE( buff );
	}
	//std::cout << "Gen: " << g << "\tP: " << p << "\ta, b: " << a << ", " << b << std::endl;
	publ = exponentiation( g, priv );
	
	Y << p << std::endl << g << std::endl << publ << std::endl;
}

mpz_class PEcc_enc( std::istream& keyInp, std::istream& plaintxt, std::ostream& ciphertxt, PE& publ, mpz_class& priv, PE& share ){
	mpz_class p;
	PE g, pKey;
	keyInp >> p >> g >> pKey;
	//std::cout << "input : " << p << g.x << pKey << std::endl;
	priv = random(1, p - 2);
	share = exponentiation( pKey, priv );
	publ = exponentiation( g, priv );
	
	mpz_class c;
	ciphertxt << publ << std::endl;
	while( !plaintxt.eof() ){
		plaintxt >> c;
		ciphertxt << (PF(c) * share.x).x << std::endl;
	}
	return (PF(c) * share.x).x;
}

void PEcc_sign( std::ofstream& sign, mpz_class Hash, PE gen, mpz_class& priv ){
	mpz_class p = discreteLog(PE(), gen), k(1);
	//std::cout << "Log: " << p << std::endl;
	while( gcd(k, p) != 1 || k == 1)	k = random(2, p - 1);
	//std::cout << "k: " << k << std::endl;
	PE r = exponentiation( gen, k );
	//std::cout << k << "  " << std::endl;
	mpz_class s = ( Hash + priv * r.x.x ) * inverse(k, p);
	s = s % (p);
	sign << r << std::endl << s;
}

mpz_class PEcc_dec( std::istream& in, std::ostream& out, mpz_class privateKey, PE& generator, PE& sharedKey ){
	PE pKey;
	in >> pKey;
	sharedKey = exponentiation( pKey, privateKey );
	PF inverse = sharedKey.x.inverse();
	//std::cout << "Inverse: " << inverse << std::endl;
	PF buff;
	while( !in.eof() ){
		in >> buff;
		out << (buff * inverse) << std::endl;
	}
	return buff.x;
}

void PEcc_verify( std::istream& in, mpz_class H, PE g, PE& key ){
	PE r;
	mpz_class s;
	in >> r >> s;
	PE LHS = exponentiation( r, s );
	PE RHS = exponentiation(key, r.x.x) * exponentiation(g, H);
	if(LHS == RHS) std::cout << "Verified" << std::endl;
	else	std::cout << "Not verified" << std::endl;
	//std::cout << LHS << std::endl << RHS << std::endl;
}

void pecc( std::string I, std::string& Y ){
	PE generator, publicKey[2], sharedKey[2];
	mpz_class privateKey[2];
	
	while( publicKey[0] == PE() ){
		
		std::ofstream out(Y + ".key");
		PEcc_key( out, privateKey[0], generator, publicKey[0] );
		out.close();
	}
	
	mpz_class Hash(0);
	while( sharedKey[1].x == PF(0) || Hash == 0 ){
		std::ifstream X(I);
		std::ifstream in(Y + ".key");
		std::ofstream cipher( Y + ".enc" );
		Hash = PEcc_enc( in, X, cipher, publicKey[1], privateKey[1], sharedKey[1] );
		in.close();
		X.close();
		cipher.close();
		//std::cout << "Shared Key: " << sharedKey[1] << std::endl;
		//std::cout << privateKey[0] << std::endl << privateKey[1] << std::endl;
	}
	
	std::ofstream sign( Y + ".sign" );									//Signing
	PEcc_sign( sign, Hash, generator, privateKey[1] );
	sign.close();
	
	std::ifstream fin( Y + ".enc" );
	std::ofstream fot( Y + ".dec" );
	Hash = PEcc_dec( fin, fot, privateKey[0], generator, sharedKey[0] );
	//std::cout << sharedKey[0];
	fin.close();
	fot.close();
	
	std::ifstream fsi( Y + ".sign" );
	PEcc_verify( fsi, Hash, generator, publicKey[1] );
	fsi.close();
}

/*-------------ECC over GF(2^m)-----------*/

bool isProper( G2E M ){
	if( M == G2E() )	return 0;
	mpz_class x = GF2::getPrime() - 1, y = 2;
	while( x % 2 == 0 && x != 1){
		//std::cout << M << " " << y << std::endl;
		if( exponentiation( M, y ) == G2E() )	return false;
		x = x / 2;
		y = y * 2;
	}
	if( exponentiation( M, 2 ) == G2E() )	return false;
	if( exponentiation( M, 3 ) == G2E() )	return false;
	return true;
}

void G2Ecc_key( std::ostream& Y, mpz_class& priv, G2E& g, G2E& publ ){
	g = G2E();
	mpz_class p(0), P, primePol[]={19, 25, 31, 47, 41, 59, 61, 55};
	p = primePol[ random(0,7).get_ui() ];
	GF2::set(p);
	P = GF2::get();
	mpz_class a = random(1, P - 1), b = random(1, P - 1);
	//std::cout << p << "\t" << a << "\t" << b << std::endl;
	G2E::set( GF2(a), GF2(b) );			//Set a, b
	priv = random(1, P - 1);
	//while( !isProper(g) )	
	while(g == G2E())	g = G2E( random(2, P - 1) );
	publ = exponentiation( g, priv );
	Y << p << std::endl << g << std::endl << publ << std::endl;
}

mpz_class G2Ecc_enc( std::istream& keyInp, std::istream& plaintxt, std::ostream& ciphertxt, G2E& publ, mpz_class& priv, G2E& share ){
	mpz_class p;
	G2E g, pKey;
	keyInp >> p >> g >> pKey;
	//std::cout << "input : " << p << "\t" << g << "\t" << pKey << std::endl;
	GF2::set(p);
	priv = random(2, GF2::get() - 2);
	share = exponentiation( pKey, priv );
	publ = exponentiation( g, priv );
	
	mpz_class c;
	ciphertxt << publ << std::endl;
	while( !plaintxt.eof() ){
		plaintxt >> c;
		ciphertxt << GF2(c) * share.x << std::endl;
	}
	return (GF2(c) * share.x).x;
}

void G2E_sign( std::ofstream& sign, mpz_class Hash, G2E gen, mpz_class priv ){
	mpz_class k(1), p = discreteLog(G2E(), gen);
	while( gcd(k, p - 1) != 1 )		k = random( 2, p - 1);
	G2E r = exponentiation( gen, k );
	mpz_class s = ( Hash + priv * r.x.x ) * inverse(k, p);
	s = s % p;
	sign << r << std::endl << s;
}

mpz_class G2Ecc_dec( std::istream& in, std::ostream& out, mpz_class& privateKey, G2E& generator, G2E& sharedKey ){
	G2E pKey;
	in >> pKey;
	sharedKey = exponentiation( pKey, privateKey );
	GF2 inverse = sharedKey.x.inverse();
	//std::cout << "Inverse: " << inverse << std::endl;
	GF2 buff;
	while( !in.eof() ){
		in >> buff;
		out << (buff * inverse) << std::endl;
	}
	return buff.x;
}

void G2E_verify( std::istream& in, mpz_class H, G2E g, G2E& key ){
	G2E r;
	mpz_class s;
	in >> r >> s;
	G2E LHS = exponentiation(r, s);
	G2E RHS = exponentiation(key, r.x.x) * exponentiation(g, H);
	if(LHS == RHS) std::cout << "Verified" << std::endl;
	else	std::cout << "Not verified" << std::endl;
}

void g2ecc( std::string M, std::string Y ){
	G2E generator, publicKey[2], sharedKey[2];
	mpz_class privateKey[2], Hash(0);
	
	while( publicKey[0] == GF2() || sharedKey[1].x == GF2(0) || sharedKey[1] == G2E() || Hash == 0 ){
		std::ofstream out(Y + ".key");
		G2Ecc_key( out, privateKey[0], generator, publicKey[0] );
		out.close();
	
		std::ifstream X(M);
		std::ifstream in(Y + ".key");
		std::ofstream cipher( Y + ".enc" );
		Hash = G2Ecc_enc( in, X, cipher, publicKey[1], privateKey[1], sharedKey[1] );
		in.close();
		X.close();
		cipher.close();
	}
	
	std::ofstream sign( Y + ".sign" );								//Signing
	G2E_sign( sign, Hash, generator, privateKey[1] );
	sign.close();
	
	std::ifstream fin( Y + ".enc" );
	std::ofstream fot( Y + ".dec" );
	G2Ecc_dec( fin, fot, privateKey[0], generator, sharedKey[0] );
	//std::cout << sharedKey[0];
	fin.close();
	fot.close();
	
	std::ifstream fsi( Y + ".sign" );
	G2E_verify( fsi, Hash, generator, publicKey[1] );
	fsi.close();
}

/*--------------------GF(2^m)-----------------*/
bool isProper( GF2 M ){
	if( M == GF2() )	return 0;
	mpz_class x = GF2::get() , y = 2;
	while( x % 2 == 0 && x != y ){
		if( exponentiation( M, y ) == GF2() )	return false;
		x = x / 2;
		y = y * 2;
	}
	if( exponentiation( M, 2 ) == GF2() )	return false;
	if( exponentiation( M, 3 ) == GF2() )	return false;
	return true;
}

void GF2_key( std::ostream& Y, mpz_class& priv, GF2& g, GF2& publ ){
	mpz_class p(0), P;
	while( p == 0 ){
		p = gPrime(10, 20);
		if( mpz_popcount(p.get_mpz_t()) % 2 == 0 )	p = 0;
	}
	GF2::set( p );
	P = GF2::get();
	priv = random(2, P - 1);
	while( !isProper(g) ) g = GF2( random(1, p - 2) );
	publ = exponentiation( g, priv );
	
	Y << p << std::endl << g << std::endl << publ << std::endl;
}

mpz_class GF2_enc( std::istream& keyInp, std::istream& plaintxt, std::ostream& ciphertxt, GF2& publ, mpz_class& priv, GF2& share ){
	mpz_class p;
	GF2 g, pKey;
	keyInp >> p >> g >> pKey;
	//std::cout << "input : " << p << g.x << pKey << std::endl;
	priv = random(2, p - 2);
	//std::cout << p << " " << g << " " << pKey << " " << priv << std::endl;
	share = exponentiation( pKey, priv );
	publ = exponentiation( g, priv );
	
	mpz_class c;
	ciphertxt << publ << std::endl;
	while( !plaintxt.eof() ){
		plaintxt >> c;
		ciphertxt << GF2(c) * share << std::endl;
	}
	
	return (GF2(c) * share).x;
}

void GF2_sign( std::ofstream& sign, mpz_class Hash, GF2& gen, mpz_class& priv ){
	mpz_class k, p = GF2::get();
	while( gcd(k, p - 1) != 1 )	k = random( 2, p - 2);
	GF2 r = exponentiation( gen, k );
	mpz_class s = ( Hash - priv * r.x ) * inverse( k, p - 1 );
	s = s % (p - 1);
	if( s < 0 )	s = p - 1 + s;
	sign << r << std::endl << s;
}

mpz_class GF2_dec( std::istream& in, std::ostream& out, mpz_class& privateKey, GF2& generator, GF2& pKey, GF2& sharedKey ){
	GF2 inverse;
	in >> pKey;
	sharedKey = exponentiation( pKey, privateKey );
	inverse = sharedKey.inverse();
	//std::cout << "Inverse: " << inverse << std::endl;
	GF2 buff;
	while( !in.eof() ){
		in >> buff;
		out << (buff * inverse) << std::endl;
	}
	return buff.x;
}

void GF2_verify( std::istream& in, mpz_class H, GF2 g, GF2& key ){
	GF2 r;
	mpz_class s;
	in >> r >> s;
	GF2 LHS = exponentiation( g, H );
	GF2 RHS = exponentiation(key, r.x) * exponentiation(r, s);
	if(LHS == RHS) std::cout << "Verified" << std::endl;
	else	std::cout << "Not verified" << std::endl;
}

void gf2( std::string M, std::string& Y ){
	GF2 generator, publicKey[2], sharedKey[2];
	mpz_class privateKey[2];
	mpz_class Hash(0);
	
	while( publicKey[0] == GF2() || publicKey[0] == GF2(0) || sharedKey[1] == GF2() || sharedKey[1] == GF2(0) || Hash == 0){
		std::ofstream out(Y + ".key");
		GF2_key( out, privateKey[0], generator, publicKey[0] );
		out.close();
	//}
	//while(  ){
		std::ifstream X(M);
		std::ifstream in(Y + ".key");
		std::ofstream cipher( Y + ".enc" );
		Hash = GF2_enc( in, X, cipher, publicKey[1], privateKey[1], sharedKey[1] );
		in.close();
		X.close();
		cipher.close();
		//std::cout << "Shared Key: " << sharedKey[1] << std::endl;
		//std::cout << privateKey[0] << std::endl << privateKey[1] << std::endl;
	}
	
	std::ofstream sign( Y + ".sign" );									//Signing
	GF2_sign( sign, Hash, generator, privateKey[1] );
	sign.close();
	
	std::ifstream fin( Y + ".enc" );
	std::ofstream fot( Y + ".dec" );
	Hash = GF2_dec( fin, fot, privateKey[0], generator, publicKey[0], sharedKey[0] );
	fin.close();
	fot.close();
	
	std::ifstream fsi( Y + ".sign" );
	GF2_verify( fsi, Hash, generator, publicKey[1] );
	fsi.close();
}

/*-------------RSA---------------*/

void RSA_key( std::ostream& Y, mpz_class priv[], PF& publ, PF shared[] ){
	mpz_class p(0);
	mpz_class q(0);
	while( p == 0 )	p = gPrime(10000, 200000);
	while( q == 0 || q == p )	q = gPrime(10000, 200000);
	mpz_class tot = (p - 1) * (q - 1);
	PF::set( tot );
	publ.x = p * q;
	shared[0].x = 2;
	while( gcd( shared[0].x, tot ) != 1 )    shared[0].x = shared[0].x + 1;
	priv[0] = inverse( shared[0].x , tot );
	shared[1].x = shared[0].x + 1;
	while( gcd( shared[1].x, tot ) != 1 )    shared[1].x = shared[1].x + 1;
	priv[1] = inverse( shared[1].x , tot );

	Y << publ << std::endl << shared[0] << std::endl << shared[1] << std::endl;
}

mpz_class RSA_enc( std::istream& keyInp, std::istream& plaintxt, std::ostream& ciphertxt ){
	mpz_class n;
	PF sharedKey;
	keyInp >> n >> sharedKey;
	PF::set(n);
	//std::cout << "input : " << p << g.x << pKey << std::endl;
    mpz_class c;
	while( !plaintxt.eof() ){
		plaintxt >> c;
		ciphertxt << exponentiation( PF(c) , sharedKey.x ) << std::endl;
	}
	return (exponentiation( PF(c), sharedKey.x)).x;
}

void RSA_sign( std::ofstream& sign, mpz_class Hash, mpz_class& priv ){
	PF s = exponentiation( PF(Hash), priv );
	sign << s ;
}

mpz_class RSA_dec( std::istream& in, std::ostream& out, mpz_class& privateKey ){
	PF buff;
	while( !in.eof() ){
		in >> buff;
		out << exponentiation (buff, privateKey) << std::endl;
	}
	return buff.x;
}

void RSA_verify( std::istream& in, std::istream& inKey, mpz_class H ){
	PF s;
	PF key;
	for(int i=0;i<3;i++)
        inKey >> key;                                   //3rd key is the corresponding sign for verifying the sign
	in >> s;
	PF LHS(H);
	PF RHS = exponentiation(s, key.x);
	if(LHS == RHS) std::cout << "Verified" << std::endl;
	else	std::cout << "Not verified" << std::endl;
};

void RSA(std::string M, std::string& Y ){
	PF publicKey, sharedKey[2];
	mpz_class privateKey[2];

	std::ofstream out(Y + ".key");                                              //Key generation
	RSA_key( out, privateKey, publicKey, sharedKey );
	out.close();

	mpz_class Hash;
	std::ifstream X(M);                                                         //Encryption
	std::ifstream in(Y + ".key");
	std::ofstream cipher( Y + ".enc" );
	Hash = RSA_enc( in, X, cipher );
	in.close();
	X.close();
	cipher.close();
	//std::cout << "Shared Key: " << sharedKey << std::endl;
	//std::cout << privateKey[0] << std::endl << privateKey[1] << std::endl;


	std::ofstream sign( Y + ".sign" );									    //Signing
	RSA_sign( sign, Hash, privateKey[1] );
	sign.close();

	std::ifstream fin( Y + ".enc" );									//decryption
	std::ofstream fot( Y + ".dec" );
	Hash = RSA_dec( fin, fot, privateKey[0] );
	fin.close();
	fot.close();

	std::ifstream fsi( Y + ".sign" );									//verification
	std::ifstream xin(Y + ".key");
	RSA_verify( fsi, xin, Hash );
	fsi.close();
}
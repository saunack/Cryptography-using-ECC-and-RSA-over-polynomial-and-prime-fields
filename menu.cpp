#include "crypt.h"
#include <iostream>
#include <fstream>
#include <string>
#include "PrimeField.h"
#include "Functions.h"
#include "GF2.h"
#include "GF2Ecc.h"
#include "PEcc.h"
#include "RSA.h"
#include <gmpxx.h>

void Main_menu();

void Binary_field(){
	std::cout<<"Do you wish to: 1. enter a prime polynomial or 2. use a random one?\n";
	int in;
	std::cin>>in;
	if(in == 1){
		std::cout<<"input a prime polynomial\n";
		int prime;
		std::cin>>prime;
		GF2::set(prime);
	}
	else if (in ==2){
		mpz_class p(0), P, primePol[]={19, 25, 31, 47, 41, 59, 61, 55};
		p = primePol[ random(0,7).get_ui() ];
		GF2::set( p );
		std::cout << "value of prime is " << p << "\n";
	}
	else	Main_menu();
	std::cout<<"enter 2 elements\n";
	GF2 a,b;
	std::cin >> a >> b;
	std::cout << "\n\na + b = " << a + b
		<< "\na * b = " << a * b
		<< "\nInverse of a : " << a.inverse()
		<< "\nInverse of b : " << b.inverse()
		<< "\na / b = " << a/b;
	std::cout<<"\n\nDo you want to perform exponentiation?(y/n)\t";
	char ch;
	std::cin >> ch;
	if( ch == 'y' || ch == 'Y'){
		mpz_class k;
		std::cout << "\nenter exponenent\n";
		std::cin >> k;
		std::cout << "\nA^k = "<<exponentiation( a, k) ;
		std::cout << "\nB^k = "<<exponentiation( b, k) ;
	}
	std::cout << "\nDo you wish to calculate discrete log of a with base b? (y/n)";
	std::cin >> ch;
	if( ch == 'y' || ch == 'Y'){
		if( discreteLog(a,b) == 0)
			std::cout << "\nLog does not exist\n";
		else
			std::cout << std::endl << discreteLog(a,b) <<std::endl;
	}
	Main_menu();
}

void Prime_field(){
	std::cout<<"Do you wish to: 1. enter a prime number or 2. use a random prime?\n";
	int in;
	std::cin>>in;
	if(in == 1){
		std::cout<<"input a prime\n";
		int prime;
		std::cin>>prime;
		PF::set(prime);
	}
	else if (in ==2){
		mpz_class p(0);
		while( p == 0 )	p = gPrime(10000, 200000);
		PF::set(p);
		std::cout << "value of prime is " << p << "\n";
	}
	else	Main_menu();
	std::cout<<"enter 2 elements of this field\n";
	PF a,b;
	std::cin >> a >> b;
	std::cout<<"\n\na + b = "<<a + b
		<<"\na - b = "<<a - b
		<<"\na * b = "<<a * b
		<<"\nInverse of a : "<<a.inverse()
		<<"\nInverse of b : "<<b.inverse()
		<<"\na / b = "<<a / b
		<<"\na ^ b = "<<exponentiation(a , b.x);
	std::cout<<"\n\nDo you wish to perform exponentiation?(y/n)\t";
	char ch;
	std::cin >> ch;
	if( ch == 'y' || ch == 'Y'){
		mpz_class k;
		std::cout << "\nenter exponenent\n";
		std::cin >> k;
		std::cout << "\nA^k = "<<exponentiation( a, k) ;
		std::cout << "\nB^k = "<<exponentiation( b, k) ;
	}
	std::cout << "\nDo you wish to calculate discrete log of a with base b? (y/n)";
	std::cin >> ch;
	if( ch == 'y' || ch == 'Y'){
		if( discreteLog(a,b) == 0)
			std::cout << "\nLog does not exist\n";
		else
			std::cout << std::endl << discreteLog(a,b) <<std::endl;
	}
	Main_menu();
}

void ECC_prime(){									//ECC prime input of a, b and points or computer-generated?
	std::cout<<"Do you wish to: 1. enter prime, a, b and the points on the ECC or 2. use a random ECC?\n";
	int in;
	std::cin>>in;
	PE A, B;
	if(in == 1){
		std::cout << "enter the prime\n";
		mpz_class prime,a, b;
		std::cin >> prime;
		PF::set(prime);
		std::cout << "input a and b\n";
		std::cin >> a >> b;
		PE::set(a, b);
		std::cout<<"enter 2 points (separated by a white space or return carriage)\n";
		std::cin>>A.x >> A.y >> B.x >> B.y;
	}
	else if (in ==2){
		mpz_class p(0);
		while( p == 0 )	p = gPrime(1000, 20000);
		PF::set(p);
		PF a( random(1, p - 1)), b(random(1, p - 1));
		while( PF(4) * a * a * a + PF(27) * b * b == PF(0) ){
			a = PF( random(1, p - 1));
			b = PF( random(1, p - 1));
		}
		PE::set(a, b);
		while(A == PE())	A = PE(PF(random(1,p-1)));
		while(B == PE())	B = PE(PF(random(1,p-1)));
		//if( A.y * A.y != A.x * A.x * A.x + a * A.x + b )	std::cout <<"WQ";
		//if( B.y * B.y != B.x * B.x * B.x + a * B.x + b )	std::cout <<"WQa";
		std::cout << "Prime : " << p << std::endl;
		std::cout << "a : "<< a << "\nb : " << b << std::endl; 
		std::cout << "A : "<< A << "\nB : " << B << std::endl; 
	}
	else	Main_menu();
	PE C = A * B;
	std::cout<<"\nA + B = "<<C;
	std::cout<<"\nInverse of A : "<<A.inverse();
	std::cout<<"\nInverse of B : "<<B.inverse();
	std::cout<<"\n\nDo you want to do scalar multiplication?(y/n)\t";
	char ch;
	std::cin >> ch;
	if( ch == 'y' || ch == 'Y'){
		mpz_class k;
		std::cout << "\nenter scalar\n";
		std::cin >> k;
		std::cout << "\nkA = "<< exponentiation(A, k);
		std::cout << "\nkB = "<< exponentiation(B, k);
	}
	std::cout << "\nDo you wish to calculate discrete log of a with base b? (y/n)";
	std::cin >> ch;
	if( ch == 'y' || ch == 'Y'){
		if( discreteLog(A,B) == 0)
			std::cout << "\nLog does not exist\n";
		else
			std::cout << std::endl << discreteLog(A,B) <<std::endl;
	}
	Main_menu();
}


void ECC_binary(){									//ECC prime input of a, b and points or computer-generated?
	std::cout<<"Do you wish to: 1. enter irreducible polynomial , a, b and the points on the ECC or 2. use a random ECC?\n";
	int in;
	std::cin>>in;
	G2E A, B;
	if(in == 1){
		std::cout << "enter the polynomial (in decimal)\n";
		mpz_class prime,a, b;
		std::cin >> prime;
		GF2::set(prime);
		std::cout << "input a and b\n";
		std::cin >> a >> b;
		G2E::set(a, b);
		std::cout<<"enter 2 points (separated by a white space or return carriage)\n";
		std::cin>>A.x >> A.y >> B.x >> B.y;
	}
	else if (in ==2){
		mpz_class p(0), P, primePol[]={19, 25, 31, 47, 41, 59, 61, 55};
		p = primePol[ random(0,7).get_ui() ];
		GF2::set(p);
		mpz_class lm = GF2::get();
		mpz_class a = random(1,lm-1), b = random(1, lm - 1);
		G2E::set(GF2(a), GF2(b));
		while(A == G2E())	A = G2E(GF2(random(1,lm-1)));
		while(B == G2E())	B = G2E(GF2(random(1,lm-1)));		
		std::cout << "Prime : " << p << std::endl;
		std::cout << "a : "<< a << "\nb : " << b << std::endl; 
		std::cout << "A : "<< A << "\nB : " << B << std::endl; 
	}
	else	Main_menu();
	
	std::cout<<"\n\nA + B = "<<A * B
		<<"\nInverse of A : "<<A.inverse()
		<<"\nInverse of B : "<<B.inverse();
	std::cout<<"\n\nDo you want to do scalar multiplication?(y/n)\t";
	char ch;
	std::cin >> ch;
	if( ch == 'y' || ch == 'Y'){
		mpz_class k;
		std::cout << "\nenter scalar\n";
		std::cin >> k;
		std::cout << "\nkA = "<<exponentiation( A, k) ;
		std::cout << "\nkB = "<<exponentiation( B, k) ;
	}
	std::cout << "\nDo you wish to calculate discrete log of a with base b? (y/n)";
	std::cin >> ch;
	if( ch == 'y' || ch == 'Y'){
		if( discreteLog(A,B) == 0)
			std::cout << "\nLog does not exist\n";
		else
			std::cout << std::endl << discreteLog(A,B) <<std::endl;
	}
	Main_menu();
}

void sub_menu1(){
	int ch=1;
	std::cout<<"\n\n1. GF(2)\n"
		<<"2. Prime field\n"
		<<"3. Elliptic curves over prime fields\n"
		<<"4. Elliptic curves over binary field\n"
		<<"0. any other key to return to main menu\n\n";
	std::cin>> ch;
	switch(ch){
		case 1:
			Binary_field();
			break;
		case 2:
			Prime_field();;
			break;
		case 3:
			ECC_prime();
			break;
		case 4:
			ECC_binary();
			break;
		default:
			Main_menu();
	}
}


void sub_menu2(){
	std::cout<< " \nNOTE:\nPlease enter the message (integers only) to be encyrypted inside inp.in\n\n";
	std::cout<<"Choose encryption type:\n"
		<<"\t1. El-Gamal scheme over prime fields\n"
		<<"\t2. El-Gamal scheme over binary fields\n"
		<<"\t3. El-Gamal scheme over ECC prime fields\n"
		<<"\t4. El-Gamal scheme over ECC binary fields\n"
		<<"\t5. RSA\n"
		<<"\t0. other key to return to main menu\n\n";
	int ch;
	std::cin>>ch;
	switch(ch){
		case 1:
			std::cout<<"\nencrypted message is in : CS.enc\nsignature is in : CS.sign\ndecrypted message is in : CS.dec\nVerification status: ";
			primefield("inp.in","CS");
			break;
		case 2:
			std::cout<<"\nencrypted message is in : CS.enc\nsignature is in : CS.sign\ndecrypted message is in : CS.dec\nVerification status: ";
			gf2("inp.in","CS");
			break;
		case 3:
			std::cout<<"\nencrypted message is in : CS.enc\nsignature is in : CS.sign\ndecrypted message is in : CS.dec\nVerification status: ";
			pecc("inp.in","CS");
			break;
		case 4:
			std::cout<<"\nencrypted message is in : CS.enc\nsignature is in : CS.sign\ndecrypted message is in : CS.dec\nVerification status: ";
			g2ecc("inp.in","CS");
			break;
		case 5:
			std::cout<<"\nencrypted message is in : CS.enc\nsignature is in : CS.sign\ndecrypted message is in : CS.dec\nVerification status: ";
			rsa("inp.in","CS");
			break;
		default:
			Main_menu();
	}
	Main_menu();
}


void Main_menu(){
	int ch=1;
	std::cout<<"\n\n";
	for(int i=0; i < 40 ; i++)
		std::cout<<"--";
	std::cout<<std::endl;
	std::cout<<"1. Operations on GF(n) fields and elliptic curves\n"
		<<"2. Encryption and decryption (Prime field, Binary Field, ECC over prime field, ECC over binary field and RSA)\n"
		<<"0. any other key to exit\n\n";
	std::cin>> ch;
	for(int i=0; i < 40 ; i++)
		std::cout<<"--";
	std::cout<<"\n\n";
	switch(ch){
		case 1:
			sub_menu1();
			break;
		case 2:
			sub_menu2();
			break;
		default:
			return;
	}
}

int main(){
	std::cout << "\nImportant: Please read the readme.txt file before continuing. \n";
	Main_menu();
	return 0;
}	
#ifndef RSA
#define RSA
void generateKey();
#endif

#ifndef ALG_H
#define ALG_H
#include <gmpxx.h>
mpz_class gcd(mpz_class, mpz_class);
mpz_class inverse(mpz_class, mpz_class);
mpz_class power(mpz_class, mpz_class, mpz_class);
mpz_class gPrime(mpz_class, mpz_class);
mpz_class random( mpz_class ll, mpz_class ul);
#endif
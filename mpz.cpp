#include <gmpxx.h>

mpz_class operator<<(mpz_class x, mpz_class a){
	mpz_class buff;
	mpz_mul_2exp( buff.get_mpz_t(), x.get_mpz_t(), a.get_ui() );
	return buff;
}

mpz_class operator>>(mpz_class X, mpz_class a){
	mpz_class buff;
	mpz_tdiv_q_2exp( buff.get_mpz_t() , X.get_mpz_t() , a.get_ui()); 
	return buff;
}
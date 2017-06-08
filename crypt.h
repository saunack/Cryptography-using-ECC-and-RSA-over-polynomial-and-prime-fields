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

#ifndef CRYPTO_H
#define CRYPTO_H
bool isProper( PF M );
bool isProper( PE M );
bool isProper( GF2 M );

void primefield( std::string, std::string );
void gf2( std::string, std::string );
void pecc( std::string, std::string );
void g2ecc( std::string, std::string );
void rsa( std::string, std::string );
#endif
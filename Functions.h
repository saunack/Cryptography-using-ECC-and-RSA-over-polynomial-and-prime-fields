#include <gmpxx.h>
#ifndef DiscreteLog_H
#define DiscreteLog_H
template <typename T>
mpz_class discreteLog(const T&, const T&);
#endif

#ifndef Expo_H
#define Expo_H
template <typename T>
T exponentiation(T Base, mpz_class Exponent);
#endif
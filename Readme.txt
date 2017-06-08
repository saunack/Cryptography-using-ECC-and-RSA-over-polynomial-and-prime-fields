Compile the program using the make command in terminal

Executable file is "target.exe"

Set the lower limit of prime (for ECC over primefield) using ECCMin and ECCMax macro defined inside crypt.cpp
Set the lower limit of prime (for RSA) using RSAMin and RSAMax macro defined inside crypt.cpp
Set the lower limit of prime (for primefield) using PrimeMin and PrimeMax macro defined inside crypt.cpp

For encryption:
1. Message ( integer(s) ) to be encrypted is to be entered in the file "inp.in"
2. The ciphertext will be output in "CS.enc"
3. The signature will be output in "CS.sign"
4. The decrypted text will be output in "CS.dec"
5. The verification status will be output on the console

Modules of encryption program:
There are 4 parts to each encryption-decryption program: Key generation (<tq>_key), encryption (<tq>_enc), signing  (<tq>_sign), decryption (<tq>_dec) and verification (<tq>_verify) (<tq> refers to Prime ECC, Binary ECC, Prime Field, Binary Field and RSA).

Algorithms implemented:
1. Prime generation - Fermat's test and Miller-Rabin's test
2. Discrete Logarithm - Baby-step Giant-step
3. Exponentiation - Double and add / Square and Multiply
4. Inverse and GCD - Extended Euclidean Algorithm

Note:
1. Proper hashing functions have not implemented in our program. Instead, we have used the last input as the hash (which illustrates the cryptographic techniques just as well). Consequently, do not enter zero as an input in the message to be encrypted.
2.Unlike the primes, an algorithm for generating the irreducible polynomials has not been implemeneted in this project. Instead, the polynomial is chosen from a predefined list inside the functions GF2_key() and G2E_key().
3. GNU Multiprecision Arithmetic Library has been used for operations involving integers larger than the default range offered by C/C++ compilers
4. Random numbers have been generated using the inbuilt Mersenne Twister algorithm included in the header random
5. GCD has not been included explicitly in the menu, but has been defined inside the program and is used.
1. What happens if your modulus n is too small, keeping in mind the range of the ASCII values for alpha-numeric characters?

    If n is too small, the message will be easy to decrpted using brute force attack.
    For instance, if n is 16 digits long, p and q can be calculated in a blink of an eye just by iterating the possible products of primes from a certain range.
    And given the fact that n and e are public, p, q, d can be cracked easily if n is too small.

2. With quantum computers, will it be easy to crack the RSA algorithm? Why?

    Yes. The reason why RSA is still secure is the computing power of mordern techonologies is still limited. Factoring a 256-bit modulus could take hundreds of millions of years. However, quantum computers are at least hundread of millions times faster than classical computers. In addition, there's an algorithm called Shor's algorithm, which is a quantum conputer algorithm for finding the prime factors of an integer. With this algorithm, cracking RSA would even be more easily. 

3. For very large values of p and q and the large data spaces involved, is it reasonable to perform all parts of RSA encryption in software? What parts might you consider implementing in hardware? To answer this, you might wish to look at the abstract of the following paper: Efficient hardware implementation of RSA cryptography.

    No. When p and q are large, the time required for calculation can be long. The modular arithmetic since "the core of the RSA implementation is how efficient the modular arithmetic operations are". Programmed FPGA can be used to implement the modular arithimetic required by RSA.
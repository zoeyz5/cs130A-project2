#ifndef HASH24_H
#define HASH24_H

#include <string>

// Helper class for calculating hash values using universal randomly selected hash functions.
class Hash24 {

private:
   // some prime numbers of suitable size
   static long const prime1 = 16890581L ;   // prime number ~ 2^24 = 16777216
   static long const prime2 = 17027399L ;   // larger prime ~ 2^24 = 16777216

   // random values for the hash function
   long random_a ;
   long random_b ; 
   long random_c ;

public:
    Hash24();

	long get_random_a() {
		return random_a;
	}
	long get_random_b() {
		return random_b;
	}
	long get_random_c() {
		return random_c;
	}
	void set_random_a(long random_a) {
		 this->random_a = random_a;
	}
	void set_random_b(long random_b) {
		this->random_b = random_b;
	}
	void set_random_c(long random_c) {
		this->random_c = random_c;
	}
    int hash(long x);
    int hash(std::string str);
    void dump();
	Hash24 operator = (const Hash24 & copy_class);
};

#endif


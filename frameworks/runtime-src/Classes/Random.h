#ifndef __Boids_Random_h_
#define __Boids_Random_h_

//minstd_rand
//http://en.wikipedia.org/wiki/Linear_congruential_generator

#include <cstdint>

class Random
{
private:
	int _seed;
	int a;
	int m;
	unsigned i;

	int gen()
	{
		i++;
		_seed = ((uint64_t)_seed * a) % m;
		return _seed;
	}
public:
	Random() : 
		_seed( 1090616 ),
		a( 48271 ),
		m( 2147483647 ),
		i( 0 ) {
	}

	void seed(int new_seed)
	{
		_seed = new_seed;
	}

	unsigned random_i()
	{
		return i;
	}

	int randomNumber(int upper_bound)
	{
		return gen() % upper_bound; // 
	}

	float randomFloat()
	{
		return (float)gen() / m;
	}
};
#endif
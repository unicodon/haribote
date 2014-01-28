#ifndef RANDOM_H
#define RANDOM_H


#define USHORT15b_RAND	(Random::GetRandUShort15b())
#define DOUBLE_RAND		(Random::GetRandDouble())
#define GAUSSIAN_RAND	(Random::GetRandGaussian())

double gasdev();

class Random
{
public:
	static unsigned short GetRandUShort15b(){
		return rand_ushort15b[count_ushort15b++];
	};

	static double GetRandDouble(){
		return rand_double[count_double++];
	};

	static double GetRandGaussian(){
		return rand_gaussian[count_gaussian++];
	};

	static void init_rand();

private:
	static bool init_flag;

	static unsigned short rand_ushort15b[0x10000];//0Å`32767
	static double rand_double[0x10000];
	static double rand_gaussian[0x10000];

	static unsigned short count_ushort15b;
	static unsigned short count_double;
	static unsigned short count_gaussian;

};

class MtRand
{
public:
	/* initializes mt[N] with a seed */
	static void init_genrand(unsigned long s);

	/* initialize by an array with array-length */
	/* init_key is the array for initializing keys */
	/* key_length is its length */
	/* slight change for C++, 2004/2/26 */
	static void init_by_array(unsigned long init_key[], int key_length);

	/* generates a random number on [0,0xffffffff]-interval */
	static unsigned long genrand_int32(void);

	/* generates a random number on [0,0x7fffffff]-interval */
	static long genrand_int31(void);

	/* These real versions are due to Isaku Wada, 2002/01/09 added */
	/* generates a random number on [0,1]-real-interval */
	static double genrand_real1(void);

	/* generates a random number on [0,1)-real-interval */
	static double genrand_real2(void);

	/* generates a random number on (0,1)-real-interval */
	static double genrand_real3(void);

	/* generates a random number on [0,1) with 53-bit resolution*/
	static double genrand_res53(void);

private:
	static unsigned long mt[];
	static int mti;
};


#endif
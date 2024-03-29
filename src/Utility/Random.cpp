#include "stdafx.h"
#include "Random.h"
#include <math.h>

bool Random::init_flag = false;


double gasdev(){
	static int iset = 0;
	static float gset;
	float fac,rsq,v1,v2;

	if(iset == 0){
		do {
			v1 = 2.0*DOUBLE_RAND-1.0;
			v2 = 2.0*DOUBLE_RAND-1.0;
			rsq=v1*v1+v2*v2;
		}while(rsq >= 1.0 || rsq == 0.0);
		fac = sqrt(-2.0*log(rsq)/rsq);
		gset = v1*fac;
		iset = 1;
		return v2*fac;
	}else{
		iset = 0;
		return gset;
	}	
}


void Random::init_rand(){
	if(init_flag)return;


	MtRand mt;
	mt.init_genrand(100);
	
	for(int i = 0;i < 0x10000;i++){
		rand_ushort15b[i] = mt.genrand_int32();
		rand_ushort15b[i] >>= 2;
		rand_double[i] = (double)USHORT15b_RAND/0x7fff;
	}

	for(int i = 0;i < 0x10000;i++){
		rand_gaussian[i] = gasdev();
	}

	init_flag = true;

	//OSYSPRINT(("init_rand() finished\n"));

}

unsigned short	Random::rand_ushort15b[0x10000];
double			Random::rand_double[0x10000];
double			Random::rand_gaussian[0x10000];

unsigned short Random::count_ushort15b = 0;
unsigned short Random::count_double = 0;
unsigned short Random::count_gaussian = 0;

static Random myrand;//

/* Period parameters */  
#define N 624
#define M 397
#define MATRIX_A 0x9908b0dfUL   /* constant vector a */
#define UPPER_MASK 0x80000000UL /* most significant w-r bits */
#define LOWER_MASK 0x7fffffffUL /* least significant r bits */

unsigned long MtRand::mt[N]; /* the array for the state vector  */
int MtRand::mti=N+1; /* mti==N+1 means mt[N] is not initialized */

/* initializes mt[N] with a seed */
void MtRand::init_genrand(unsigned long s)
{
    mt[0]= s & 0xffffffffUL;
    for (mti=1; mti<N; mti++) {
        mt[mti] = 
	    (1812433253UL * (mt[mti-1] ^ (mt[mti-1] >> 30)) + mti); 
        /* See Knuth TAOCP Vol2. 3rd Ed. P.106 for multiplier. */
        /* In the previous versions, MSBs of the seed affect   */
        /* only MSBs of the array mt[].                        */
        /* 2002/01/09 modified by Makoto Matsumoto             */
        mt[mti] &= 0xffffffffUL;
        /* for >32 bit machines */
    }
}

/* initialize by an array with array-length */
/* init_key is the array for initializing keys */
/* key_length is its length */
/* slight change for C++, 2004/2/26 */
void MtRand::init_by_array(unsigned long init_key[], int key_length)
{
    int i, j, k;
    init_genrand(19650218UL);
    i=1; j=0;
    k = (N>key_length ? N : key_length);
    for (; k; k--) {
        mt[i] = (mt[i] ^ ((mt[i-1] ^ (mt[i-1] >> 30)) * 1664525UL))
          + init_key[j] + j; /* non linear */
        mt[i] &= 0xffffffffUL; /* for WORDSIZE > 32 machines */
        i++; j++;
        if (i>=N) { mt[0] = mt[N-1]; i=1; }
        if (j>=key_length) j=0;
    }
    for (k=N-1; k; k--) {
        mt[i] = (mt[i] ^ ((mt[i-1] ^ (mt[i-1] >> 30)) * 1566083941UL))
          - i; /* non linear */
        mt[i] &= 0xffffffffUL; /* for WORDSIZE > 32 machines */
        i++;
        if (i>=N) { mt[0] = mt[N-1]; i=1; }
    }

    mt[0] = 0x80000000UL; /* MSB is 1; assuring non-zero initial array */ 
}

/* generates a random number on [0,0xffffffff]-interval */
unsigned long MtRand::genrand_int32(void)
{
    unsigned long y;
    static unsigned long mag01[2]={0x0UL, MATRIX_A};
    /* mag01[x] = x * MATRIX_A  for x=0,1 */

    if (mti >= N) { /* generate N words at one time */
        int kk;

        if (mti == N+1)   /* if init_genrand() has not been called, */
            init_genrand(5489UL); /* a default initial seed is used */

        for (kk=0;kk<N-M;kk++) {
            y = (mt[kk]&UPPER_MASK)|(mt[kk+1]&LOWER_MASK);
            mt[kk] = mt[kk+M] ^ (y >> 1) ^ mag01[y & 0x1UL];
        }
        for (;kk<N-1;kk++) {
            y = (mt[kk]&UPPER_MASK)|(mt[kk+1]&LOWER_MASK);
            mt[kk] = mt[kk+(M-N)] ^ (y >> 1) ^ mag01[y & 0x1UL];
        }
        y = (mt[N-1]&UPPER_MASK)|(mt[0]&LOWER_MASK);
        mt[N-1] = mt[M-1] ^ (y >> 1) ^ mag01[y & 0x1UL];

        mti = 0;
    }
  
    y = mt[mti++];

    /* Tempering */
    y ^= (y >> 11);
    y ^= (y << 7) & 0x9d2c5680UL;
    y ^= (y << 15) & 0xefc60000UL;
    y ^= (y >> 18);

    return y;
}

/* generates a random number on [0,0x7fffffff]-interval */
long MtRand::genrand_int31(void)
{
    return (long)(genrand_int32()>>1);
}

/* generates a random number on [0,1]-real-interval */
double MtRand::genrand_real1(void)
{
    return genrand_int32()*(2.3283064370807973754314699618685e-10);
    /* divided by 2^32-1 */ 
}

/* generates a random number on [0,1)-real-interval */
double MtRand::genrand_real2(void)
{
    return genrand_int32()*(1.0/4294967296.0); 
    /* divided by 2^32 */
}

/* generates a random number on (0,1)-real-interval */
double MtRand::genrand_real3(void)
{
    return (((double)genrand_int32()) + 0.5)*(1.0/4294967296.0); 
    /* divided by 2^32 */
}

/* generates a random number on [0,1) with 53-bit resolution*/
double MtRand::genrand_res53(void) 
{ 
    unsigned long a=genrand_int32()>>5, b=genrand_int32()>>6; 
    return(a*67108864.0+b)*(1.0/9007199254740992.0); 
} 
/* These real versions are due to Isaku Wada, 2002/01/09 added */

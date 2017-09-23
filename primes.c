#include <math.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <time.h>


/* Retired Algorithms
[0] Brute force, but skipping even numbers -- nearly identical performance to naive approach since
    the very first trial division by 2 always indicates a composite value
[1] Variations using double precision floating-point instead of uint64_t integers -- see profile.c
    for summary results, but double multiplication and division have very comparable operation rates
    to uint64_t division
[2] "Faster" integer square root algorithms for early termination checks -- casts to floating-point,
    hardware square root, ceiling, and cast back to integer is still 2x faster than Warren's Figure
    11-1 algorithm with excellent starting guesses based on the previous monotonic square root
    calculated
*/

/**************************************************************************************************/

void algo0(uint32_t num, uint64_t* primes)
{
  printf("ALGORITHM 0: absolutely naive brute force\n");

  // variables
  bool comp;
  uint32_t m;

  // initialize first two primes
  primes[0] = 2ul;
  primes[1] = 3ul;

  // find the rest
  uint32_t n = 2;
  uint64_t val = 4ul;
  while (n < num)
  {
    comp = false;
    for (m = 0; m < n; ++m)
    {
      if (0ul == (val % primes[m]))
      {
        comp = true;
        break;
      }
    }

    if (!comp)
    {
      primes[n] = val;
      ++n;
    }
    ++val;
  }

  return;
}

/**************************************************************************************************/

void algo1(uint32_t num, uint64_t* primes)
{
  printf("ALGORITHM 1: smarter trial division with early termination at square root\n");

  // variables
  bool comp;
  uint32_t m;

  // initialize first two primes
  primes[0] = 2ul;
  primes[1] = 3ul;

  // find the rest
  uint32_t n = 2;
  uint64_t val = 5ul;
  uint64_t root = 3ul;
  uint64_t rootSquare = root * root;
  while (n < num)
  {
    comp = false;
    if (rootSquare < val)  // update of root is required
    {
      ++root;
        // this is safe because val increments by two so the root can never increase more than one
      rootSquare += (root << 1) + 1;
        // (root + 1)^2 = root^2 + 2*root + 1
    }
    for (m = 1; m < n; ++m)  // skip checking 2 since all evens are skipped
    {
      if (primes[m] > root)  // early termination
        break;
      else
        if (0ul == (val % primes[m]))
        {
          comp = true;
          break;
        }
    }

    if (!comp)
    {
      primes[n] = val;
      ++n;
    }
    val += 2ul;  // skip all even values
  }

  return;
}

/**************************************************************************************************/

void algo2(uint32_t num, uint64_t* primes)
{
  printf("ALGORITHM 2: full Sieve of Eratosthenes with bool array memory\n");

  // determine the sieve size and allocate the array
  const double dblNum = (double) num;
  const uint32_t sieveSize = (uint32_t) (dblNum * log(dblNum * log(dblNum))) + 1;
    /* from Wikipedia bound (Prime-counting_function#Inequalities) with +1
       for the unused zero index */
  printf("num = %d, sieveSize = %d\n", num, sieveSize);
  bool* sieve = NULL;
  sieve = malloc(sizeof(bool) * sieveSize);
  if (sieve == NULL)
  {
    printf("malloc() failed\n");
    return;
  }
  memset(sieve, false, sieveSize);

  // initialize first two primes
  uint32_t n = 2;
  uint32_t s;
  primes[0] = 2ul;
  primes[1] = 3ul;
  for (s = 2; s < sieveSize; s += 2)
    sieve[s] = true;
  for (s = 3; s < sieveSize; s += 3)
    sieve[s] = true;

  // find the rest
  uint64_t val = 5ul;
  while (n < num)
  {
    // check the sieve
    if (!sieve[val])
    {
      primes[n] = val;
      ++n;
      for (s = val; s < sieveSize; s += val)
        sieve[s] = true;
    }
    ++val;
  }

  free((void *) sieve);
  primes = NULL;

  return;
}

/**************************************************************************************************/

void algo3(uint32_t num, uint64_t* primes)
{
  printf("ALGORITHM 3: half Sieve of Eratosthenes with bool array memory\n");

  // determine the sieve size and allocate the array
  const double dblNum = (double) num;
  const uint32_t sieveSize = (uint32_t) (dblNum * log(dblNum * log(dblNum))) + 1;
  const uint32_t sieveHalfSize = sieveSize >> 1;
    /* from Wikipedia bound (Prime-counting_function#Inequalities) with +1
       for the unused zero index */
  printf("num = %d, sieveHalfSize = %d\n", num, sieveHalfSize);
  bool* sieve = NULL;
  sieve = malloc(sizeof(bool) * sieveHalfSize);
  if (sieve == NULL)
  {
    printf("malloc() failed\n");
    return;
  }
  memset(sieve, false, sieveHalfSize);

  // initialize first two primes
  uint32_t n = 2;
  uint64_t s;
  primes[0] = 2ul;
  primes[1] = 3ul;
  for (s = 3ul; s < sieveSize; s += 6ul)  // stride 6 since there are no evens
    sieve[(s + 1ul) >> 1] = true;

  // find the rest
  uint64_t val = 5ul;
  uint64_t val2;
  while (n < num)
  {
    // check the sieve
    if (!sieve[(val + 1ul) >> 1])
    {
      primes[n] = val;
      ++n;
      val2 = val << 1;
      for (s = val + val2; s < sieveSize; s += val2) // stride 2x since there are no evens
        sieve[(s + 1) >> 1] = true;
    }
    val += 2ul;  // skip even values
  }

  free((void *) sieve);
  primes = NULL;

  return;
}

/**************************************************************************************************/

inline bool getBit(uint64_t* mem, uint64_t bitNum)
{
  uint64_t val = mem[bitNum >> 6];
  val >>= bitNum & 0x3F;
  return(val & 0x1);
}

inline void setBit(uint64_t* mem, uint64_t bitNum)
{
  mem[bitNum >> 6] |= 1ul << (bitNum & 0x3F);
  return;
}

void algo4(uint32_t num, uint64_t* primes)
{
  printf("ALGORITHM 4: half Sieve of Eratosthenes with bit field memory\n");

  // determine the sieve size and allocate the array
  const double dblNum = (double) num;
  const uint32_t sieveSizeBits = (uint32_t) (dblNum * log(dblNum * log(dblNum))) + 1;
  const uint32_t sieveHalfSizeBits = sieveSizeBits >> 1;
    /* from Wikipedia bound (Prime-counting_function#Inequalities) with +1
       for the unused zero index */
  uint32_t sieveHalfSizeWords = sieveHalfSizeBits >> 6;
  if (sieveHalfSizeBits & 0x3F)
    ++sieveHalfSizeWords;
  printf("num = %d, sieveHalfSizeBits = %d, sieveHalfSizeWords = %d\n",
         num, sieveHalfSizeBits, sieveHalfSizeWords);
  uint64_t* sieve = NULL;
  sieve = malloc(sizeof(uint64_t) * sieveHalfSizeWords);
  if (sieve == NULL)
  {
    printf("malloc() failed\n");
    return;
  }
  memset(sieve, 0, sieveHalfSizeWords * sizeof(uint64_t));

  // initialize first two primes
  uint32_t n = 2;
  uint64_t s;
  primes[0] = 2ul;
  primes[1] = 3ul;
  for (s = 2ul; s < sieveHalfSizeBits; s += 3ul)  // stride 6 since there are no evens
    setBit(sieve, s);

  // find the rest
  uint64_t idx = 3ul;  // val = 5ul
  uint64_t val;
  while (n < num)
  {
    // check the sieve
    if (!getBit(sieve, idx))
    {
      val = (idx << 1) - 1ul;
      primes[n] = val;
      ++n;
      for (s = (3ul * val + 1ul) >> 1; s < sieveHalfSizeBits; s += val)
        setBit(sieve, s);
    }
    ++idx;
  }
    // key relationships for mapping between the actual number values and sieve bit indices:
    // idx(val) = (val + 1) >> 1
    // val(idx) = (idx << 1) + 1

  free((void *) sieve);
  primes = NULL;

  return;
}

/**************************************************************************************************/

void algo5(uint32_t num, uint64_t* primes)
{
  printf("ALGORITHM 5: third Sieve of Eratosthenes with bit field memory\n");

  // determine the sieve size and allocate the array
  const double dblNum = (double) num;
  const uint32_t sieveSizeBits = (uint32_t) (dblNum * log(dblNum * log(dblNum))) + 1;
  const uint32_t sieveThirdSizeBits = sieveSizeBits / 3;
    /* from Wikipedia bound (Prime-counting_function#Inequalities) with +1
       for the unused zero index */
  uint32_t sieveThirdSizeWords = sieveThirdSizeBits >> 6;
  if (sieveThirdSizeBits % 3)
    ++sieveThirdSizeWords;
  printf("num = %d, sieveThirdSizeBits = %d, sieveThirdSizeWords = %d\n",
         num, sieveThirdSizeBits, sieveThirdSizeWords);
  uint64_t* sieve = NULL;
  sieve = malloc(sizeof(uint64_t) * sieveThirdSizeWords);
  if (sieve == NULL)
  {
    printf("malloc() failed\n");
    return;
  }
  memset(sieve, 0, sieveThirdSizeWords * sizeof(uint64_t));

  // initialize first two primes
  uint32_t n = 2;
  uint64_t s;
  primes[0] = 2ul;
  primes[1] = 3ul;

  // find the rest
  uint64_t idx = 1ul;  // val = 5ul
  uint64_t val;
  bool isOdd, useA;
  uint64_t strideA, strideB;
  while (n < num)
  {
    // check the sieve
    if (!getBit(sieve, idx))
    {
      isOdd = idx & 1;
      val = 3u * idx + ((isOdd) ? 2ul : 1ul);
      primes[n] = val;
      ++n;  
      
      strideA = (val << 1) / 3u + ((isOdd) ? 0ul : 1ul);
      strideB = (val << 2) / 3u + ((isOdd) ? 1ul : 0ul);
      
      //printf("val = %lu, init = %lu, strideA = %lu, strideB = %lu, idx = %lu\n",
      //       val, 5u * val / 3u, strideA, strideB, idx);
      useA = true;
      for (s = (5u * val / 3u); s < sieveThirdSizeBits; )
      {
        //printf("  setting idx %lu\n", s);
        setBit(sieve, s);
        s += (useA) ? strideA : strideB;
        useA = !useA;
      }
    }
    ++idx;
  }
    // UPDATE THESE
    // key relationships for mapping between the actual number values and sieve bit indices:
    // idx(val) = (val + 1) >> 1
    // val(idx) = (idx << 1) + 1

  free((void *) sieve);
  primes = NULL;

  return;
}

/**************************************************************************************************/

int main(int argc, char **argv)
{
  // parameters
  const uint32_t numPrint = 8;

  // setup array of function points for all algorithm variants
  typedef void ALGO_FUNCTION(uint32_t, uint64_t*);
  ALGO_FUNCTION * algos[6] = {&algo0, &algo1, &algo2, &algo3, &algo4, &algo5};
  const int numAlgs = 6;

  // check input arguments
  if (argc != 3)
  {
    printf("usage: '%s <algorithm index> <number of primes to generate>'\n", argv[0]);
    return(1);
  }
  int algIdx = atoi(argv[1]);
  if ((algIdx < 0) || (algIdx >= numAlgs))
  {
    printf("algorithm index must be on [0, numAlgos - 1]\n");
    return(1);
  }
  ALGO_FUNCTION * func = algos[algIdx];
  const uint32_t num = atoi(argv[2]);

  // allocate array from primes
  uint64_t* primes = NULL;
  primes = malloc(sizeof(uint64_t) * num);
  if (NULL == primes)
  {
    printf("malloc() failed\n");
    return(1);
  }

  // run the algorithm with timing
  clock_t tBegin, tEnd;
  double timeSpent;
  tBegin = clock();

  (*func)(num, primes);

  tEnd = clock();
  timeSpent = (double) (tEnd - tBegin) / CLOCKS_PER_SEC;

  // print select results
  uint32_t n;
  for (n = 0; n < numPrint; ++n)
    printf("primes[%u] = %lu\n", n, primes[n]);
  printf("...\n");
  for (n = num - numPrint; n < num; ++n)
    printf("primes[%u] = %lu\n", n, primes[n]);

  // clean up memory
  free((void *) primes);
  primes = NULL;

  // report final timing
  printf("time: %0.3f s\n", timeSpent);
  printf("done\n");
  return(0);
}


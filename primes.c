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
  uint64_t val = 4ul;

  uint32_t n = 2;
  while (n < num)
  {
    comp = false;
    for (m = 0; m < n; ++m)
    {
      if ((val % primes[m]) == 0ul)
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
  uint64_t val = 5ul;
  uint64_t root;

  uint32_t n = 2;
  while (n < num)
  {
    comp = false;
    root = (uint64_t) ceil(sqrt((double) val));
    //printf("val: %ld, root: %ld\n", val, root);

    for (m = 1; m < n; ++m)  // skip checking 2
    {     
      if (primes[m] > root)  // early termination
        break;
      else
        if ((val % primes[m]) == 0ul)
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
  uint32_t s;
  primes[0] = 2ul;
  primes[1] = 3ul;
  for (s = 3; s < sieveSize; s += 6)  // stride 6 since there are no evens
    sieve[(s + 1) >> 1] = true;

  // find the rest
  uint64_t val = 5ul;
  uint64_t val2;
  while (n < num)
  {
    // check the sieve
    if (!sieve[(val + 1) >> 1])
    {
      primes[n] = val;
      ++n;
      val2 = val << 1;
      for (s = val; s < sieveSize; s += val2) // stride 2x since there are no evens
        sieve[(s + 1) >> 1] = true;
    }
    val += 2;  // skip even values
  }

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
  ALGO_FUNCTION * algos[4] = {&algo0, &algo1, &algo2, &algo3};
  const int numAlgs = 4;

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
  for (uint32_t n = 0; n < numPrint; ++n)
    printf("primes[%u] = %lu\n", n, primes[n]);
  printf("...\n");
  for (uint32_t n = num - numPrint; n < num; ++n)
    printf("primes[%u] = %lu\n", n, primes[n]);

  // clean up memory
  free((void *) primes);
  primes = NULL;

  // report final timing
  printf("time: %0.3f s\n", timeSpent);
  printf("done\n");
  return(0);
}


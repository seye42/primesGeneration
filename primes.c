#include <math.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

const uint32_t numPrint = 16;

void algo_0(uint32_t num, uint64_t* primes)
{
  // absolutely naive, brute-force approach

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


void algo_1(uint32_t num, uint64_t* primes)
{
  // slightly smarter than 0: skip the even numbers

  // variables
  bool comp;
  uint32_t m;

  // initialize first two primes
  primes[0] = 2ul;
  primes[1] = 3ul;

  // find the rest
  uint64_t val = 5ul;

  uint32_t n = 2;
  while (n < num)
  {
    comp = false;
    for (m = 1; m < n; ++m)  // skip checking 2
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
    val += 2ul;  // skip all even values
  }

  return;
}


void algo_2(uint32_t num, uint64_t* primes)
{
  // early termination of check loop based on square roots

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


void algo_2flt(uint32_t num, double* primes)
{
  // early termination of check loop based on square roots
  // double precision arithmetic

  // variables
  bool comp;
  uint32_t m;

  // initialize first two primes
  primes[0] = 2.0;
  primes[1] = 3.0;

  // find the rest
  double val = 5.0;
  double root;
  double ratio;

  uint32_t n = 2;
  while (n < num)
  {
    comp = false;
    root = sqrt(val);
    //printf("val: %ld, root: %ld\n", val, root);

    for (m = 1; m < n; ++m)  // skip checking 2
    {     
      if (primes[m] > root)  // early termination
        break;
      else
        ratio = val / primes[m];
        if (floor(ratio) == ratio)
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
    val += 2.0;  // skip all even values
  }

  return;
}


void algo_2invflt(uint32_t num, double* primes)
{
  // early termination of check loop based on square roots
  // double precision arithmetic
  // use of inverses (trial division becomes trial multiplication)

  // variables
  bool comp;
  uint32_t m;

  // initialize first two primes
  primes[0] = 1.0 / 2.0;
  primes[1] = 1.0 / 3.0;

  // find the rest
  double val = 5.0;
  double root;
  double ratio;

  uint32_t n = 2;
  while (n < num)
  {
    comp = false;
    root = sqrt(val);
    //printf("val: %ld, root: %ld\n", val, root);

    for (m = 1; m < n; ++m)  // skip checking 2
    {     
      if (primes[m] > root)  // early termination
        break;
      else
        ratio = val * primes[m];
        if (floor(ratio) == ratio)
        {
          comp = true;
          break;
        }
    }

    if (!comp)
    {
      primes[n] = 1.0 / val;
      ++n;
    }
    val += 2.0;  // skip all even values
  }

  return;
}


void algo_3(uint32_t num, uint64_t* primes)
{
  // sieve of Eratosthenes

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


void algo_4(uint32_t num, uint64_t* primes)
{
  // half-size sieve of Eratosthenes

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

int main(int argc, char **argv)
{
  if (argc != 2)
  {
    printf("usage: '%s <number of primes to generate>'\n", argv[0]);
    return(1);
  }

  const uint32_t num = atoi(argv[1]);

  uint64_t* primes = NULL;
  primes = malloc(sizeof(uint64_t) * num);
  //double* primes = NULL;
  //primes = malloc(sizeof(double) * num);
  if (primes == NULL)
  {
    printf("malloc() failed\n");
    return(2);
  }

  clock_t tBegin, tEnd;
  double timeSpent;
  tBegin = clock();

  algo_4(num, primes);

  tEnd = clock();
  timeSpent = (double) (tEnd - tBegin) / CLOCKS_PER_SEC;

  // print results
  for (uint32_t n = 0; n < numPrint; ++n)
    printf("primes[%u] = %lu\n", n, primes[n]);
    //printf("primes[%u] = %f\n", n, primes[n]);
  printf("...\n");
  for (uint32_t n = num - numPrint; n < num; ++n)
    printf("primes[%u] = %lu\n", n, primes[n]);
    //printf("primes[%u] = %f\n", n, primes[n]);

  free((void *) primes);
  primes = NULL;

  printf("time: %0.3f s\n", timeSpent);
  printf("done\n");
  return(0);
}


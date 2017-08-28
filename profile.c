#include <stdint.h>
#include <stdio.h>
#include <time.h>

uint64_t uint32Add(uint64_t numOps, uint64_t val)
{
  uint64_t idx;
  uint32_t ret = 1;

  for (idx = 0; idx < numOps; ++idx)
    ret += val;

  return(ret);
}

uint64_t uint32Subtract(uint64_t numOps, uint64_t val)
{
  uint64_t idx;
  uint32_t ret = 1;

  for (idx = 0; idx < numOps; ++idx)
    ret -= val;

  return(ret);
}

uint64_t uint32Multiply(uint64_t numOps, uint64_t val)
{
  uint64_t idx;
  uint32_t ret = 1;

  for (idx = 0; idx < numOps; ++idx)
    ret *= val;

  return(ret);
}

uint64_t uint32Divide(uint64_t numOps, uint64_t val)
{
  uint64_t idx;
  uint32_t ret = -1;

  for (idx = 0; idx < numOps; ++idx)
    ret /= val;

  return(ret);
}

uint64_t uint64Add(uint64_t numOps, uint64_t val)
{
  uint64_t idx;
  uint64_t ret = 1l;

  for (idx = 0; idx < numOps; ++idx)
    ret += val;

  return(ret);
}

uint64_t uint64Subtract(uint64_t numOps, uint64_t val)
{
  uint64_t idx;
  uint64_t ret = 1l;

  for (idx = 0; idx < numOps; ++idx)
    ret -= val;

  return(ret);
}

uint64_t uint64Multiply(uint64_t numOps, uint64_t val)
{
  uint64_t idx;
  uint64_t ret = 1l;

  for (idx = 0; idx < numOps; ++idx)
    ret *= val;

  return(ret);
}

uint64_t uint64Divide(uint64_t numOps, uint64_t val)
{
  uint64_t idx;
  uint64_t ret = -1l;

  for (idx = 0; idx < numOps; ++idx)
    ret /= val;

  return(ret);
}

double doubleAdd(uint64_t numOps, double val)
{
  uint64_t idx;
  double ret = 1.0;

  for (idx = 0; idx < numOps; ++idx)
    ret += val;

  return(ret);
}

double doubleSubtract(uint64_t numOps, double val)
{
  uint64_t idx;
  double ret = 1.0;

  for (idx = 0; idx < numOps; ++idx)
    ret -= val;

  return(ret);
}

double doubleMultiply(uint64_t numOps, double val)
{
  uint64_t idx;
  double ret = 1.0;

  for (idx = 0; idx < numOps; ++idx)
    ret *= val;

  return(ret);
}

double doubleDivide(uint64_t numOps, double val)
{
  uint64_t idx;
  double ret = -1000000000.0;

  for (idx = 0; idx < numOps; ++idx)
    ret /= val;

  return(ret);
}


int main()
{
  uint64_t numOps = 1000000000l;
  uint32_t valInt = 2;
  uint64_t valLng = 2l;
  double   valDbl = 2.0;
  uint32_t retInt;
  uint64_t retLng;
  double   retDbl;
  clock_t tBegin, tEnd;
  double timeSpent;

  // integer addition
  tBegin = clock();
  retInt = uint32Add(numOps, valInt);
  tEnd = clock();
  timeSpent = (double) (tEnd - tBegin) / CLOCKS_PER_SEC;
  printf("uint32 addition: %0.2f Mop/s (final = %d)\n", numOps / timeSpent / 1.0e6, retInt);

  // integer subtraction
  tBegin = clock();
  retInt = uint32Subtract(numOps, valInt);
  tEnd = clock();
  timeSpent = (double) (tEnd - tBegin) / CLOCKS_PER_SEC;
  printf("uint32 subtraction: %0.2f Mop/s (final = %d)\n", numOps / timeSpent / 1.0e6, retInt);

  // integer multiplication
  tBegin = clock();
  retInt = uint32Multiply(numOps, valInt);
  tEnd = clock();
  timeSpent = (double) (tEnd - tBegin) / CLOCKS_PER_SEC;
  printf("uint32 multiplication: %0.2f Mop/s (final = %d)\n", numOps / timeSpent / 1.0e6, retInt);

  // integer division
  tBegin = clock();
  retInt = uint32Divide(numOps, valInt);
  tEnd = clock();
  timeSpent = (double) (tEnd - tBegin) / CLOCKS_PER_SEC;
  printf("uint32 division: %0.2f Mop/s (final = %d)\n", numOps / timeSpent / 1.0e6, retInt);

  // long integer addition
  tBegin = clock();
  retLng = uint64Add(numOps, valLng);
  tEnd = clock();
  timeSpent = (double) (tEnd - tBegin) / CLOCKS_PER_SEC;
  printf("uint64 addition: %0.2f Mop/s (final = %ld)\n", numOps / timeSpent / 1.0e6, retLng);

  // long integer subtraction
  tBegin = clock();
  retLng = uint64Subtract(numOps, valLng);
  tEnd = clock();
  timeSpent = (double) (tEnd - tBegin) / CLOCKS_PER_SEC;
  printf("uint64 subtraction: %0.2f Mop/s (final = %ld)\n", numOps / timeSpent / 1.0e6, retLng);

  // long integer multiplication
  tBegin = clock();
  retLng = uint64Multiply(numOps, valLng);
  tEnd = clock();
  timeSpent = (double) (tEnd - tBegin) / CLOCKS_PER_SEC;
  printf("uint64 multiplication: %0.2f Mop/s (final = %ld)\n", numOps / timeSpent / 1.0e6, retLng);

  // long integer division
  tBegin = clock();
  retLng = uint64Divide(numOps, valLng);
  tEnd = clock();
  timeSpent = (double) (tEnd - tBegin) / CLOCKS_PER_SEC;
  printf("uint64 division: %0.2f Mop/s (final = %ld)\n", numOps / timeSpent / 1.0e6, retLng);

  // floating-point addition
  tBegin = clock();
  retDbl = doubleAdd(numOps, valDbl);
  tEnd = clock();
  timeSpent = (double) (tEnd - tBegin) / CLOCKS_PER_SEC;
  printf("double addition: %0.2f Mop/s (final = %0.2f)\n", numOps / timeSpent / 1.0e6, retDbl);

  // floating-point subtraction
  tBegin = clock();
  retDbl = doubleSubtract(numOps, valDbl);
  tEnd = clock();
  timeSpent = (double) (tEnd - tBegin) / CLOCKS_PER_SEC;
  printf("double subtraction: %0.2f Mop/s (final = %0.2f)\n", numOps / timeSpent / 1.0e6, retDbl);

  // floating-point multiplication
  tBegin = clock();
  retDbl = doubleMultiply(numOps, valDbl);
  tEnd = clock();
  timeSpent = (double) (tEnd - tBegin) / CLOCKS_PER_SEC;
  printf("double multiplication: %0.2f Mop/s (final = %0.2f)\n", numOps / timeSpent / 1.0e6, retDbl);

  // floating-point division
  tBegin = clock();
  retDbl = doubleDivide(numOps, valDbl);
  tEnd = clock();
  timeSpent = (double) (tEnd - tBegin) / CLOCKS_PER_SEC;
  printf("double division: %0.2f Mop/s (final = %0.2f)\n", numOps / timeSpent / 1.0e6, retDbl);

  return(0);
}


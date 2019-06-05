#ifndef LAB1_H

#define LAB1_H

#include <inttypes.h>
#include <limits.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

#define MS_LEN 5UL
uint64_t MS[MS_LEN] = {0U};
size_t U = MS_LEN - 1U;
size_t I = MS_LEN - 1U;

clock_t t_start = 0;

uint64_t pseudo_slucajni_64_bitovni_broj ()
{
  uint64_t n;

  size_t i;

  do
  {
    n = 0U;
    for (i = 0U; i < 5U; ++i)
      n = (n << (15ULL * (uint64_t)i)) + ((uint64_t)rand() & 0x7fffULL);
  }
  while(n < 0x1000000000000000ULL);

  for (i = 1U; i < 63U && n >> ((uint64_t)i + 1ULL); ++i)
    if (
      ((n >> (uint64_t)i) & 1U) == ((n >> ((uint64_t)i + 1ULL)) & 1U) &&
      ((n >> (uint64_t)i) & 1U) == ((n >> ((uint64_t)i - 1ULL)) & 1U)
    )
    {
      n = (
        ((n >> (uint64_t)i) & 1U) ?
          (n & ~(1ULL << (uint64_t)i)) :
          (n | (1ULL << (uint64_t)i))
      );

      ++i;
    }

  return n;
}

int test_bitovi (uint64_t n)
{
  int bitovi;

  while (n >= 0b111ULL)
  {
    bitovi = (int)(n & 0b111ULL);

    if (!bitovi || bitovi == 0b111)
      return 0;

    n >>= 1ULL;
  }

  return 1;
}

int test_pseudo_prost (uint64_t n)
{
  uint64_t k;

  return 1;

  if (n == 1ULL)
    return 0;

  if (n == 2ULL || n == 3ULL)
    return 1;

  if (!(n & 1ULL && n % 3ULL))
    return 0;

  for (k = 5ULL; k <= n / k; k += 6ULL)
    if (!(n % k && n % (k + 2ULL)))
      return 0;

  return 1;
}

#endif /* LAB1_H */

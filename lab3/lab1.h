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

void ispisi_MS ()
{
   size_t i;

   printf("{ ");
   for (i = 0U; i < MS_LEN; ++i)
     printf("%02" PRIx64 " ", MS[i] & 0xffU);
   printf("}");
}

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

int provjera_zahtjeva ()
{
  static clock_t t0 = 0U;
  clock_t t1;

  t1 = clock();

  if (!t0)
    t0 = t_start;

  if ((long double)(t1 - t0) / (CLOCKS_PER_SEC) < 1.0L)
    return 1;

  t0 = t1;

  if ((uint64_t)rand() / (((uint64_t)(RAND_MAX) + 1ULL) / 2ULL))
  {
    I = (I + 1U) % MS_LEN;

    printf("%.2LF - ", (long double)(t1 - t_start) / (CLOCKS_PER_SEC));
    printf("MS = ");
    ispisi_MS();
    printf(", ");
    printf("I = %lu, ", I);
    printf("U = %lu, ", U);
    printf("MS[I] = 0x%" PRIx64 "\n", MS[I]);

    return 1;
  }

  return (unsigned int)rand() / (((unsigned int)(RAND_MAX) + 1U) / 10U) ? 0 : 1;
}

#endif /* LAB1_H */

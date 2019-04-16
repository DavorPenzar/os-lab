#include <inttypes.h>
#include <limits.h>
#include <pthread.h>
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
    static const uint64_t A = 8531648002905263869ULL;
    static const uint64_t B = 18116652324302926351ULL;

    uint64_t n;

    size_t i;

    n = (uint64_t)rand() * A % B | 1U;

    for (i = 1U; n >> i; ++i)
      if (
        (n >> i & 1U) == (n >> i + 1 & 1U) &&
        (n >> i & 1U) == (n >> i - 1 & 1U)
      )
      {
        n = n >> i & 1U ? n & ~(1U << i) : n | 1U << i;

        ++i;
      }

    return n;
}

int test_bitovi (uint64_t n)
{
    int bitovi;

    while (n >= 0b111U)
    {
      bitovi = (int)(n & 0b111U);

      if (!bitovi || bitovi == 0b111U)
        return 0;

      n >>= 1U;
    }

    return 1;
}


int test_pseudo_prost (uint64_t n)
{
  uint64_t k;

  size_t i;

  return 1;

  if (n == 1U)
    return 0;

  if (n == 2U || n == 3U)
    return 1;

  if (!(n & 1U && n % 3U))
    return 0;

  for (i = 0U, k = 5U; i < (size_t)(INT_MAX) && k <= n / k; ++i, k += 6U)
    if (!(n % k && n % (k + 2U)))
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

/*
  if ((long double)(t1 - t0) / (CLOCKS_PER_SEC) < 1.0L)
    return 1;
*/

  t0 = t1;

  if (rand() & 1)
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

  return !(rand() % 10U);
}

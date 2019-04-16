/* Ukljucivanje standardne biblioteke. */
#include <inttypes.h>
#include <limits.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

/**
 * Provjeri ne sadrzi li 64-bitovni broj (bez predznaka) iste 3 uzastopne
 * binarne znamenke bez vodecih nula.
 *
 * Ako broj n ne sadrzi 3 iste uzastopne binarne znamenke, povratna vrijednost
 * je razlicita od 0; inace je jednaka 0.
 *
 * Slozenost funkcije reda je log(n) ako pretpostavimo da su sve "jednostavne"
 * operacije (operacije dobivene pozivom operatora) konstantne slozenosti.
 *
 */
int test_bitovi (uint64_t n)
{
  /* Pomocna varijabla koja "sprema" trenutna 3 uzastopna bita za provjeru. */
  int bitovi;

  /* Dok nisu iskusane sve kombinacije 3 uzastopna bita koje mogu biti jednake,
  provjeravaj trenutna 3 uzastopna bita.  Provjeravaju se prvo zadnja (najmanje
  znacajna) 3 bita, zatim prva 3 od posljednja 3 i tako dalje.  Nakon svake
  provjere broj n se transformira bitovnom operacijom shift udesno (brise se
  najmanje znacajni bit, iduci postaje najmanje znacajni i tako dalje --- broj u
  binarnom zapisu postaje kraci za 1 znamenku),  stoga je uvjet prekida
  n < 0b111.  Naime, bilo koja kombinacija bitova u intervalu [0 .. 0b111) ne
  sadrzi 3 uzastopna ista bita (ako zanemarimo vodece nule, sto i
  zanemarujemo). */
  while (n >= 0b111U)
  {
    /* Spremi trenutna posljednja (najmanje znacajna bita) u varijablu
    bitovi. */
    bitovi = (int)(n & 0b111U);

    /* Ako je bitovi == 0 (odnosno !bitovi) ili ako je bitovi == 0b111, prekini
    rad funkcije i vrati 0 (broj sadrzi 3 uzastopne iste binarne znamenke). */
    if (!bitovi || bitovi == 0b111U)
      return 0;

    /* Transforiraj broj n bitovnom operacijom shift udesno. */
    n >>= 1U;
  }

  /* Ako nijedna provjera kombinacija 3 uzastopna bita nije vratila 0, broj ne
  sadrzi 3 uzastopne iste binarne znamenke --- vrati vrijednost razlicitu od
  0. */
  return 1;
}

/**
 * Provjeri je li 64-bitovni broj (bez predznaka) prost.
 *
 * Ako je broj n prost, povratna vrijednost je razlicita od 0; inace je jednaka
 * 0.
 *
 * Slozenost funkcije reda je sqrt(n) ako pretpostavimo da su sve "jednostavne"
 * operacije (operacije dobivene pozivom operatora) konstantne slozenosti.
 *
 */
int test_pseudo_prost (uint64_t n)
{
  /* Pomocna varijabla.  Broj n > 1 je slozen (nije prost) ako je djeljiv s 2, 3
  ili nekim brojem oblika 6 * i +- 1 za i >= 1.  Naime, n > 0 je slozen ako je
  razlicit od 1 i ima barem jedan netrivijalni djelitelj, a, ako je djeljiv
  brojem oblika 6 * i + j za i >= 1 i j iz skupa {2, 3, 4}, onda je 6 * i + j
  djeljivo s 2 ili 3 pa je, po tranzitivnosti djeljivosti na skupu prirodnih
  brojeva, i n djeljiv s 2 ili 3 --- nakon provjere djeljivosti broja n s 2 i 3
  dovoljno je provjeravati djeljivost s brojevima oblika 6 * i +- 1 za i >= 1.
  U varijablu k "spremat cemo" prirodne visekratnike broja 6 razlicite od 0. */
  uint64_t k;

  /* Broj 1 po definiciji nije prost --- ako je n == 1, vrati 0. */
  if (n == 1U)
    return 0;

  /* Ako je n == 2 ili je n == 3, vrati vrijednost razlicitu od 0. */
  if (n == 2U || n == 3U)
    return 1;

  /* Ako je broj n djeljiv s 2 (ako je !(n & 1)) ili ako je n djeljiv s 3 (ako
  je !(n % 3)), vrati 0 --- broj n (vec je poznato da je n == 0 ili je n > 3) je
  jednak 0 ili je slozen, dakle, nije prost.  Provjera je reducirana
  DeMorganovim zakonom. */
  if (!(n & 1U && n % 3U))
    return 0;

  /* Za svaki prirodni visekratnik k broja 6 razlicit od 0 provjeri djeljivost
  broja n s k - 1 i k + 1 --- ako je broj n djeljiv takvim brojem (ako je
  !(n % (k +- 1))), vrati 0 --- broj je slozen (vec je poznato da je n > 3),
  dakle, nije prost.  Provjera je reducirana DeMorganovim zakonom.  Provjerava
  se djeljivost dok je k^2 <= n (dok je k <= n / k zbog prevencije "overflow-a")
  jer je broj n > 1 prost ako i samo ako nije djeljiv nijednim prirodnim brojem
  iz intervala (1, sqrt(n)]. */
  for (k = 6U; k <= n / k; k += 6U)
    if (!(n % (k - 1U) && n % (k + 1U)))
      return 0;

  /* Ako nijedna provjera djeljivosti broja n nije vratila 0, broje je prost ---
  vrati vrijednost razlicitu od 0. */
  return 1;
}

int main (int argc, char** argv)
{
  uint64_t MS[3U] = {
    0x32b59b56b554a65bULL,
    0x26d9695b26d65533ULL,
    0x4d6ab64ccd9b66dULL
  };

  size_t i;

  for (i = 0U; i <3U; ++i)
    printf("%d\n", test_bitovi(MS[i]));

  return EXIT_SUCCESS;
}

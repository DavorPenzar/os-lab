/**
 * RJESENJE 1. DOMACE ZADACE IZ OPERACIJSKIH SUSTAVA
 *
 * Autor: Davor Penzar
 * Datum: 3. IV. 2019.
 *
 */

/* Ukljucivanje standardne biblioteke. */
#include <inttypes.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

/**
 * Inicijalizacija meduspremnika (MS).
 *
 * Duljina meduspremnika (MS_LEN) postavljeno je na 5, meduspremnik (MS) je
 * inicijaliziran na niz nula, a indeks zadnje unesenog broja (U) je
 * inicijaliziran na MS_LEN - 1 i azurira se PRIJE unosa.
 *
 */
#define MS_LEN 5UL
uint64_t MS[MS_LEN] = {0};
size_t U = MS_LEN - 1;

/**
 * Definicija prvog otkucaja procesorskog sata.
 *
 * Prvi otkucaj procesorskog sata inicijalizira se u funkciji main(...), a ovdje
 * je, konsistentnosti radi, inicijaliziran na 0.
 *
 */
clock_t t_start = 0;

/**
 * Ispisi meduspremnik na stdout.
 *
 * Meduspremnik se ispisuje u formatu
 *   { x_0 x_1 ... x_{MS_LEN - 1} }
 * Pri cemu se svaka vrijednost meduspremnika ispisuje u obliku posljednje
 * (najmanje znacajne) znamenke te vrijednosti u heksadecimalnom zapisu.
 *
 */
void ispisi_MS ()
{
  /* Indeks iteracije. */
  size_t i;

  /* Ispisi meduspremnik na stdout u definiranom formatu. */
  printf("{ ");
  for (i = 0; i < MS_LEN; ++i)
    printf("%02" PRIx64 " ", MS[i] & 0xff);
  printf("}");
}

/**
 * Dohvati pseudo-slucajni 64-bitovni broj (bez predznaka).
 *
 * Za generiranje pseudo-slucajnog broja koristi se funkcija rand(...)
 * definirana u stdlib.h.
 *
 */
uint64_t pseudo_slucajni_64_bitovni_broj ()
{
  /* Konstante A, B bez predznaka.  Pseudo-slucajni broj generira se pozivom
       rand() * A % B
  */
  static const uint64_t A = 8531648002905263869ULL;
  static const uint64_t B = 18116652324302926351ULL;

  /* Vrati pseudo-slucajni 64-bitovni broj (bez predznaka) po gore definiranoj
  "formuli". */
  return (uint64_t)rand() * A % B;
}

/**
 * Provjeri ne sadrzi li 64-bitovni broj (bez predznaka) iste 3 uzastopne
 * binarne znamenke bez vodecih nula.
 *
 * Ako broj n ne sadrzi 3 iste uzastopne binarne znamenke, povratna vrijednost
 * je razlicita od 0; inace je jednaka 0.
 *
 * Slozenost funkcije reda je log_2(n) ako pretpostavimo da su sve "jednostavne"
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
  while (n >= 0b111)
  {
    /* Spremi trenutna posljednja (najmanje znacajna bita) u varijablu
    bitovi. */
    bitovi = (int)(n & 0b111);

    /* Ako je bitovi == 0 (odnosno !bitovi) ili ako je bitovi == 0b111, prekini
    rad funkcije i vrati 0 (broj sadrzi 3 uzastopne iste binarne znamenke). */
    if (!bitovi || bitovi == 0b111)
      return 0;

    /* Transforiraj broj n bitovnom operacijom shift udesno. */
    n >>= 1;
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
  /* Pomocna varijabla.  Broj n != 1 je slozen (nije prost) ako je djeljiv s 2,
  3 ili nekim brojem oblika (6 * i) +- 1 za i >= 1.  Naime, broj je slozen ako
  je razlicit od 1 i djeljiv barem jednim prostim brojem, a, ako je djeljiv
  brojem oblika (6 * i) + j za i >= 1 i j iz skupa {2, 3, 4}, onda je
  (6 * i) + j djeljivo s 2 ili 3 pa je, po tranzitivnosti djeljivosti na skupu
  prirodnih brojeva, i n djeljiv s 2 ili 3 --- nakon provjere djeljivosti broja
  n s 2 i 3 dovoljno je provjeravati djeljivost s brojevima oblika (6 * i) +- 1
  za i >= 1.  U varijabli k "spremat cemo" prirodne visekratnike broja 6
  razlicite od 0. */
  uint64_t k;

  /* Broj 1 po definiciji nije prost --- ako je n == 1, vrati 0. */
  if (n == 1)
    return 0;

  /* Ako je n == 2 ili je n == 3, vrati vrijednost razlicitu od 0. */
  if (n == 2 || n == 3)
    return 1;

  /* Ako je broj n djeljiv s 2 (ako je !(n & 1)) ili ako je n djeljiv s 3 (ako
  je !(n % 3)), vrati 0 --- broj n (vec je poznato da je n == 0 ili je n > 3) je
  jednak 0 ili je slozen, dakle, nije prost.  Provjera je reducirana
  DeMorganovim zakonom. */
  if (!(n & 1 && n % 3))
    return 0;

  /* Za svaki prirodni visekratnik k broja 6 razlicit od 0 provjeri djeljivost
  broja n s k - 1 i k + 1 --- ako je broj n djeljiv takvim brojem (ako je
  !(n % (k +- 1))), vrati 0 --- broj je slozen (vec je poznato da je n > 3),
  dakle, nije prost.  Provjera je reducirana DeMorganovim zakonom.  Provjerava
  se djeljivost dok je k^2 <= n jer je broj n > 1 prost ako i samo ako nije
  djeljiv nijednim prirodnim brojem iz intervala (1, sqrt(n)]. */
  for (k = 6; k <= n / k; k += 6)
    if (!((n % (k - 1)) && (n % (k + 1))))
      return 0;

  /* Ako nijedna provjera djeljivosti broja n nije vratila 0, broje je prost ---
  vrati vrijednost razlicitu od 0. */
  return 1;
}

/**
 * Provjeri zahtjev za novim brojem.
 *
 * Ako je izmedu dvije uzastopne provjere zahtjeva proslo strogo manje od 1
 * sekunde (pri prvom pozivu funkcije provjerava se vrijeme proslo od prvog
 * otkucaja procesorskog sata), funkcija vraca vrijednost razlicitu od 0. Inace
 * funkcija s vjerojatnosti 0.5 ispisuje sadrzaj meduspremnika na stdout i vraca
 * vrijednost razlicitu od 0, s vjerojatnosti 0.4 vraca vrijednost razlicitu
 * od 0 bez ispisa, a s vjerojatnosti 0.1 vraca 0 bez ispisa.
 *
 * Ispis spremnika vrsi se u formatu
 *   /t/ - MS = /MS/, I = /I/, U = /U/, MS[I] = /MS[I]/
 * gdje je
 *   --  t --  vrijeme (u sekundama) od pocetka pokretanja programa,
 *   --  MS  --  ispis dobiven pozivom funkcije ispisi_MS(...),
 *   --  I --  redni broj (pocevsi od 0) zadnjeg poziva funkcije modulo MS_LEN,
 *   --  U --  indeks zadnje unesene vrijednosti u meduspremnik,
 *   --  MS[I] --  heksadecimalni zapis vrijednosti meduspremnika na indeksu I.
 *
 * Za izracun vjerojatnosti koristi se funkcija rand(...) definirana u stdlib.h.
 *
 */
int provjera_zahtjeva ()
{
  /* Indeks posljednje ispisane vrijednosti u meduspremniku.  Indeks je
  inicijaliziran na 0, a azurira se NAKON ispisa.  S obzirom na to da je
  potrebno "pamtiti" indeks posljednje ispisane vrijednosti, varijabla I je
  staticka. */
  static size_t I = 0;

  /* Otkucaji procesorskog sata posljednje provjere zahtjeva i trenutne provjere
  zahjeva.  Otkucaj posljednje provjere zahtjeva inicijlizirana je na 0 (kasnije
  se azurira ako je jednaka 0).  S obzirom na to da je potrebno "pamtiti"
  vrijeme (otkucaj procesorskog sata) posljednje provjere zahtjeva, varijabla t0
  je staticka. */
  static clock_t t0 = 0;
  clock_t t1;

  /* Pomocna varijabla.  Kasnije se postavlja na vrijednost iz intervala [0, 1]
  uniformno ako pretpostavimo da funkcija rand(...) definirana u stdlib.h
  uniformno generira brojeve iz intevrala [0 .. RAND_MAX].  Zapravo, p moze
  poprimiti vrijednosti samo oblika k / RAND_MAX za k iz intervala
  [0 .. RAND_MAX] (do na preciznost racunalnog djeljenja), stoga nije prava
  kontinuirana uniformna slucajna varijabla. */
  long double p;

  /* Dohvati vrijednost otkucaja procesorskog sata na pocetku poziva funkcije
  i spremi u varijablu t1. */
  t1 = clock();

  /* Ako je t0 (inace vrijednost otkucaja procesorskog sata na pocetku zadnjeg
  poziva funkcije) jednaka 0, postavi t0 na t_start. */
  if (!t0)
    t0 = t_start;

  /* Ako je izmedu dva uzastopna poziva funkcije proslo strogo manje od 1
  sekunde, postavi t0 na t1 i vrati vrijednost razlicitu od 0. */
  if ((long double)(t1 - t0) / CLOCKS_PER_SEC < 1.0L)
  {
    /* Postavi t0 na t1. */
    t0 = t1;

    /* Vrati vrijednost razlicitu od 0. */
    return 1;
  }

  /* Postavi t0 na t1. */
  t0 = t1;

  /* Pseudo-slucajno generiraj vrijednost p iz intervala [0, 1]. */
  p = (long double)rand() / RAND_MAX;

  /* Ako je p < 0.5, ispisi meduspremnik u zadanom formatu, azuriraj varijablu
  I i vrati vrijednost razlicitu od 0. */
  if (p < 0.5L)
  {
    /* Ispisi meduspremnik u zadanom formatu. */
    printf("%.2LF - ", (long double)(t1 - t_start) / CLOCKS_PER_SEC);
    printf("MS = ");
    ispisi_MS();
    printf(", ");
    printf("I = %lu, ", I);
    printf("U = %lu, ", U);
    printf("MS[I] = 0x%" PRIx64 "\n", MS[I]);

    /* Azuriraj varijablu I. */
    I = (I + 1) % MS_LEN;

    /* Vrati vrijednost razlicitu od 0. */
    return 1;
  }

  /* Ako je p u intervalu [0.5, 0.6), vrati 0; inace vrati vrijednost razlicitu
  od 0. */
  return p < 0.6L ? 0 : 1;
}

/**
 * Glavni kod programa.
 *
 * Meduspremnik se popunjuje prostim brojevima koji ne sadrze 3 uzastopne iste
 * binarne znamenke dok provjera zahtjeva
 * (poziv funkcije provjera_zahtjeva(...)) ne vrati 0.
 *
 */
int main (int argc, char** argv)
{
  /* Pseudo-slucajni 64-bitovni broj (bez predznaka).  Inicijalizira se i
  azurira kasnije u petlji. */
  uint64_t x;

  /* Postavi "seed" pseudo-slucajnih brojeva pozivom funkcije rand(...)
  definirane u stdlib.h na time(NULL). */
  srand(time(NULL));

  /* Inicijaliziraj prvi otkucaj procesorskog sata. */
  t_start = clock();

  /* Ucini barem jednom, i cini dok funkcija provjera_zahtjeva() ne vrati 0,
  spremanje novog pseudo-slucajnog 64-bitovnog broja (bez predznaka) nad kojim
  pozivi funkcija test_bitovi(...) i test_pseudo_prost(...) ne vracaju 0. */
  do
  {
    /* Generiraj pseudo-slucajni 64-bitovni broj (bez predzanka) i spremi ga
    u varijablu x dok ne sadrzi 3 iste uzastopne iste binarne znamenke (dok je
    !test_bitovi(x)) i dok nije prost (dok je !test_pseudo_prost(x)).  Provjera
    je reducirana DeMorganovim zakonom. */
    do
      x = pseudo_slucajni_64_bitovni_broj();
    while (!(test_bitovi(x) && test_pseudo_prost(x)));

    /* Azuriraj varijablu U i upisi x u meduspremnik na indeksu U. */
    U = (U + 1) % MS_LEN;
    MS[U] = x;
  }
  while (provjera_zahtjeva());

  /* Uspjesno zavrsi program (vrati EXIT_SUCCESS). */
  return EXIT_SUCCESS;
}

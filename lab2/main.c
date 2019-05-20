/**
 * 2778
 *
 */

/* Standardna biblioteka. */
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

/* Funkcije iz lab1 (korigirane). */
#include "lab1.h"

/* Ukupni broj dretvi. */
size_t N_dretve = 0;

/* Polja ID-a dretvi i BROJ, ULAZ za Lamportov algoritam ukljucivanja/
 * iskljucivanja. */
unsigned int* ID = NULL;
unsigned int* BROJ = NULL;
unsigned int* ULAZ = NULL;

/* Globalna varijabla "kraj". */
int kraj = 0;

/* MUTEX lokoti (iako se provodi Lamportov algoritam, za sigurnost pristupa
 * memoriji koriste se i POSIX MUTEX lokoti). */
pthread_mutex_t IO_lokot;
pthread_mutex_t kraj_lokot;
pthread_mutex_t KO_ulaz_lokot;
pthread_mutex_t KO_broj_lokot;

/* Izlazna datoteka (stdout ili readme.txt). */
FILE* izlaz;

void udi_u_KO (unsigned int i)
{
  unsigned int j;

  /* Postavi ULAZ[i] na 1. */
  pthread_mutex_lock(&KO_ulaz_lokot);
  ULAZ[i] = 1U;
  pthread_mutex_unlock(&KO_ulaz_lokot);

  /* Dohvati max(BROJ) + 1 i upisi u BROJ[i]. */
  pthread_mutex_lock(&KO_broj_lokot);
  BROJ[i] = 0U;
  for (j = 0U; j < N_dretve; ++j)
    if (BROJ[j] > BROJ[i])
      BROJ[i] = BROJ[j];
  ++BROJ[i];
  pthread_mutex_unlock(&KO_broj_lokot);

  /* Postavi ULAZ[i] na 0. */
  pthread_mutex_lock(&KO_ulaz_lokot);
  ULAZ[i] = 0U;
  pthread_mutex_unlock(&KO_ulaz_lokot);

  /* Pricekaj svoj red. */
  for (size_t j = 0U; j < N_dretve; ++j)
  {
    /* "Preskoci sebe". */
    if (j == i)
      continue;

    /* Cekaj dok nije ULAZ[j] == 0. */
    while (1)
    {
      pthread_mutex_lock(&KO_ulaz_lokot);
      if (!ULAZ[j])
      {
        pthread_mutex_unlock(&KO_ulaz_lokot);

        break;
      }
      pthread_mutex_unlock(&KO_ulaz_lokot);
    }

    /* Cekaj dok nije BROJ[j] == 0 ili dok dretva nije
     * (BROJ[i], i) < (BROJ[j], j) leksikografski. */
    while (1)
    {
      pthread_mutex_lock(&KO_broj_lokot);
      if (!BROJ[j] || BROJ[j] > BROJ[i] || (BROJ[j] == BROJ[i] && j > i))
      {
        pthread_mutex_unlock(&KO_broj_lokot);

        break;
      }
      pthread_mutex_unlock(&KO_broj_lokot);
    }
  }
}

void izadi_iz_KO (unsigned int i)
{
  /* Postavi BROJ[i] na 0. */
  pthread_mutex_lock(&KO_broj_lokot);
  BROJ[i] = 0U;
  pthread_mutex_unlock(&KO_broj_lokot);
}

void* dretva_generiranje (void* arg)
{
  /* ID dretve. */
  unsigned int id;

  /* Generirani broj. */
  uint64_t x;

  /* Dohvati svoj ID. */
  id = *((size_t*)arg);

  /* Generiraj brojeve i upisuj ih u medusprenik. */
  while (1)
  {
    /* Ako je kraj, zavrsi. */
    pthread_mutex_lock(&kraj_lokot);
    if (kraj)
    {
      pthread_mutex_unlock(&kraj_lokot);

      break;
    }
    pthread_mutex_unlock(&kraj_lokot);

    /* Generiraj broj. */
    do
      x = pseudo_slucajni_64_bitovni_broj();
    while (!test_pseudo_prost(x)); /* Garantirano je da ce generirani broj
                                      zadovoljavati bitovni test. */

    /* Udi u KO, spremi broj u medusprenik; izadi iz KO. */
    udi_u_KO(id);
    U = (U + 1U) % MS_LEN;
    MS[U] = x;
    izadi_iz_KO(id);
  }

  return NULL;
}

void* dretva_provjeravanje (void* arg)
{
  /* ID dretve. */
  unsigned int id;

  /* Iscitani broj. */
  uint64_t y;

  /* Dohvati svoj ID. */
  id = *((size_t*)arg);

  /* Citaj brojeve iz meduspremnika i ispisuj na izlaz. */
  while (1)
  {
    /* Ako je kraj, zavrsi. */
    pthread_mutex_lock(&kraj_lokot);
    if (kraj)
    {
      pthread_mutex_unlock(&kraj_lokot);

      break;
    }
    pthread_mutex_unlock(&kraj_lokot);

    /* Udi u KO, ucitaj broj i ispisi ga; izadi iz KO. */
    udi_u_KO(id);
    I = (I + 1U) % MS_LEN;
    y = MS[I];
    pthread_mutex_lock(&IO_lokot);
    fprintf(izlaz, "%u, 0x%" PRIx64 ", uzeo broj.\n", id, y);
    pthread_mutex_unlock(&IO_lokot);
    izadi_iz_KO(id);

    /* Cekaj y mod 5 sekundi. */
    sleep(y % 5ULL);

    /* Ispisi potrosnju broja. */
    pthread_mutex_lock(&IO_lokot);
    fprintf(izlaz, "%u, 0x%" PRIx64 ", potrosio broj.\n", id, y);
    pthread_mutex_unlock(&IO_lokot);
  }

  return NULL;
}

int main (int argc, char** argv)
{
  /* Definicija varijabli (velicine polja dretvi, polja dretvi, indeksi
   * iteracija). */

  size_t N_generatori;
  size_t N_provjeravaci;

  pthread_t* generatori;
  pthread_t* provjeravaci;

  size_t i;
  size_t j;

  srand(time(NULL));

  t_start = clock();

  /* Inicijaliziraj kraj na 0. */
  kraj = 0;

  /* Inicjaliziraj MUTEX lokote. */
  pthread_mutex_init(&IO_lokot, NULL);
  pthread_mutex_init(&kraj_lokot, NULL);
  pthread_mutex_init(&KO_ulaz_lokot, NULL);
  pthread_mutex_init(&KO_broj_lokot, NULL);

  N_generatori =
    3U + (unsigned int)rand() / (((unsigned int)(RAND_MAX) + 1U) / 3U);
  N_provjeravaci =
    3U + (unsigned int)rand() / (((unsigned int)(RAND_MAX) + 1U) / 3U);

  N_dretve = N_generatori + N_provjeravaci;

  /* Za ispis na stfout zakomentirati drugu liniju, za ispis na readme.tx
   * zakomentirati prvu liniju. */
/*izlaz = stdout;*/
  izlaz = fopen("readme.txt", "at");

  if (!izlaz)
    exit(-0x1);

  /* Inicijaliziraj polja ID, BROJ, ULAZ. */

  ID = malloc(N_dretve * sizeof *ID);
  BROJ = malloc(N_dretve * sizeof *BROJ);
  ULAZ = malloc(N_dretve * sizeof *ULAZ);

  if (!(BROJ && ULAZ && ID))
  {
    fclose(izlaz);

    free(ID);
    free(BROJ);
    free(ULAZ);

    exit(0x1);
  }

  memset(BROJ, 0, N_dretve * sizeof *BROJ);
  memset(ULAZ, 0, N_dretve * sizeof *ULAZ);
  memset(ID, 0, N_dretve * sizeof *ID);

  /* Inicijaliziraj polja generatori, provjeravaci. */

  generatori = malloc(N_generatori * sizeof *generatori);
  provjeravaci = malloc(N_provjeravaci * sizeof *provjeravaci);

  if (!(generatori && provjeravaci))
  {
    fclose(izlaz);

    free(ID);
    free(BROJ);
    free(ULAZ);

    free(generatori);
    free(provjeravaci);

    exit(0x2);
  }

  /* Ispisi kolicine dretvi. */
  pthread_mutex_lock(&IO_lokot);
  fprintf(
    izlaz,
    "%lu dretvi: %lu generatora + %lu provjeravaca.\n\n",
      N_dretve,
      N_generatori,
      N_provjeravaci
  );
  pthread_mutex_unlock(&IO_lokot);

  /* Inicijaliziraj generatore. */
  for (i = 0U; i < N_generatori; ++i)
  {
    ID[i] = (unsigned int)i;
    if (pthread_create(generatori + i, NULL, dretva_generiranje, ID + i))
    {
      for (j = 0U; j < i; ++j)
        pthread_cancel(generatori[j]);

      fclose(izlaz);

      free(ID);
      free(BROJ);
      free(ULAZ);

      free(generatori);
      free(provjeravaci);

      exit(0x21);
    }
  }

  /* Cekaj 2 sekunde. */
  sleep(2U);

  /* Inicijaliziraj provjeravace. */
  for (i = 0U; i < N_provjeravaci; ++i)
  {
    ID[i] = (unsigned int)(N_generatori + i);
    if (pthread_create(provjeravaci + i, NULL, dretva_provjeravanje, ID + i))
    {
      for (j = 0U; j < N_generatori; ++j)
        pthread_cancel(generatori[j]);
      for (j = 0U; j < i; ++j)
        pthread_cancel(provjeravaci[j]);

      fclose(izlaz);

      free(ID);
      free(BROJ);
      free(ULAZ);

      free(generatori);
      free(provjeravaci);

      exit(0x22);
    }
  }

  /* Cekaj 30 sekundi. */
  sleep(30U);

  /* Postavi kraj na 1. */
  pthread_mutex_lock(&kraj_lokot);
  kraj = 1;
  pthread_mutex_unlock(&kraj_lokot);

  /* Zatvori sve dretve. */

  for (i = 0U; i < N_generatori; ++i)
  {
    if (pthread_join(generatori[i], NULL))
    {
      pthread_cancel(generatori[i]);
      for (j = i + 1U; j < N_generatori; ++j)
        pthread_cancel(generatori[j]);
      for (j = 0U; j < N_provjeravaci; ++j)
        pthread_cancel(provjeravaci[j]);

      fclose(izlaz);

      free(ID);
      free(BROJ);
      free(ULAZ);

      free(generatori);
      free(provjeravaci);

      exit(0x31);
    }
  }

  for (i = 0U; i < N_provjeravaci; ++i)
  {
    if (pthread_join(provjeravaci[i], NULL))
    {
      pthread_cancel(provjeravaci[i]);
      for (j = i + 1U; j < N_provjeravaci; ++j)
        pthread_cancel(provjeravaci[j]);

      fclose(izlaz);

      free(ID);
      free(BROJ);
      free(ULAZ);

      free(generatori);
      free(provjeravaci);

      exit(0x32);
    }
  }

  /* Oslobodi memoriju. */

  free(ID);
  free(BROJ);
  free(ULAZ);

  free(generatori);
  free(provjeravaci);

  if (!(izlaz == stdout || izlaz == stderr))
  {
    fprintf(izlaz, "\n\n\n");

    fclose(izlaz);
  }

  /* Uspjesno zavrsi program. */
  return EXIT_SUCCESS;
}

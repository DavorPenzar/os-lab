#if defined A && defined B
  #if A == B
    #undef A
    #undef B
  #endif /* A, B */
#endif /* A, B */

#if !(defined A && defined B)
  #if defined A
    #undef A
  #endif /* A */

  #if defined B
    #undef B
  #endif /* B */

  #define A 0
  #define B 1
#endif /* A, B */



#define VARIJANTA_ZADATKA A
/*#define VARIJANTA_ZADATKA B*/



#ifndef VARIJANTA_ZADATKA
  #define VARIJANTA_ZADATKA A
/*#define VARIJANTA_ZADATKA B*/
#endif /* VARIJANTA_ZADATKA */



#include <inttypes.h>
#include <limits.h>
#include <pthread.h>
#include <semaphore.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

#define KNRM  "\x1B[0m"
#define KRED  "\x1B[31m"
#define KGRN  "\x1B[32m"
#define KYEL  "\x1B[33m"
#define KBLU  "\x1B[34m"
#define KMAG  "\x1B[35m"
#define KCYN  "\x1B[36m"
#define KWHT  "\x1B[37m"

#include "lab1.h"

#define MEM_LEN 64UL
long int MEM[MEM_LEN] = {0L};

struct MEM_BLOK
{
  size_t i;
  size_t n;
  struct MEM_BLOK* prethodni;
  struct MEM_BLOK* sljedeci;
};

struct MEM_BLOK* blokovi = NULL;

size_t N_dretve = 0;

unsigned int* ID = NULL;

int kraj = 0;

pthread_mutex_t IO_lokot;
pthread_mutex_t kraj_lokot;

#if VARIJANTA_ZADATKA == A
sem_t KO_semafor;
sem_t novi_brojevi_semafor;
sem_t slobodna_memorija_semafor;
#elif VARIJANTA_ZADATKA == B
pthread_mutex_t KO_monitor;
pthread_cond_t novi_broj_uvjet;
pthread_cond_t slobodna_memorija_uvjet;
size_t novi_brojevi = 0;
#endif /* VARIJANTA_ZADATKA */

FILE* izlaz;

void inicijaliziraj_blokove ()
{
  blokovi = malloc(sizeof *blokovi);
  if (!blokovi)
    exit(-0x21);
  blokovi->i = 0U;
  blokovi->n = MEM_LEN;
  blokovi->prethodni = NULL;
  blokovi->sljedeci = NULL;
}

void oslobodi_blokove ()
{
  struct MEM_BLOK* aux;

  while (blokovi)
  {
    aux = blokovi->sljedeci;

    if (blokovi->sljedeci)
      blokovi->sljedeci->prethodni = NULL;

    blokovi->i = 0U;
    blokovi->n = 0U;
    blokovi->prethodni = NULL;
    blokovi->sljedeci = NULL;
    free(blokovi);

    blokovi = aux;
  }
}

int spoji_blokove_unazad (struct MEM_BLOK* blok0, struct MEM_BLOK* blok1)
{
  if (!(blok0 && blok1))
    return -1;
  if (blok0->i + blok0->n != blok1->i)
    return 1;

  blok0->n += blok1->n;

  if (blokovi == blok1)
  {
    blokovi = blokovi->sljedeci;
    if (blokovi)
      blokovi->prethodni = NULL;

    blok1->i = 0U;
    blok1->n = 0U;
    blok1->prethodni = NULL;
    blok1->sljedeci = NULL;
    free(blok1);
  }
  else
  {
    if (blok1->prethodni)
      blok1->prethodni->sljedeci = blok1->sljedeci;
    if (blok1->sljedeci)
      blok1->sljedeci->prethodni = blok1->prethodni;

    blok1->i = 0U;
    blok1->n = 0U;
    blok1->prethodni = NULL;
    blok1->sljedeci = NULL;
    free(blok1);
  }

  return 0;
}

int spoji_blokove_unaprijed (struct MEM_BLOK* blok0, struct MEM_BLOK* blok1)
{
  if (!(blok0 && blok1))
    return -1;
  if (blok0->i + blok0->n != blok1->i)
    return 1;

  blok1->i = blok0->i;
  blok1->n += blok0->n;

  if (blokovi == blok0)
  {
    blokovi = blokovi->sljedeci;
    if (blokovi)
      blokovi->prethodni = NULL;

    blok0->i = 0U;
    blok0->n = 0U;
    blok0->prethodni = NULL;
    blok0->sljedeci = NULL;
    free(blok0);
  }
  else
  {
    if (blok0->prethodni)
      blok0->prethodni->sljedeci = blok0->sljedeci;
    if (blok0->sljedeci)
      blok0->sljedeci->prethodni = blok0->prethodni;

    blok0->i = 0U;
    blok0->n = 0U;
    blok0->prethodni = NULL;
    blok0->sljedeci = NULL;
    free(blok0);
  }

  return 0;
}

int pomakni_blok_naprijed (struct MEM_BLOK* blok)
{
  int t;

  struct MEM_BLOK* aux0;
  struct MEM_BLOK* aux1;

  if (!blok)
    return -1;
  if (!blok->prethodni)
    return 1;

  t = blokovi == blok->prethodni ? 1 : 0;

  aux0 = blok->prethodni;
  aux1 = blok->sljedeci;

  if (blok->prethodni->prethodni)
    blok->prethodni->prethodni->sljedeci = blok;
  blok->prethodni = blok->prethodni->prethodni;
  blok->sljedeci = aux0;
  blok->sljedeci->prethodni = blok;
  blok->sljedeci->sljedeci = aux1;
  if (blok->sljedeci->sljedeci)
    blok->sljedeci->sljedeci->prethodni = blok->sljedeci;

  if (t)
    blokovi = blok;

  return 0;
}

int pomakni_blok_nazad (struct MEM_BLOK* blok)
{
  int t;

  struct MEM_BLOK* aux0;
  struct MEM_BLOK* aux1;

  if (!blok)
    return -1;
  if (!blok->sljedeci)
    return 1;

  t = blokovi == blok ? 1 : 0;

  aux0 = blok->prethodni;
  aux1 = blok->sljedeci;

  if (blok->sljedeci->sljedeci)
    blok->sljedeci->sljedeci->prethodni = blok;
  blok->sljedeci = blok->sljedeci->sljedeci;
  blok->prethodni = aux1;
  blok->prethodni->sljedeci = blok;
  blok->prethodni->prethodni = aux0;
  if (blok->prethodni->prethodni)
    blok->prethodni->prethodni->sljedeci = blok->prethodni;

  if (t)
    blokovi = blok->prethodni;

  return 0;
}

void udi_u_KO (unsigned int i)
{
#if VARIJANTA_ZADATKA == A
  sem_wait(&KO_semafor);
#elif VARIJANTA_ZADATKA == B
  pthread_mutex_lock(&KO_monitor);
#endif /* VARIJANTA_ZADATKA */
}

void izadi_iz_KO (unsigned int i)
{
#if VARIJANTA_ZADATKA == A
  sem_post(&KO_semafor);
#elif VARIJANTA_ZADATKA == B
  pthread_mutex_unlock(&KO_monitor);
#endif /* VARIJANTA_ZADATKA */
}

void cekaj_novi_broj (unsigned int i)
{
#if VARIJANTA_ZADATKA == A
  sem_wait(&novi_brojevi_semafor);
#elif VARIJANTA_ZADATKA == B
  pthread_mutex_lock(&KO_monitor);
  while (!novi_brojevi)
    pthread_cond_wait(&novi_broj_uvjet, &KO_monitor);
  --novi_brojevi;
  pthread_mutex_unlock(&KO_monitor);
#endif /* VARIJANTA_ZADATKA */
}

void objavi_novi_broj (unsigned int i)
{
#if VARIJANTA_ZADATKA == A
  sem_post(&novi_brojevi_semafor);
#elif VARIJANTA_ZADATKA == B
  pthread_mutex_lock(&KO_monitor);
  ++novi_brojevi;
  pthread_cond_signal(&novi_broj_uvjet);
  pthread_mutex_unlock(&KO_monitor);
#endif /* VARIJANTA_ZADATKA */
}

void cekaj_slobodnu_memoriju (unsigned int i)
{
#if VARIJANTA_ZADATKA == A
  sem_wait(&slobodna_memorija_semafor);
#elif VARIJANTA_ZADATKA == B
  pthread_mutex_lock(&KO_monitor);
  pthread_cond_wait(&slobodna_memorija_uvjet, &KO_monitor);
  pthread_mutex_unlock(&KO_monitor);
#endif /* VARIJANTA_ZADATKA */
}

void objavi_slobodnu_memoriju (unsigned int i)
{
#if VARIJANTA_ZADATKA == A
  sem_post(&slobodna_memorija_semafor);
#elif VARIJANTA_ZADATKA == B
  pthread_mutex_lock(&KO_monitor);
  pthread_cond_signal(&slobodna_memorija_uvjet);
  pthread_mutex_unlock(&KO_monitor);
#endif /* VARIJANTA_ZADATKA */
}

int zauzmi_memoriju (unsigned int i, size_t n)
{
  struct MEM_BLOK* blok;

  size_t r;

  if (!n)
    return 0;

  for (blok = blokovi; blok; blok = blok->sljedeci)
    if (n <= blok->n)
      break;

  if (!blok)
    return 1;

  for (r = 0U; r < n; ++r)
    MEM[blok->i + r] = i;

  blok->i += n;
  blok->n -= n;

  if (!blok->n)
  {
    if (blokovi == blok)
    {
      blokovi = blokovi->sljedeci;
      if (blokovi)
        blokovi->prethodni = NULL;

      blok->i = 0U;
      blok->n = 0U;
      blok->prethodni = NULL;
      blok->sljedeci = NULL;
      free(blok);
    }
    else
    {
      if (blok->prethodni)
        blok->prethodni->sljedeci = blok->sljedeci;
      if (blok->sljedeci)
        blok->sljedeci->prethodni = blok->prethodni;

      blok->i = 0U;
      blok->n = 0U;
      blok->prethodni = NULL;
      blok->sljedeci = NULL;
      free(blok);
    }

    return 0;
  }

  while (blok->prethodni)
  {
    if (
      blok->n > blok->prethodni->n ||
      (blok->n == blok->prethodni->n && blok->i > blok->prethodni->i)
    )
      break;

    pomakni_blok_naprijed(blok);
  }

  return 0;
}

int oslobodi_memoriju (unsigned int i)
{
  size_t n;

  struct MEM_BLOK* blok;

  struct MEM_BLOK* aux;

  size_t r;

  n = 0U;
  for (r = 0U; r < MEM_LEN; ++r)
    if (MEM[r] == i)
    {
      for (n = 0U; r + n < MEM_LEN; ++n)
      {
        if (MEM[r + n] != i)
          break;
        MEM[r + n] = -1L;
      }

      break;
    }

  if (!n)
    return 0;

  if (r >= MEM_LEN)
    return -1;

  blok = malloc(sizeof *blok);
  if (!blok)
    exit(-0x23);
  blok->i = r;
  blok->n = n;
  blok->prethodni = NULL;
  blok->sljedeci = blokovi;
  blokovi = blok;
  if (blokovi->sljedeci)
    blokovi->sljedeci->prethodni = blokovi;

  for (aux = blokovi; aux; aux = aux->sljedeci)
  {
    if (!spoji_blokove_unaprijed(aux, blok))
      aux = blok;
    if (!spoji_blokove_unazad(blok, aux))
      aux = blok;
  }

  while (blok->sljedeci)
  {
    if (
      blok->n < blok->sljedeci->n ||
      (blok->n == blok->sljedeci->n && blok->i < blok->sljedeci->i)
    )
      break;

    pomakni_blok_nazad(blok);
  }

  return 0;
}

void ispisi_uzeo_broj (unsigned int i, uint64_t x)
{
  fprintf(
    izlaz,
    "%s%u%s, %s0x%" PRIx64 "%s, %suzeo broj%s.\n",
      izlaz == stdout ? KYEL : "",
      i,
      izlaz == stdout ? KNRM : "",
      izlaz == stdout ? KWHT : "",
      x,
      izlaz == stdout ? KNRM : "",
      izlaz == stdout ? KMAG : "",
      izlaz == stdout ? KNRM : ""
  );
}

void ispisi_potrosio_broj (unsigned int i, uint64_t x)
{
  fprintf(
    izlaz,
    "%s%u%s, %s0x%" PRIx64 "%s, %spotrosio broj%s.\n",
      izlaz == stdout ? KYEL : "",
      i,
      izlaz == stdout ? KNRM : "",
      izlaz == stdout ? KWHT : "",
      x,
      izlaz == stdout ? KNRM : "",
      izlaz == stdout ? KCYN : "",
      izlaz == stdout ? KNRM : ""
  );
}

void ispisi_memoriju (void)
{
  int t;

  size_t i;

  t = 0;
  if (izlaz == stdout)
    fprintf(izlaz, KGRN);
  for (i = 0U; i < MEM_LEN; ++i)
  {
    if (MEM[i] == -1L)
    {
      if (t)
      {
        t = 0;
        if (izlaz == stdout)
          fprintf(izlaz, KGRN);
      }
      fprintf(izlaz, "-");
    }
    else
    {
      if (!t)
      {
        t = 1;
        if (izlaz == stdout)
          fprintf(izlaz, KRED);
      }
      fprintf(izlaz, "%lx", MEM[i]);
    }
  }
  if (izlaz == stdout)
    fprintf(izlaz, KNRM);
}

void* dretva_generiranje (void* arg)
{
  unsigned int id;

  uint64_t x;

  id = *((size_t*)arg);

  while (1)
  {
    pthread_mutex_lock(&kraj_lokot);
    if (kraj)
    {
      pthread_mutex_unlock(&kraj_lokot);

      break;
    }
    pthread_mutex_unlock(&kraj_lokot);

    do
      x = pseudo_slucajni_64_bitovni_broj();
    while (!test_pseudo_prost(x));

    udi_u_KO(id);
    U = (U + 1U) % MS_LEN;
    MS[U] = x;
    izadi_iz_KO(id);

    objavi_novi_broj(id);
  }

  return NULL;
}

void* dretva_provjeravanje (void* arg)
{
  unsigned int id;

  uint64_t y;

  int m;

  id = *((size_t*)arg);

  while (1)
  {
    pthread_mutex_lock(&kraj_lokot);
    if (kraj)
    {
      pthread_mutex_unlock(&kraj_lokot);

      break;
    }
    pthread_mutex_unlock(&kraj_lokot);

    cekaj_novi_broj(id);

    udi_u_KO(id);
    I = (I + 1U) % MS_LEN;
    y = MS[I];
    pthread_mutex_lock(&IO_lokot);
    ispisi_uzeo_broj(id, y);
    pthread_mutex_unlock(&IO_lokot);
    do
    {
      m = zauzmi_memoriju(id, y % 20ULL);
      if (m)
      {
        izadi_iz_KO(id);
        cekaj_slobodnu_memoriju(id);
        udi_u_KO(id);
      }
    }
    while (m);
    izadi_iz_KO(id);

    pthread_mutex_lock(&IO_lokot);
    fprintf(izlaz, "MEM: ");
    ispisi_memoriju();
    fprintf(izlaz, "\n");
    pthread_mutex_unlock(&IO_lokot);

    sleep(y % 5ULL);

    udi_u_KO(id);
    pthread_mutex_lock(&IO_lokot);
    ispisi_potrosio_broj(id, y);
    pthread_mutex_unlock(&IO_lokot);
    oslobodi_memoriju(id);
    izadi_iz_KO(id);
    objavi_slobodnu_memoriju(id);
  }

  return NULL;
}

int main (int argc, char** argv)
{
  size_t N_generatori;
  size_t N_provjeravaci;

  pthread_t* generatori;
  pthread_t* provjeravaci;

  size_t i;
  size_t j;

  srand(time(NULL));

  memset(MEM, 0xFF, MEM_LEN * sizeof *MEM);
  inicijaliziraj_blokove();

  kraj = 0;

  pthread_mutex_init(&IO_lokot, NULL);
  pthread_mutex_init(&kraj_lokot, NULL);

#if VARIJANTA_ZADATKA == A
  sem_init(&KO_semafor, 0, 1);
  sem_init(&novi_brojevi_semafor, 0, 0);
  sem_init(&slobodna_memorija_semafor, 0, 0);
#elif VARIJANTA_ZADATKA == B
  pthread_mutex_init(&KO_monitor, NULL);
  pthread_cond_init(&novi_broj_uvjet, NULL);
  pthread_cond_init(&slobodna_memorija_uvjet, NULL);
  novi_brojevi = 0;
#endif /* VARIJANTA_ZADATKA */

  N_generatori =
    3U + (unsigned int)rand() / (((unsigned int)(RAND_MAX) + 1U) / 3U);
  N_provjeravaci =
    3U + (unsigned int)rand() / (((unsigned int)(RAND_MAX) + 1U) / 3U);

  N_dretve = N_generatori + N_provjeravaci;

  izlaz = stdout;
/*izlaz = fopen("readme.txt", "at");*/

  if (!izlaz)
  {
    oslobodi_blokove();

    exit(-0x1);
  }

  ID = malloc(N_dretve * sizeof *ID);

  if (!ID)
  {
    if (!(izlaz == stdout || izlaz == stderr))
      fclose(izlaz);

    oslobodi_blokove();

    free(ID);

    exit(0x1);
  }

  memset(ID, 0, N_dretve * sizeof *ID);

  generatori = malloc(N_generatori * sizeof *generatori);
  provjeravaci = malloc(N_provjeravaci * sizeof *provjeravaci);

  if (!(generatori && provjeravaci))
  {
    if (!(izlaz == stdout || izlaz == stderr))
      fclose(izlaz);

    oslobodi_blokove();

    free(ID);

    free(generatori);
    free(provjeravaci);

    exit(0x2);
  }

  pthread_mutex_lock(&IO_lokot);
  fprintf(
    izlaz,
    "%lu dretvi: %lu generatora + %lu provjeravaca.\n\n",
      N_dretve,
      N_generatori,
      N_provjeravaci
  );
  pthread_mutex_unlock(&IO_lokot);

  for (i = 0U; i < N_generatori; ++i)
  {
    ID[i] = (unsigned int)i;
    if (pthread_create(generatori + i, NULL, dretva_generiranje, ID + i))
    {
      for (j = 0U; j < i; ++j)
        pthread_cancel(generatori[j]);

      if (!(izlaz == stdout || izlaz == stderr))
        fclose(izlaz);

      oslobodi_blokove();

      free(ID);

      free(generatori);
      free(provjeravaci);

      exit(0x31);
    }
  }

  sleep(2U);

  for (i = 0U; i < N_provjeravaci; ++i)
  {
    ID[i] = (unsigned int)(N_generatori + i);
    if (pthread_create(provjeravaci + i, NULL, dretva_provjeravanje, ID + i))
    {
      for (j = 0U; j < N_generatori; ++j)
        pthread_cancel(generatori[j]);
      for (j = 0U; j < i; ++j)
        pthread_cancel(provjeravaci[j]);

      if (!(izlaz == stdout || izlaz == stderr))
        fclose(izlaz);

      oslobodi_blokove();

      free(ID);

      free(generatori);
      free(provjeravaci);

      exit(0x32);
    }
  }

  sleep(30U);

  pthread_mutex_lock(&kraj_lokot);
  kraj = 1;
  pthread_mutex_unlock(&kraj_lokot);

  for (i = 0U; i < N_generatori; ++i)
  {
    if (pthread_join(generatori[i], NULL))
    {
      pthread_cancel(generatori[i]);
      for (j = i + 1U; j < N_generatori; ++j)
        pthread_cancel(generatori[j]);
      for (j = 0U; j < N_provjeravaci; ++j)
        pthread_cancel(provjeravaci[j]);

      if (!(izlaz == stdout || izlaz == stderr))
        fclose(izlaz);

      oslobodi_blokove();

      free(ID);

      free(generatori);
      free(provjeravaci);

      exit(0x41);
    }
  }

#if VARIJANTA_ZADATKA == A
  for (i = 0U; i < N_provjeravaci; ++i)
    sem_post(&novi_brojevi_semafor);
#elif VARIJANTA_ZADATKA == B
  novi_brojevi = N_provjeravaci;
#endif /* VARIJANTA_ZADATKA */

  for (i = 0U; i < N_provjeravaci; ++i)
  {
#if VARIJANTA_ZADATKA == A
#elif VARIJANTA_ZADATKA == B
    pthread_cond_broadcast(&novi_broj_uvjet);
#endif /* VARIJANTA_ZADATKA */

    if (pthread_join(provjeravaci[i], NULL))
    {
      pthread_cancel(provjeravaci[i]);
      for (j = i + 1U; j < N_provjeravaci; ++j)
        pthread_cancel(provjeravaci[j]);

      if (!(izlaz == stdout || izlaz == stderr))
        fclose(izlaz);

      oslobodi_blokove();

      free(ID);

      free(generatori);
      free(provjeravaci);

      exit(0x42);
    }
  }

  oslobodi_blokove();

  free(ID);

  free(generatori);
  free(provjeravaci);

  if (!(izlaz == stdout || izlaz == stderr))
  {
    fprintf(izlaz, "\n\n\n");

    fclose(izlaz);
  }

  return EXIT_SUCCESS;
}

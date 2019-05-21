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

#include "lab1.h"

size_t N_dretve = 0;

unsigned int* ID = NULL;

int kraj = 0;

pthread_mutex_t IO_lokot;
pthread_mutex_t kraj_lokot;

#if VARIJANTA_ZADATKA == A
sem_t KO_semafor;
sem_t novi_brojevi_semafor;
#elif VARIJANTA_ZADATKA == B
pthread_mutex_t novi_broj_monitor;
pthread_cond_t novi_broj_uvjet;
size_t novi_brojevi = 0;
#endif /* VARIJANTA_ZADATKA */

FILE* izlaz;

void udi_u_KO (unsigned int i)
{
#if VARIJANTA_ZADATKA == A
  sem_wait(&KO_semafor);
#elif VARIJANTA_ZADATKA == B
  pthread_mutex_lock(&novi_broj_monitor);
#endif /* VARIJANTA_ZADATKA */
}

void izadi_iz_KO (unsigned int i)
{
#if VARIJANTA_ZADATKA == A
  sem_post(&KO_semafor);
#elif VARIJANTA_ZADATKA == B
  pthread_mutex_unlock(&novi_broj_monitor);
#endif /* VARIJANTA_ZADATKA */
}

void cekaj_novi_broj (unsigned int i)
{
#if VARIJANTA_ZADATKA == A
  sem_wait(&novi_brojevi_semafor);
#elif VARIJANTA_ZADATKA == B
  pthread_mutex_lock(&novi_broj_monitor);
  while (!novi_brojevi)
    pthread_cond_wait(&novi_broj_uvjet, &novi_broj_monitor);
  --novi_brojevi;
  pthread_mutex_unlock(&novi_broj_monitor);
#endif /* VARIJANTA_ZADATKA */
}

void objavi_novi_broj (unsigned int i)
{
#if VARIJANTA_ZADATKA == A
  sem_post(&novi_brojevi_semafor);
#elif VARIJANTA_ZADATKA == B
  pthread_mutex_lock(&novi_broj_monitor);
  ++novi_brojevi;
  pthread_cond_signal(&novi_broj_uvjet);
  pthread_mutex_unlock(&novi_broj_monitor);
#endif /* VARIJANTA_ZADATKA */
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

    sleep(x % 5ULL);
  }

  return NULL;
}

void* dretva_provjeravanje (void* arg)
{
  unsigned int id;

  uint64_t y;

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
    fprintf(izlaz, "%u, 0x%" PRIx64 ", uzeo broj.\n", id, y);
    pthread_mutex_unlock(&IO_lokot);
    izadi_iz_KO(id);

    pthread_mutex_lock(&IO_lokot);
    fprintf(izlaz, "%u, 0x%" PRIx64 ", potrosio broj.\n", id, y);
    pthread_mutex_unlock(&IO_lokot);
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

  t_start = clock();

  kraj = 0;

  pthread_mutex_init(&IO_lokot, NULL);
  pthread_mutex_init(&kraj_lokot, NULL);

#if VARIJANTA_ZADATKA == A
  sem_init(&KO_semafor, 0, 1);
  sem_init(&novi_brojevi_semafor, 0, 0);
#elif VARIJANTA_ZADATKA == B
  pthread_mutex_init(&novi_broj_monitor, NULL);
  pthread_cond_init(&novi_broj_uvjet, NULL);
  novi_brojevi = 0;
#endif /* VARIJANTA_ZADATKA */

  N_generatori =
    3U + (unsigned int)rand() / (((unsigned int)(RAND_MAX) + 1U) / 3U);
  N_provjeravaci =
    3U + (unsigned int)rand() / (((unsigned int)(RAND_MAX) + 1U) / 3U);

  N_dretve = N_generatori + N_provjeravaci;

/*izlaz = stdout;*/
  izlaz = fopen("readme.txt", "at");

  if (!izlaz)
    exit(-0x1);

  ID = malloc(N_dretve * sizeof *ID);

  if (!ID)
  {
    fclose(izlaz);

    free(ID);

    exit(0x1);
  }

  memset(ID, 0, N_dretve * sizeof *ID);

  generatori = malloc(N_generatori * sizeof *generatori);
  provjeravaci = malloc(N_provjeravaci * sizeof *provjeravaci);

  if (!(generatori && provjeravaci))
  {
    fclose(izlaz);

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

      fclose(izlaz);

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

      fclose(izlaz);

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

      fclose(izlaz);

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

      fclose(izlaz);

      free(ID);

      free(generatori);
      free(provjeravaci);

      exit(0x42);
    }
  }

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

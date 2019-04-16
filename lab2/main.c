/**
 * 2778
 *
 */

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

#include "lab1.h"

size_t N_dretve = 0;

size_t* ID;

size_t* BROJ;
size_t* ULAZ;

int svrsi = 0;

pthread_mutex_t IO_lokot;
pthread_mutex_t kraj_lokot;
pthread_mutex_t KO_ulaz_lokot;
pthread_mutex_t KO_broj_lokot;

void udi_u_KO (size_t i)
{
  size_t j;

  pthread_mutex_lock(&KO_ulaz_lokot);
  ULAZ[i] = 1U;
  pthread_mutex_unlock(&KO_ulaz_lokot);

  pthread_mutex_lock(&KO_broj_lokot);
  BROJ[i] = 0U;
  for (j = 0U; j < N_dretve; ++j)
    if (BROJ[j] > BROJ[i])
      BROJ[i] = BROJ[j];
  ++BROJ[i];
  pthread_mutex_unlock(&KO_broj_lokot);

  pthread_mutex_lock(&KO_ulaz_lokot);
  ULAZ[i] = 0U;
  pthread_mutex_unlock(&KO_ulaz_lokot);

  for (size_t j = 0U; j < N_dretve; ++j)
  {
    if (j == i)
      continue;

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

    while (1)
    {
      pthread_mutex_lock(&KO_broj_lokot);
      if (!BROJ[j] || BROJ[j] > BROJ[i] || BROJ[j] == BROJ[i] && j > i)
      {
        pthread_mutex_unlock(&KO_broj_lokot);

        break;
      }
      pthread_mutex_unlock(&KO_broj_lokot);
    }
  }
}

void izadi_iz_KO (size_t i)
{
  pthread_mutex_lock(&KO_broj_lokot);
  BROJ[i] = 0U;
  pthread_mutex_unlock(&KO_broj_lokot);
}

void* dretva_generiranje (void* arg)
{
  size_t id;

  uint64_t x;

  id = *((size_t*)arg);

  while (1)
  {
    sleep(1U);

    do
      x = pseudo_slucajni_64_bitovni_broj();
    while (!test_pseudo_prost(x));

    udi_u_KO(id);
    U = (U + 1U) % MS_LEN;
    MS[U] = x;
    izadi_iz_KO(id);

    pthread_mutex_lock(&kraj_lokot);
    if (svrsi)
    {
      pthread_mutex_unlock(&kraj_lokot);

      break;
    }
    pthread_mutex_unlock(&kraj_lokot);
  }
}

void* dretva_provjeravanje (void* arg)
{
  pthread_t id;

  uint64_t y;

  id = *((size_t*)arg);

  while (1)
  {
    udi_u_KO(id);
    I = (I + 1U) % MS_LEN;
    y = MS[I];
    printf("%lu, 0x%" PRIx64 ", uzeo broj\n", id, y);
    izadi_iz_KO(id);

    sleep(y % 5U);

    pthread_mutex_lock(&IO_lokot);
    printf("%lu, 0x%" PRIx64 ", potrosio broj\n", id, y);
    pthread_mutex_unlock(&IO_lokot);

    pthread_mutex_lock(&kraj_lokot);
    if (svrsi)
    {
      pthread_mutex_unlock(&kraj_lokot);

      break;
    }
    pthread_mutex_unlock(&kraj_lokot);
  }
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

  svrsi = 0;

  pthread_mutex_init(&IO_lokot, NULL);
  pthread_mutex_init(&kraj_lokot, NULL);
  pthread_mutex_init(&KO_ulaz_lokot, NULL);
  pthread_mutex_init(&KO_broj_lokot, NULL);

  N_generatori = 3U + (unsigned int)rand() % 3U;
  N_provjeravaci = 3U + (unsigned int)rand() % 3U;

  N_dretve = N_generatori + N_provjeravaci;

  BROJ = malloc(N_dretve * sizeof *BROJ);
  ULAZ = malloc(N_dretve * sizeof *ULAZ);
  ID = malloc(N_dretve * sizeof *ID);

  if (!(BROJ && ULAZ && ID))
  {
    free(BROJ);
    free(ULAZ);
    free(ID);

    exit(0x1);
  }

  memset(BROJ, N_dretve * sizeof *BROJ, 0);
  memset(ULAZ, N_dretve * sizeof *ULAZ, 0);
  memset(ID, N_dretve * sizeof *ID, 0);

  generatori = malloc(N_generatori * sizeof *generatori);
  provjeravaci = malloc(N_provjeravaci * sizeof *provjeravaci);

  if (!(generatori && provjeravaci))
  {
    free(generatori);
    free(provjeravaci);
    free(BROJ);
    free(ULAZ);
    free(ID);

    exit(0x2);
  }

  for (i = 0U; i < N_generatori; ++i)
  {
    ID[i] = i;
    if (pthread_create(generatori + i, NULL, dretva_generiranje, ID + i))
    {
      for (j = 0U; j < i; ++j)
        pthread_cancel(generatori[j]);

        free(generatori);
        free(provjeravaci);
        free(BROJ);
        free(ULAZ);
        free(ID);

      exit(0x21);
    }
  }

  sleep(2U);

  for (i = 0U; i < N_provjeravaci; ++i)
  {
    ID[i] = N_generatori + i;
    if (pthread_create(provjeravaci + i, NULL, dretva_provjeravanje, ID + i))
    {
      for (j = 0U; j < N_generatori; ++j)
        pthread_cancel(generatori[j]);
      for (j = 0U; j < i; ++j)
        pthread_cancel(provjeravaci[j]);

        free(generatori);
        free(provjeravaci);
        free(BROJ);
        free(ULAZ);
        free(ID);

      exit(0x22);
    }
  }

  sleep(5U);

  pthread_mutex_lock(&kraj_lokot);
  svrsi = 1;
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

      free(generatori);
      free(provjeravaci);
      free(BROJ);
      free(ULAZ);
      free(ID);

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

      free(generatori);
      free(provjeravaci);
      free(BROJ);
      free(ULAZ);
      free(ID);

      exit(0x32);
    }
  }

  free(generatori);
  free(provjeravaci);
  free(BROJ);
  free(ULAZ);
  free(ID);

  return EXIT_SUCCESS;
}

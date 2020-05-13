/*
 * mmult.c
 * Author: Jared Spaulding
 * Date: May 15, 2020
 *
 * COSC 3750, Homework 11
 *
 * Matrix multiplier. See header for more details.
 */

#include "mmult.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <pthread.h>

void *mmult(void *arg) {
  struct arg_struct *args = arg;
  if (pthread_mutex_lock(&args->lock) != 0)
    perror("mmult");
  clock_t begin = clock();
  args->mp[args->col] = 0;
  double value;
  int i;
  for (i = 0; i < args->n; i++){
    value += args->m1[args->row*args->n + i] * args->m2[i*args->p + args->col];
  }
  args->mp[args->col] = value;
  args->col++;
  clock_t end = clock();
  args->time += (double)(end - begin) / CLOCKS_PER_SEC;
  if (pthread_mutex_unlock(&args->lock) != 0)
    perror("mmult");
  if (args->col == args->p) {
    if (pthread_cond_signal(&args->cv) != 0)
      perror("mmult");
    args->row++;
  }
  pthread_exit(NULL);
}

void printSummary(int m, int n, int p, int tds, double t) {
  printf("\n%s\n\t%s %d\n\t%s %d\n\t%s %d\n\n%s %d\n\n%s %f %s\n\n", 
    "Matrix sizes:", "M:", m, "N:", n, "P:", p, "Worker Threads:", tds,
    "Total time:", t, "seconds.");
}

int main(int argc, char **argv) {
  int arg = 0;
  FILE *fd1, *fd2, *fdOut;
  int m, n, p, tds;
  double t;

  if (argc > 4) {
    arg++;
    tds = atoi(argv[1]);
  } else {
    tds = 0;
  }

  if (argc == 1 || argc == 2) {
    printf("%s\n", "mmult: missing matrix arguments");
    return -1;
  } else if (argc == 3) {
    printf("%s\n", "mmult: missing output file");
    return -1;
  }

  // Open Fds
  if ((fd1 = fopen(argv[1 + arg], "r")) == NULL || 
    (fd2 = fopen(argv[2 + arg], "r")) == NULL ||
    (fdOut = fopen(argv[3 + arg], "w")) == NULL) {

    perror("mmult");
    return -1;
  }

  // Read row/cols files
  int temp; // to check if m1 col == m2 row
  if ( (fread(&m, 1, sizeof(int), fd1) == 0) ||
    (fread(&n, 1, sizeof(int), fd1) == 0) || 
    (fread(&temp, 1, sizeof(int), fd2) == 0) || 
    (fread(&p, 1, sizeof(int), fd2) == 0)) {

    perror("mmult");
    return -1;
  }

  // Check that m1 col == m2 row
  if (n != temp) {
    printf("%s\n", "mmult: matrices cannot be multiplied");
    return -1;
  }

  double m1[m * n];
  double m2[n * p];

  // Read values to store in m1
  int i,j;
  for (i = 0; i < m; i++){
    for (j = 0; j < n; j++) {
      if (fread(&m1[i*n + j], 1, sizeof(double), fd1) == 0) {
        perror("mmult");
        return -1;
      }
      if (m1[i*n + j] < 0) {
        printf("%s\n", "mmult: first matrix contains a negative value");
      }
    }
  }

  // Read values to store in m2
  for (i = 0; i < n; i++) {
    for (j = 0; j < p; j++) {
      if (fread(&m2[i*p + j], 1, sizeof(double), fd2) == 0) {
        perror("mmult");
        return -1;
      }
      if (m2[i*p + j] < 0) {
        printf("%s\n", "mmult: second matrix contains a negative value");
      }
    }
  }

  pclose(fd1);
  pclose(fd2);

  double mp[m * p];

  // Write product rows and cols to file
  if ((fwrite(&m, 1, sizeof(int), fdOut) == 0) ||
    (fwrite(&p, 1, sizeof(int), fdOut) == 0)) {

    perror("mmult");
    return -1;
  }

  // Multiply matrices
  if (argc < 5) {
    clock_t begin = clock();

    int k;
    for (i = 0; i < m; i++) {
      for (j = 0; j < p; j++) {
        mp[i*p + j] = 0;
        for (k = 0; k < n; k++) {
          mp[i*p + j] += m1[i*n + k] * m2[k*p + j];
        }
      }
    }

    clock_t end = clock();

    t = (double)(end - begin) / CLOCKS_PER_SEC;

    // Write product values to file
    for (i = 0; i < m; i++) {
      for (j = 0; j < p; j++) {
        if (fwrite(&mp[i*p + j], 1, sizeof(double), fdOut) == 0){
          perror("mmult");
          return -1;
        }
      }
    }
  } else {
    pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;
    pthread_cond_t cv = PTHREAD_COND_INITIALIZER;
    pthread_t td[tds];

    struct arg_struct args;
    args.lock = lock;
    args.cv = cv;
    args.m1 = m1;
    args.m2 = m2;
    args.n = n;
    args.p = p;
    args.row = 0;

    // Set thread stack size
    pthread_attr_t tattr;
    if (pthread_attr_init(&tattr) != 0 || 
      pthread_attr_setstacksize(&tattr, size) != 0) {
      perror("mmult");
      return -1;
    }

    for (i = 0; i < m; i++) {
      args.col = 0;
      args.mp = (double *)malloc(p * sizeof(double));
      for (j = 0; j < p; j++) {
        if (pthread_create(&td[j], &tattr, &mmult, &args) != 0){
          perror("mmult");
          return -1;
        }
      }
      if (pthread_cond_wait(&args.cv, &args.lock) != 0) {
        perror("mmult");
        return -1;
      }
      for (j = 0; j < p; j++) {
        if (fwrite(&args.mp[j], 1, sizeof(double), fdOut) == 0){
          perror("mmult");
          return -1;
        }
      }
      for (j = 0; j < p; j++) {
        if (pthread_join(td[j], NULL) != 0) {
          perror("mmult");
          return -1;
        }
      }
    }
    t = args.time;
  }

  pclose(fdOut);

  printSummary(m, n, p, tds, t);

  return 0;
}
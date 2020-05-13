/*
 * mmult.h
 * Author: Jared Spaulding
 * Date: May 15, 2020
 *
 * COSC 3750, Homework 11
 *
 * Multi threaded matrix multiplier.
 *
 * @examples
 * ./mmult 10 m1 m2 m3
 * ./mmult m1 m2 m3
 *
 */

#ifndef MMULT_H
#define MMULT_H

#include <pthread.h>
#include <limits.h>

size_t size = PTHREAD_STACK_MIN + 0x1000000;  // 16 MB

struct arg_struct {
  pthread_mutex_t lock;
  pthread_cond_t cv;
  int row;
  int col;
  int n;
  int p;
  double *m1;
  double *m2;
  double *mp;
  double time;
};

extern void *mmult(void *);
/*
 * Worker function for threading the matrix multiplication.
 * Computes one element of the product matix.
 */

extern void printSummary(int, int, int, int, double);
/*
 * Nice print of multiplication information
 */

extern int main(int, char **);
/*
 * Takes arguments of two matrix files and and out file.
 * Can take number of threads.
 * Performs matrix multiplication writting the product to outfile arg
 */

#endif
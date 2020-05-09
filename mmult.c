/*
 * mmult.c
 * Author: Jared Spaulding
 * Date: May 15, 2020
 *
 * COSC 3750, Homework 11
 *
 * Matrix multiplier. Number of threads used is defined as an argument
 */

#include "matrix.h"
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **argv) {
  if (argc < 5){
    // argv[1] is f1
    // argv[2] is f2
    // argv[3] is outputf

    if (argc == 1 || argc == 2) {
      printf("%s\n", "mmult: missing matrix arguments");
      exit(1);
    } else if (argc == 3) {
      printf("%s\n", "mmult: missing output file");
      exit(1);
    }

    // Open Fds
    FILE *fd1;
    FILE *fd2;
    FILE *fdOut;

    if ((fd1 = fopen(argv[1], "r")) == NULL || 
      (fd2 = fopen(argv[2], "r")) == NULL ||
      (fdOut = fopen(argv[3], "w")) == NULL) {
      perror("mmult");
    }

    // Read matrices from files
    struct matrix *m1 = parseMatrix(fd1);
    struct matrix *m2 = parseMatrix(fd2);
    if (m1 == NULL) {
      printf("%s\n", "mmult: An error occured reading first matrix");
      exit(1);
    }

    if (m2 == NULL) {
      printf("%s\n", "mmult: An error occured reading second matrix");
      exit(1);
    }

    // Compute multiplication
    struct matrix *m3 = multiplyMatrix(m1, m2);
    if (m3 != NULL) {
      // Write product matrix to outFile
      if (!writeMatrixToFile(m3, fdOut))
        exit(1);
    }
  } else {
    // argv[1] is threadc
    // argv[2] is f1
    // argv[3] is f2
    // argv[4] is outputf
  }
  return 0;
}
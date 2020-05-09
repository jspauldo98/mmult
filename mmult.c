/*
 * mmult.c
 * Author: Jared Spaulding
 * Date: May 15, 2020
 *
 * COSC 3750, Homework 11
 *
 * Matrix multiplier. Number of threads used is defined as an argument
 */

#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **argv) {
  // // test read
  // FILE *testF;
  // testF = fopen(argv[1], "r");
  // int testRow, testCol;
  // fread(&testRow, 1, sizeof(int), testF);
  // fread(&testCol, 1, sizeof(int), testF);
  // int i,j;
  // for (i = 0; i < testRow; i++) {
  //   for (j = 0; j < testCol; j++) {
  //     double value;
  //     fread(&value, 1, sizeof(double), testF);
  //     printf("%f\t", value);
  //   }
  //   printf("\n");
  // }

  FILE *fd1, *fd2, *fdOut;
  int m, n, p;

  if (argc == 1 || argc == 2) {
    printf("%s\n", "mmult: missing matrix arguments");
    return -1;
  } else if (argc == 3) {
    printf("%s\n", "mmult: missing output file");
    return -1;
  }

  // Open Fds
  if ((fd1 = fopen(argv[1], "r")) == NULL || 
    (fd2 = fopen(argv[2], "r")) == NULL ||
    (fdOut = fopen(argv[3], "w")) == NULL) {

    perror("mmult");
    return -1;
  }

  // Read matrix 1 from file
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

  // Mutiply matrices
  int k;
  for (i = 0; i < m; i++) {
    for (j = 0; j < p; j++) {
      for (k = 0; k < n; k++) {
        mp[i*p + j] += m1[i*n + k] * m2[k*p + j];
      }
    }
  }

  // Write product rows and cols to file
  if ((fwrite(&m, 1, sizeof(int), fdOut) == 0) ||
    (fwrite(&p, 1, sizeof(int), fdOut) == 0)) {

    perror("mmult");
    return -1;
  }

  // Write product values to file
  for (i = 0; i < m; i++) {
    for (j = 0; j < p; j++) {
      if (fwrite(&mp[i*p + j], 1, sizeof(double), fdOut) == 0){
        perror("mmult");
        return -1;
      }
    }
  }

  pclose(fdOut);

  return 0;
}
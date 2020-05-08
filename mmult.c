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

int main(int argc, char **argv) {

  if (argc < 4){
    // argv[1] is f1
    // argv[2] is f2
    // argv[3] is outputf
    FILE *fd1 = fopen(argv[1], "r");
    FILE *fd2 = fopen(argv[2], "r");
    struct matrix m1 = parseMatrix(fd1);
    struct matrix m2 = parseMatrix(fd2);
    printM(m1);
    printM(m2);

    struct matrix m3 = multiplyMatrix(m1, m2);
    printM(m3);

    pclose(fd1);
    pclose(fd2);
  } else {
    // argv[1] is threadc
    // argv[2] is f1
    // argv[3] is f2
    // argv[4] is outputf
  }
  return 0;
}
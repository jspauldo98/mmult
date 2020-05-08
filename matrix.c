/*
 * mmult.c
 * Author: Jared Spaulding
 * Date: May 15, 2020
 *
 * COSC 3750, Homework 11
 *
 * See header for more information
 */

#include <stdlib.h>
#include "matrix.h"
#include <stdio.h>

struct matrix parseMatrix(FILE *file) {
  // struct matrix *m = (struct matrix*)malloc(sizeof(struct matrix));
  struct matrix m;

  // Get row number
  fread(&m.rows, 1, 4, file);

  // Get column number
  fread(&m.cols, 1, 4, file);

  m.values = (double **)malloc(m.rows * sizeof(double **));

  int i,j;
  for (i = 0; i < m.rows; i++) {
    m.values[i] = (double *)malloc(m.cols * sizeof(double *));
    for (j = 0; j <m.cols; j++) {
      fread(&m.values[i][j], 1, 8, file);
    }
  }
  return m;
}

struct matrix multiplyMatrix(struct matrix m1, struct matrix m2) {
  struct matrix m3;
  m3.rows = m1.rows;
  m3.cols = m2.cols;
  m3.values = (double **)malloc(m3.rows * sizeof(double **));

  int i,j,k;
  for (i = 0; i < m1.rows; i++) {
    m3.values[i] = (double *)malloc(m2.cols * sizeof(double *));
    for (j = 0; j < m2.cols; j++) {
      for (k = 0; k < m1.cols; k ++) {
        m3.values[i][j] += m1.values[i][k] * m2.values[k][j];
      }
    }
  }

  return m3;
}

void printM(struct matrix m) {
  printf("%d\n", m.rows);
  printf("%d\n", m.cols);
  int i,j;
  for (i = 0; i <m.rows; i++) {
    for (j = 0; j < m.cols; j++) {
      printf("%f", m.values[i][j]);
    }
    printf("\n");
  }
}
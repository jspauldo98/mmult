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

struct matrix *parseMatrix(FILE *fd) {
  struct matrix *m = (struct matrix *)malloc(sizeof(struct matrix));

  // Read row and col numbers
  if ( (fread(&m->rows, 1, sizeof(int), fd) == 0) ||
    (fread(&m->cols, 1, sizeof(int), fd) == 0)) {
    perror("mmult");
    return NULL;
  }

  // Read values store in matrix struct
  m->values = (double **)malloc(m->rows * sizeof(double **));
  int i,j;
  for (i = 0; i < m->rows; i++) {
    m->values[i] = (double *)malloc(m->cols * sizeof(double *));
    for (j = 0; j < m->cols; j++) {
      if (fread(&m->values[i][j], 1, sizeof(double), fd) == 0){
        perror("mmult");
        return NULL;
      }
    }
  }

  pclose(fd);
  return m;
}

struct matrix *multiplyMatrix(struct matrix *m1, struct matrix *m2) {
  struct matrix *m3 = (struct matrix *)malloc(sizeof(struct matrix));
  m3->rows = m1->rows;
  m3->cols = m2->cols;

  if (m1->cols != m2->rows) {
    printf("%s\n", "mmult: matrices cannot be multiplied");
    return NULL;
  }

  m3->values = (double **)malloc(m3->rows * sizeof(double **));

  int i,j,k;
  for (i = 0; i < m1->rows; i++) {
    m3->values[i] = (double *)malloc(m2->cols * sizeof(double *));
    for (j = 0; j < m2->cols; j++) {
      for (k = 0; k < m1->cols; k ++) {
        m3->values[i][j] += m1->values[i][k] * m2->values[k][j];
      }
    }
  }

  return m3;
}

int writeMatrixToFile(struct matrix *m, FILE *fd) {
  // Write number of rows and cols
  if ((fwrite(&m->rows, 1, sizeof(int), fd) == 0) || 
    (fwrite(&m->cols, 1, sizeof(int), fd) == 0)) {
    perror("mmult");
    return 0;
  }

  // Write values
  int i,j;
  for (i = 0; i < m->rows; i++) {
    for (j = 0; j < m->cols; j++) {
      if (fwrite(&m->values[i][j], 1, sizeof(double), fd) == 0) {
        perror("mmult");
        return 0;
      }
    }
  }
  pclose(fd);

  return 1;
}

void printM(struct matrix *m) {
  // printf("%d\n", m->rows);
  // printf("%d\n", m->cols);
  int i,j;
  for (i = 0; i <m->rows; i++) {
    for (j = 0; j < m->cols; j++) {
      printf("%f", m->values[i][j]);
    }
    printf("\n");
  }
}
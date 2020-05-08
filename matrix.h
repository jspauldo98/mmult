/*
 * mmult.h
 * Author: Jared Spaulding
 * Date: May 15, 2020
 *
 * COSC 3750, Homework 11
 *
 * Contains functions for matrix maipulation
 */

#ifndef MATRIX_H
#define MATRIX_H

#include <stdio.h>

struct matrix {
  int rows;
  int cols;
  double **values;
};

extern struct matrix parseMatrix(FILE *);
/*
 * Returns a matrix given fd of matrix file
 */

extern struct matrix multiplyMatrix(struct matrix, struct matrix);
/*
 * Multiplies two matrices together and returns the product
 */

extern void printM(struct matrix);

#endif
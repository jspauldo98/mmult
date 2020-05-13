# mmult

mmult is a utility program that multiplies matrices given as arguments. This is a project for Linux Programming (COSC 3750).

## Prerequisites

GNU make is required.

## Installation

Use the GNU make utility to compile.

```bash
make
```

Use make_mat to generate matrices.

```bash
./make_mat matrix 4 5
  # Creates 4x5 matrix of random doubles file named matrix
```

## Synopsis

```bash
./mmult [#threads] [matrix 1] [matrix 2] [outfile]
```

## Description

Multiply matrices to outfile using or not using threading. Specified by args

## Examples
```bash
./mmult m1 m2 m3
    # multiply m1 by m2. Write product to m3 

./mmult 10 m1 m2 m3
    # multiply m1 by m2 using 10 worker threads. Write product to m3
```

## Author

Jared Spaulding

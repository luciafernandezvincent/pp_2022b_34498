#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <stdbool.h>

struct Pokemon {
  int type;
  double hp;
};

typedef struct Pokemon pokemon;

typedef enum {
    ST_OK,
    ST_ERR_NULL,
} status_t;


status_t parse_arguments(int argc, char *argv[], char *help, int *width, int *height, int *seed, int *n);
pokemon **matCreate( size_t rows, size_t cols );
void matOG( pokemon ***mat, size_t rows, size_t cols, int seed);
void matFuture( pokemon ***newmat,  size_t rows, size_t cols, pokemon ***mat);
void vecinos(int x, int y, pokemon ***mat,pokemon ***mat2, size_t rows, size_t cols);
void damage2(pokemon ***mat,pokemon ***mat2, size_t rows, size_t cols);
void print_ppm(pokemon ***mat, size_t rows, size_t cols, char *filename);
void n_ppm(pokemon ***mat, pokemon ***mat2, size_t rows, size_t cols, int n, int r);
void freeMat(pokemon ***mat, size_t rows, size_t cols);
int game(int argc, char *argv[]);
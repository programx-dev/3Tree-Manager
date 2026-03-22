#pragma once

#include <stdint.h>
#include <stdlib.h>

#define MIN_ARR_LEN 14
#define MAX_KEY_LEN 101

typedef struct
{
    char key[MAX_KEY_LEN];
    int64_t payload;
} Cell;

typedef struct
{
    Cell *data;
    size_t len;
    size_t allocated;
} Arr;

int64_t binSearch(char *key, Cell *data, int64_t l, int64_t r);

void binInsertationSort(Arr *arr, size_t start);
#include <stdio.h>
#include "algoritm.h"
#include <inttypes.h>
#include <stdlib.h>

void freeMemory(Arr *arr)
{
    if (arr)
    {
        free(arr->data);
        free(arr);
    }
}

void printArr(Arr *arr)
{
    for (size_t i = 0; i < arr->len; ++i)
    {
        printf("(%s %" PRId64 ") ", arr->data[i].key, arr->data[i].payload);
    }
    printf("\n");
}

void printKeysArr(Arr *arr)
{
    for (size_t i = 0; i < arr->len; ++i)
    {
        printf("%s", arr->data[i].key);
    }
    printf("\n");
}

int main(void)
{
    Arr *arr;

    arr = malloc(sizeof(*arr));

    if (!arr)
    {
        fprintf(stderr, "Can't to malloc");
        return 1;
    }

    arr->data = malloc(MIN_ARR_LEN * sizeof(Cell));

    if (!(arr->data))
    {
        fprintf(stderr, "Can't to malloc");
        free(arr);
        return 1;
    }

    arr->allocated = MIN_ARR_LEN;
    arr->len = 0;

    while (1)
    {
        Cell cell;

        if (scanf(" (%100s %" SCNd64 ")", cell.key, &(cell.payload)) != 2)
        {
            break;
        }

        if (arr->len + 1 > arr->allocated)
        {
            Cell *tmp_data = realloc(arr->data, arr->allocated * 2 * sizeof(Cell));
            if (tmp_data)
            {
                arr->data = tmp_data;
                arr->allocated *= 2;
            }
            else
            {
                fprintf(stderr, "Can't to realloc memory");
                freeMemory(arr);
                exit(1);
            }
        }

        arr->data[arr->len] = cell;
        arr->len += 1;

        // binInsertationSort(arr, ++start_sort); // Онлайн сортировка
    }

    printf("Source array:\n");
    printArr(arr);

    binInsertationSort(arr, 1);

    printf("Sorted array:\n");
    printArr(arr);

    char key[MAX_KEY_LEN];
    printf("\nKeys to search:\n");

    while (printf("Search key: "), scanf("%100s", key) == 1)
    {
        int64_t res = binSearch(key, arr->data, 0, (int64_t)arr->len - 1);

        if (res == -1)
            printf("Key '%s' not found\n", key);
        else
            printf("Key '%s' at index [%" PRId64 "]: payload = %" PRId64 "\n",
                   key, res, arr->data[res].payload);
    }

    freeMemory(arr);
    arr = NULL;

    return 0;
}
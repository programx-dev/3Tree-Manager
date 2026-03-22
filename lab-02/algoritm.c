#include "algoritm.h"

#include <string.h>
#include <inttypes.h>

int64_t binSearch(char *key, Cell *data, int64_t l, int64_t r)
{
    while (l != r + 1)
    {
        int64_t mid = (l + r) / 2;
        int cmp = strcmp(key, data[mid].key);

        if (cmp == 0)
        {
            return mid;
        }
        else if (cmp < 0)
        {
            r = mid - 1;
        }
        else
        {
            l = mid + 1;
        }
    }
    return -1;
}

size_t binSearchPlace(char *key, Cell *data, int64_t l, int64_t r)
{
    // Факт: цикл завершается когда l > r, те. l == r + 1

    while (l != r + 1)
    {
        int64_t mid = (l + r) / 2;
        int cmp = strcmp(key, data[mid].key);

        if (cmp < 0)
        {
            r = mid - 1;
        }
        else
        {
            l = mid + 1;
        }
    }

    // Инвариант: элементы из [0; l - 1] <= искомого
    // элементы из [r + 1; конец] > искомого

    // После цикла: ..., r, l, ... =>
    // Из инваранта => место l - искомое

    return l;
}

void binInsertationSort(Arr *arr, size_t start)
{
    // Область [0; start) явялется отсортированной

    for (size_t i = start; i < arr->len; ++i)
    {
        size_t place = binSearchPlace(arr->data[i].key, arr->data, 0, i - 1);
        Cell tmp = arr->data[i];

        for (size_t j = i; j > place; j--)
        {
            arr->data[j] = arr->data[j - 1];
        }

        arr->data[place] = tmp;
    }
}
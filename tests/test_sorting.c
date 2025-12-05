#include <assert.h>
#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>

void sort_array(uint32_t *arr, size_t size);

int main(void) {
    // basic shuffled case
    uint32_t a[] = {5, 3, 8, 1, 0, 7, 2, 4};
    uint32_t e1[] = {0, 1, 2, 3, 4, 5, 7, 8};
    sort_array(a, sizeof(a) / sizeof(a[0]));
    for (size_t i = 0; i < sizeof(a) / sizeof(a[0]); i++) assert(a[i] == e1[i]);

    // duplicates stay grouped
    uint32_t b[] = {4, 4, 2, 2, 4, 1};
    uint32_t e2[] = {1, 2, 2, 4, 4, 4};
    sort_array(b, sizeof(b) / sizeof(b[0]));
    for (size_t i = 0; i < sizeof(b) / sizeof(b[0]); i++) assert(b[i] == e2[i]);

    // empty
    sort_array(NULL, 0);

    // size 1
    uint32_t c[] = {42};
    sort_array(c, 1);
    assert(c[0] == 42);

    // already sorted
    uint32_t d[] = {1, 2, 3, 4, 5};
    sort_array(d, 5);
    for (size_t i = 0; i < 5; i++) assert(d[i] == i + 1);

    // reverse
    uint32_t e[] = {9, 7, 5, 3, 1};
    uint32_t er[] = {1, 3, 5, 7, 9};
    sort_array(e, 5);
    for (size_t i = 0; i < 5; i++) assert(e[i] == er[i]);

    return 0;
}
